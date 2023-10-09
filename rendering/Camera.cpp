#include "Camera.h"

#include "world/Player.h"

Camera::Camera()
{
    this->pos = glm::vec3(0.0f, 8.0f, -8.0f);
    this->center = glm::vec3(0.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->rotation = 0.0f;
    this->pitch = glm::radians(60.0f);
    this->r = 18.0f;
}

Camera::~Camera()
{
}

void Camera::setPos(glm::vec3& value)
{
    this->pos = value;
}

void Camera::setCenter(glm::vec3& value)
{
    this->center = value;
}

void Camera::setUp(glm::vec3& value)
{

    this->up = value;
}

void Camera::update(Player* player)
{
    this->pos = glm::vec3(0.0) + glm::vec3(r * glm::sin(rotation) * glm::cos(pitch), r * glm::sin(pitch), -r * glm::cos(rotation) * glm::cos(pitch));
    this->center = glm::vec3(0.0);
    this->view = glm::lookAt(pos, center, up);
}

glm::vec3 Camera::getPos() const
{
    return this->pos;
}

glm::vec3 Camera::getCenter() const
{
    return this->center;
}

void Camera::addRadian(float rot)
{
    this->rotation += rot;
}

void Camera::addPitch(float v)
{
    this->pitch += v;
    if (this->pitch > glm::radians(80.0)) this->pitch = glm::radians(80.0);
    if (this->pitch < glm::radians(45.0)) this->pitch = glm::radians(45.0);
}

void Camera::addR(float v)
{
    this->r += v;
    if (this->r >= 18.0f) this->r = 18.0f;
    else if (this->r <= 10.0f)
        this->r = 10.0f;
}

glm::mat4 const& Camera::getView() const
{
    return this->view;
}
