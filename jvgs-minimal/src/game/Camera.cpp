#include "Camera.h"
#include "Entity.h"
#include "Level.h"

#include "../core/LogManager.h"
using namespace jvgs::core;

using namespace std;

#include "../video/VideoManager.h"
using namespace jvgs::video;

using namespace jvgs::math;

#include "../tinyxml/tinyxml.h"

namespace jvgs
{
    namespace game
    {
        void Camera::loadData(TiXmlElement *element)
        {
			Vector2D::fromXML(element->FirstChildElement("position"), 
				&position);
            element->QueryFloatAttribute("maxdistance", &maxDistance);

            if(element->Attribute("target"))
                target = element->Attribute("target");

            Entity *entity = getLevel()->getEntityById(target);
            if(entity)
                setPosition(entity->getPosition());
        }

		Camera::Camera(const std::string &target, float maxDistance,
				const math::Vector2D &position, Level *level)
        {
			this->level = level;
			this->position = position;
            this->target = target;
            this->maxDistance = maxDistance;
        }

        Camera::Camera(TiXmlElement *element, Level *level)
        {
			this->level = level;
            load(element);
        }

        Camera::~Camera()
        {
        }

		Level *Camera::getLevel() const
        {
            return level;
        }

        void Camera::update(float ms)
        {
            Entity *entity = getLevel()->getEntityById(target);
            if(entity) {
                Vector2D position= getPosition();
                Vector2D direction = entity->getPosition() - position;
                if(direction.getLength() > maxDistance) {
                    direction.setLength((direction.getLength() - maxDistance));
                    setPosition(position + direction);
                }
            }
        }

        const Vector2D &Camera::getPosition() const
        {
            return position;
        }

        void Camera::setPosition(const Vector2D &position)
        {
            this->position = position;
        }

        void Camera::transform() const
        {
            VideoManager *videoManager = VideoManager::getInstance();
            videoManager->translate(-position + videoManager->getSize() * 0.5f);
        }

        BoundingBox& Camera::getBoundingBox()
        {
            VideoManager *videoManager = VideoManager::getInstance();
            boundingBox = BoundingBox(position - videoManager->getSize() * 0.5f,
                    position + videoManager->getSize() * 0.5f);
            return boundingBox;
        }
    }
}
