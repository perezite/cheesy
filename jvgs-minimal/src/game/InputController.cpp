#include "InputController.h"
#include "Entity.h"

#include "../input/InputManager.h"
using namespace jvgs::input;

#include "../tinyxml/tinyxml.h"

using namespace jvgs::math;

using namespace std;

namespace jvgs
{
    namespace game
    {
        void InputController::loadData(TiXmlElement *element)
        {
            element->QueryFloatAttribute("minjumpdelay", &minJumpDelay);
            element->QueryFloatAttribute("jumpforce", &jumpForce);
        }

        InputController::InputController(Entity *entity)
                : Controller(entity)
        {
			configuration = InputConfiguration::getConfiguration();
            inputManager = InputManager::getInstance();

            /* Some defaults. */
            minJumpDelay = 200.0f;
            jumpForce = 50.0f;
            jumpDelay = 0.0f;
        }

        InputController::InputController(Entity *entity,
                TiXmlElement *element): Controller(entity)
        {
			load(element);
            configuration = InputConfiguration::getConfiguration();
            inputManager = InputManager::getInstance();

            jumpDelay = 0.0f;
            load(element);
        }

        InputController::~InputController()
        {
        }

        void InputController::affect(float ms)
        {
            Entity *entity = getEntity();
            Vector2D velocity = entity->getVelocity();
            CollisionResponsePositioner *positioner = entity->getPositioner();

            /** In a jump. */
            if(jumpDelay > 0.0f)
                jumpDelay -= ms;

            /** Can start a jump. */
            if(positioner) {
                if(inputManager->isKeyDown(configuration->getKey("jump")) &&
                        positioner->canJump(ms) && jumpDelay <= 0.0f) {
                    jumpDelay = minJumpDelay;
                    velocity = positioner->getGravity() * -1.0f * jumpForce;
				}
            }

            if(inputManager->isKeyDown(configuration->getKey("left")))
                velocity.setX(-entity->getSpeed());
            else if(inputManager->isKeyDown(configuration->getKey("right")))
                velocity.setX(entity->getSpeed());
            else
                velocity.setX(0.0f);

			if(inputManager->isKeyTicked(configuration->getKey("action"))) {
				// do something
			}

            entity->setVelocity(velocity);
        }

        void InputController::setMinJumpDelay(float minJumpDelay)
        {
            this->minJumpDelay = minJumpDelay;
        }

        void InputController::setJumpForce(float jumpForce)
        {
            this->jumpForce = jumpForce;
        }
    }
}
