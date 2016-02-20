#ifndef JVGS_GAME_INPUTCONTROLLER_H
#define JVGS_GAME_INPUTCONTROLLER_H

#include "../input/InputConfiguration.h"
#include "../input/InputManager.h"
#include "Controller.h"

#include "../tinyxml/tinyxml.h"

namespace jvgs
{
    namespace game
    {
		class InputController : public Controller
        {
            private:
                /** Reference to the input configuration. */
                input::InputConfiguration *configuration;

                /** Reference to the input manager. */
                input::InputManager *inputManager;

                /** Minimum delay between jumps. */
                float minJumpDelay;

                /** Force to jump with. */
                float jumpForce;

                /** Delay to next jump. */
                float jumpDelay;

            protected:
                /* Override
                 */
                virtual void loadData(TiXmlElement *element);

            public:
                /** Constructor.
                 *  @param entity Entity to control.
                 */
                InputController(Entity *entity);

                /** Constructor.
                 *  @param entity Entity to control.
                 *  @param element TiXmlElement to load data from.
                 */
                InputController(Entity *entity, TiXmlElement *element);

                /** Destructor.
                 */
                virtual ~InputController();

                /* Override.
                 */
                virtual void affect(float ms);

                /** Set the minimum jump delay.
                 *  @param The minimum jump delay.
                 */
                void setMinJumpDelay(float minJumpDelay);

                /** Set the jump force.
                 *  @param jumpForce Force to jump with.
                 */
                void setJumpForce(float jumpForce);
        };
    }
}

#endif
