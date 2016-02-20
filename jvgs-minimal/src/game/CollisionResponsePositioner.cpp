#include "CollisionResponsePositioner.h"
#include "Entity.h"
#include "Level.h"

#include "../input/InputManager.h"
using namespace jvgs::input;

#include "../sketch/Sketch.h"
using namespace jvgs::sketch;

#include "../math/BoundingBox.h"
#include "../math/Vector2D.h"
#include "../math/Polygon.h"
using namespace jvgs::math;

#include "../tinyxml/tinyxml.h"

#include <iostream>
using namespace std;

namespace jvgs
{
    namespace game
    {
        const float CollisionResponsePositioner::VERY_CLOSE = 0.1f;
        const int CollisionResponsePositioner::MAX_STEPS = 10;
        const float CollisionResponsePositioner::SLIP_LIMIT = 0.1f;

        void CollisionResponsePositioner::loadData(TiXmlElement *element)
        {
			Vector2D::fromXML(element->FirstChildElement("gravity"), &gravity);
            element->QueryFloatAttribute("jumpdistancelimit",
                    &jumpDistanceLimit);
        }

        CollisionResponsePositioner::CollisionResponsePositioner(Entity *entity)
			: Affector(entity)
        {
            setGravity(Vector2D(0.0f, 0.0f));
            jumpDistanceLimit = 100.0f;
        }

        CollisionResponsePositioner::CollisionResponsePositioner(Entity *entity,
                TiXmlElement *element)
			: Affector(entity)
        {
            setGravity(Vector2D(0.0f, 0.0f));
            jumpDistanceLimit = 100.0f;
            load(element);
        }

        CollisionResponsePositioner::~CollisionResponsePositioner()
        {
        }

        const Vector2D &CollisionResponsePositioner::getGravity() const
        {
            return gravity;
        }

        void CollisionResponsePositioner::setGravity(const Vector2D &gravity)
        {
            this->gravity = gravity;
        }

        void CollisionResponsePositioner::affect(float ms)
        {
			Entity *entity = getEntity();
            CollisionDetector *collisionDetector =
                    entity->getLevel()->getCollisionDetector();

			/* Check for collision between shape polygon and world */
			math::Polygon *worldPolygon = entity->getWorldPolygon();
			Vector2D velocity = entity->getVelocity();
			bool worldPointCollision = 
				collisionDetector->willCollide(worldPolygon, velocity, ms);

            /* Now set the entity's state. */
			if (worldPointCollision == false) {
				entity->setPosition(entity->getPosition() + entity->getVelocity() * ms);
				entity->setVelocity(entity->getVelocity() + getGravity() * ms);
			} else {
				entity->setVelocity(math::Vector2D(0.0f, 0.0f));
			}
        }

        bool CollisionResponsePositioner::canJump(float ms)
        {  
			// can jump, if standing on the world
			Entity *entity = getEntity();
			CollisionDetector *collisionDetector =
				entity->getLevel()->getCollisionDetector();
			math::Polygon *worldPolygon = entity->getWorldPolygon();
			Vector2D velocity = entity->getVelocity();

			if (collisionDetector->willCollide(worldPolygon, velocity, ms)) {
				return true;
			}

			return false;
        }
    }
}
