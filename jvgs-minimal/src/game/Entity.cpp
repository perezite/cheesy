#include "Entity.h"
#include "Level.h"
#include "Controller.h"
#include "InputController.h"
#include "CollisionResponsePositioner.h"
#include "../video/SketchyRenderer.h"

#include "../core/LogManager.h"
using namespace jvgs::core;

#include "../video/VideoManager.h"
using namespace jvgs::video;

#include "../tinyxml/tinyxml.h"

#include <algorithm>

using namespace std;
using namespace jvgs::math;
using namespace jvgs::sketch;

namespace jvgs
{
    namespace game
    {
        void Entity::loadData(TiXmlElement *element)
        {
            PropertyMap::loadData(element);

            /* Get id. */
            if(element->Attribute("id"))
                setId(element->Attribute("id"));

            /* Load basic data. */
            Vector2D position = getPosition();
            Vector2D::fromXML(element->FirstChildElement("position"),
                    &position);
            setPosition(position);

            Vector2D velocity = getVelocity();
            Vector2D::fromXML(element->FirstChildElement("velocity"),
                    &velocity);
            setVelocity(velocity);

            Vector2D::fromXML(element->FirstChildElement("radius"), &radius);

            queryBoolAttribute(element, "collisionchecker", &collisionChecker);

            float speed = getSpeed();;
            element->QueryFloatAttribute("speed", &speed);
            setSpeed(speed);

            /* Load controller. */
            TiXmlElement *controllerElement =
                    element->FirstChildElement("controller");
            if(controllerElement) {
                string type = controllerElement->Attribute("type");
				controller = getControllerInstance(type, controllerElement);
				if (controller == 0) 
					LogManager::getInstance()->error("No such controller: %s", type.c_str());
            }
			
			/* load polygon */
			TiXmlElement *pointsElement = 
				element->FirstChildElement("points");
			if (pointsElement) {
				const char *filename = pointsElement->Attribute("filename");
				if(filename) {
					shapePolygon = new math::Polygon(std::string(filename));
				}
			}

            /* Load positioner. */
            TiXmlElement *positionerElement =
                    element->FirstChildElement("positioner");
            if(positionerElement) {
                string type = positionerElement->Attribute("type");
				positioner = getPositionerInstance(type, positionerElement);
				if (positioner == 0) 
					LogManager::getInstance()->error("No such positioner: %s", type.c_str());
            }

            /* Load script. */
            script = element->Attribute("script") ?
                    element->Attribute("script") : script;
        }

        Entity::Entity(TiXmlElement *element, Level *level)
        {
			this->id = "none";
            this->level = level;
            position = Vector2D(0.0f, 0.0f);
            velocity = Vector2D(0.0f, 0.0f);
            speed = 0.3f;
            collisionChecker = false;
            controller = 0;
            positioner = 0;
            load(element);
			timer = 0.0f;
        }

        Entity::~Entity()
        {
            if(controller)
                delete controller;
            if(positioner)
                delete positioner;
        }

		Controller *Entity::getControllerInstance(
			const std::string& type, TiXmlElement *controllerElement) 
		{
			if (type == "inputcontroller") 
				return new InputController(this, controllerElement);
			else 
				return NULL;
		}

		CollisionResponsePositioner *Entity::getPositionerInstance(
					const std::string &type, TiXmlElement *positionerElement)
		{
			if(type=="collisionresponsepositioner")
				return new CollisionResponsePositioner(this, positionerElement);
			else 
				return NULL;
		}


        const string &Entity::getId() const
        {
            return id;
        }

        void Entity::setId(const string &id)
        {
            this->id = id;
        }

		Level *Entity::getLevel() const
        {
            return level;
        }

        const Vector2D &Entity::getPosition() const
        {
            return position;
        }

        void Entity::setPosition(const Vector2D &position)
        {
            this->position = position;
        }

        const Vector2D &Entity::getVelocity() const
        {
            return velocity;
        }

        void Entity::setVelocity(const Vector2D &velocity)
        {
            this->velocity = velocity;
        }

