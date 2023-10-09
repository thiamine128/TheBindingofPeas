#include "Entity.h"

#include "ItemEntity.h"

class Good : public Entity
{
public:
    Good(World*, glm::vec3 const&, int);

    virtual void tick();
    virtual void render(const Shader*) const;
    virtual bool isItem() const;
protected:
    int item;
};