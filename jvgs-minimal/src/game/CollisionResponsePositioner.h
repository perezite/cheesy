#ifndef JVGS_GAME_COLLISIONRESPONSEPOSITIONER_H
#define JVGS_GAME_COLLISIONRESPONSEPOSITIONER_H

#include "../sketch/Sketch.h"
#include "../core/XMLLoadable.h"
#include "../math/BoundedObject.h"
#include "../math/Vector2D.h"
#include "../math/MathManager.h"
#include "Affector.h"
#include <vector>

class TiXmlElement;

namespace jvgs
{
    namespace game
    {
        class Entity;

		class CollisionResponsePositioner : public Affector
        {
            private:
				const static float VERY_CLOSE;
				const static int MAX_STEPS;
				const static float SLIP_LIMIT;

                /** Gravity pulling the entity. */
                math::Vector2D gravity;

                /** MathManager to perform calculations. */
                math::MathManager *mathManager;

                /** Max distance from collision if you want to jump. */
                float jumpDistanceLimit;

            protected:
                /* Override
                 */
                void loadData(TiXmlElement *element);

            public:
                /** Constructor.
                 *  @param entity Entity to respond to collisions.
                 */
                CollisionResponsePositioner(Entity *entity);

                /** Constructor.
                 *  @param entity Entity to respond to collisions.
                 *  @param element TiXmlElement containing the data.
                 */
                CollisionResponsePositioner(Entity *entity,
                        TiXmlElement *element);

                /** Destructor.
                 */
                virtual ~CollisionResponsePositioner();

                /** Get the gravity applied by the positioner.
                 *  @return The gravity.
                 */
                virtual const math::Vector2D &getGravity() const;

                /** Set the gravity.
                 *  @param gravity The new gravity.
                 */
                virtual void setGravity(const math::Vector2D &gravity);

                /* Override
                 */
                virtual void affect(float ms);

                /* Override
                 */
                virtual bool canJump(float ms);


        };
    }
}

#endif
