#include "WorldRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Camera.h"
#include "Model.h"
#include "ModelManager.h"
#include "Game.h"
#include "Window.h"
#include "ShaderManager.h"
#include "TextureManager.h"

WorldRenderer::WorldRenderer()
{
    // �����������
    this->camera = new Camera();
    
    //������ɫ��
    ShaderManager::load();
    
    //��ʼ��͸�Ӿ���
    this->projection = glm::mat4(1.0);
    this->lightProjection = glm::ortho(-12.0f, 12.0f, -12.0f, 12.0f, -12.0f, 12.0f);

    this->updateProjection();

    this->defaultShader = ShaderManager::getDefault();

    //��ʼ�����ͼ��gbuffer
    this->initShadowMap();
    this->initGBuffer();

    //��ʼ��ֱ��vao
    this->linesVbo = new VertexBufferObject();
    this->linesVao = new VertexArrayObject();
    this->linesEbo = new ElementBufferObject();
}

WorldRenderer::~WorldRenderer()
{
    
}

void WorldRenderer::setWorld(World* world)
{
    this->world = world;
}

World *WorldRenderer::getWorld() const
{
    return this->world;
}

void WorldRenderer::render()
{
    if (this->world != nullptr)
    {
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        Player* player = this->world->getPlayer();

        this->camera->update(player);

        ShaderManager::setupUniforms(this);

        glViewport(0, 0, this->shadowWidth, this->shadowHeight);
        this->renderToDepthMap();
        glViewport(0, 0, Window::getInstance()->getWidth(), Window::getInstance()->getHeight());
        
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        this->renderDefault();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glEnable(GL_BLEND);
    }  
}

Camera* WorldRenderer::getCamera() const
{
    return this->camera;
}

void WorldRenderer::renderToDepthMap()
{
    this->depthMapFramebuffer->bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    this->renderWorld(ShaderManager::getShadow(), true);
    this->depthMapFramebuffer->unbind();
}

void WorldRenderer::renderDefault()
{
    glClearColor(0.5f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE1);
    this->depthMap->bind();
    this->renderWorld(this->defaultShader, false);
    if (Game::getInstance()->getController()->useMouseControl())
        this->renderSelected();
}

void WorldRenderer::renderWorld(const Shader* shader, bool shadow)
{
    shader->use();
    this->world->getPlayer()->render(shader);
    this->world->getRoom()->render(shader);
}

void WorldRenderer::updateProjection()
{
    this->projection = glm::perspective(glm::radians(45.0f), (float) Window::getInstance()->getWidth() / (float) Window::getInstance()->getHeight(), 0.1f, 100.0f);

    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Window::getInstance()->getWidth(), Window::getInstance()->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, gColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Window::getInstance()->getWidth(), Window::getInstance()->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Window::getInstance()->getWidth(), Window::getInstance()->getHeight());
}

glm::vec3 WorldRenderer::getWorldPos(glm::vec2 const& viewpos) const
{
    float x = 2.0f * viewpos.x / Window::getInstance()->getWidth() - 1.0f;
    float y = 1.0f - 2.0f * viewpos.y / Window::getInstance()->getHeight();
    glm::vec4 rayclip = glm::vec4(x, y, -1.0f, 1.0f);
    glm::vec4 worldpos = glm::inverse(this->projection) * rayclip;
    worldpos.z = -1.0f, worldpos.w = 0.0f;
    worldpos = glm::inverse(this->getView()) * worldpos;
    glm::vec3 result = glm::vec3(worldpos.x, worldpos.y, worldpos.z);
    glm::vec3 cpos = this->camera->getPos();
    
    if (result.y != 0.0f)
        result /= result.y;
    result = cpos - cpos.y * result;
    return result;
}