        float Entity::getSpeed() const
        {
            return speed;
        }

        void Entity::setSpeed(float speed)
        {
            this->speed = speed;
        }

        const Vector2D &Entity::getRadius() const
        {
            return radius;
        }

        void Entity::setRadius(const Vector2D &radius)
        {
            this->radius = radius;
        }

        bool Entity::isCollisionChecker() const
        {
            return collisionChecker;
        }

        void Entity::setCollisionChecker(bool collisionChecker)
        {
            this->collisionChecker = collisionChecker;
        }

        void Entity::setController(Controller *controller)
        {
            if(this->controller)
                delete this->controller;
            this->controller = controller;
        }

        Controller *Entity::getController() const
        {
            return controller;
        }

        void Entity::setPositioner(CollisionResponsePositioner *positioner)
        {
            if(this->positioner)
                delete this->positioner;
            this->positioner = positioner;
        }

        CollisionResponsePositioner *Entity::getPositioner() const
        {
            return positioner;
        }

        void Entity::setScript(const string &script)
        {
            this->script = script;
        }

        const string &Entity::getScript() const
        {
            return script;
        }

        float Entity::getTimer() const
        {
            return timer;
        }

        void Entity::setTimer(float timer)
        {
            this->timer = timer;
        }

        BoundingBox &Entity::getBoundingBox()
        {			
			return getWorldPolygon()->getBoundingBox();
        }

		math::Polygon *Entity::getWorldPolygon() 
		{
			worldPolygon = *shapePolygon;
			worldPolygon.translate(getPosition());
			return &worldPolygon;
		}

        void Entity::update(float ms)
        {
            if(controller)
                controller->affect(ms);
            if(positioner)
                positioner->affect(ms);

            /* Check for collision. */
            if(collisionChecker) {
                BoundingBox &myBoundingBox = getBoundingBox();
                for(int i = 0; i < getLevel()->getNumberOfEntities(); i++) {
                    Entity *other = getLevel()->getEntity(i);
                    if(this != other) {
                        /* Collision found. */
                        if(myBoundingBox.intersectsWith(
                                &other->getBoundingBox(), getVelocity() * ms,
                                other->getVelocity() * ms)) {
							// do something
                        }
                    }
                }
            }
        }

        void Entity::render()
        {
			/* render bounding box */
			BoundingBox &boundingBox = getBoundingBox();
			VideoManager *videoManager = VideoManager::getInstance();	
			videoManager->push();
			videoManager->translate(getPosition());
			videoManager->pop();
			SketchyRenderer renderer;
			
			renderer.begin(RENDERTYPE_LINE_LOOP);
			math::Vector2D topLeft = boundingBox.getTopLeft();
			math::Vector2D bottomRight = boundingBox.getBottomRight();
			renderer.vector(topLeft);
			renderer.vector(math::Vector2D(bottomRight.getX(), 
				topLeft.getY()));
			renderer.vector(bottomRight);
			renderer.vector(math::Vector2D(topLeft.getX(), 
				bottomRight.getY()));
			renderer.end();

			/* render shape polygon */
			Color oldColor = videoManager->getColor();
			videoManager->setColor(Color(1.0f, 0.0f, 0.0f, 1.0f));
			videoManager->push();
			videoManager->translate(getPosition());
			videoManager->pop();

			renderer.begin(RENDERTYPE_LINE_LOOP);
			math::Polygon *shapePolygon = getWorldPolygon();
			unsigned int nPoints = shapePolygon->getNumPoints();
			for (int i = 0; i < (int)nPoints; i++) {
				renderer.vector(shapePolygon->getPoint(i));
			}
			renderer.end();
			videoManager->setColor(oldColor);

        }
		
        void Entity::set(const std::string &key,
                const std::string &value)
        {
            PropertyMap::set(key, value);
        }

        void Entity::setBool(const std::string &key, bool value)
        {
            PropertyMap::setBool(key, value);
        }

        void Entity::setGarbage()
        {
            View::setGarbage();
        }
    }
}