void WorldRenderer::initShadowMap()
{
    this->shadowWidth = this->shadowHeight = 1024;
    this->depthMapFramebuffer = new FramebufferObject();
    this->depthMap = new Texture();
    this->depthMap->bind();
    this->depthMap->setupDepthMap(this->shadowWidth, this->shadowHeight);
    this->depthMapFramebuffer->bind();
    this->depthMap->attachToFramebuffer(GL_DEPTH_ATTACHMENT);
    this->depthMapFramebuffer->setDrawBuffer(GL_NONE);
    this->depthMapFramebuffer->setReadBuffer(GL_NONE);
    this->depthMapFramebuffer->unbind();
}

void WorldRenderer::initGBuffer()
{
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Window::getInstance()->getWidth(), Window::getInstance()->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gNormal, 0);

    glGenTextures(1, &gColor);
    glBindTexture(GL_TEXTURE_2D, gColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Window::getInstance()->getWidth(), Window::getInstance()->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gColor, 0);

    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);

    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Window::getInstance()->getWidth(), Window::getInstance()->getHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WorldRenderer::renderHint(AABB const& aabb, glm::vec3 const& dst, glm::vec3 const& ded) const
{
    this->linesVbo->bind();
    glm::vec3 vert[10] =
    {
        {aabb.getMinX(), aabb.getMinY(), aabb.getMinZ()},
        {aabb.getMaxX(), aabb.getMinY(), aabb.getMinZ()},
        {aabb.getMaxX(), aabb.getMaxY(), aabb.getMinZ()},
        {aabb.getMinX(), aabb.getMaxY(), aabb.getMinZ()},
        {aabb.getMinX(), aabb.getMinY(), aabb.getMaxZ()},
        {aabb.getMaxX(), aabb.getMinY(), aabb.getMaxZ()},
        {aabb.getMaxX(), aabb.getMaxY(), aabb.getMaxZ()},
        {aabb.getMinX(), aabb.getMaxY(), aabb.getMaxZ()},
        dst + glm::vec3(0.0, 0.001, 0.0),
        ded + glm::vec3(0.0, 0.001, 0.0)
    };
    unsigned int indices[26] =
    {
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        4, 5,
        5, 6,
        6, 7,
        7, 4,
        0, 4,
        1, 5,
        2, 6,
        3, 7,
        8, 9
    };
    this->linesVao->bind();
    this->linesVbo->bind();
    this->linesVbo->bufferDataDynamic(sizeof(vert), vert);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    this->linesEbo->bind();
    this->linesEbo->bufferDataDynamic(sizeof(indices), indices);
    glEnableVertexAttribArray(0);
    ShaderManager::getLine()->use();
    glLineWidth(2.0f);
    glDrawElements(GL_LINES, 26, GL_UNSIGNED_INT, 0);
}

void WorldRenderer::renderSelected()
{
    glm::vec3 pos = this->getWorldPos(glm::vec2(Game::getInstance()->getController()->getMouseX(), Game::getInstance()->getController()->getMouseY()));
    AABB aabb = AABB(glm::vec3(pos.x - 0.3, 0.001, pos.z - 0.3), glm::vec3(pos.x + 0.3, 0.6, pos.z + 0.3));
    this->renderHint(aabb, world->getPlayer()->getPosFrame(), pos);
}

glm::vec3 WorldRenderer::getSunPosition() const
{
    return this->camera->getCenter() + glm::vec3(1.0, 2.0, -1.0);
}

glm::mat4 WorldRenderer::getView() const
{
    return this->camera->getView();
}

glm::mat4 WorldRenderer::getLightSpace() const
{
    return this->lightProjection * glm::lookAt(this->getSunPosition(), this->camera->getCenter(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 WorldRenderer::getProjection() const
{
    return this->projection;
}

void WorldRenderer::bindPostTex()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gColor);
}

GLuint WorldRenderer::getGColor() const
{
    return gColor;
}

GLuint WorldRenderer::getGNormal() const
{
    return gNormal;
}

GLuint WorldRenderer::getDepthMap() const
{
    return depthMap->getId();
}
