#ifndef JVGS_GAME_CAMERA_H
#define JVGS_GAME_CAMERA_H

#include "../math/Vector2D.h"
#include "../math/BoundingBox.h"
#include "../math/BoundedObject.h"
#include "../core/XMLLoadable.h"
#include <string>

namespace jvgs
{
    namespace game
    {
        class Entity;
        class Level;

		class Camera : public math::BoundedObject, public core::XMLLoadable 
        {
			private:
			    /** Level the camera is in. */
                Level *level;

				/** Camera position. */
                math::Vector2D position;

                /** Bounding box. */
                math::BoundingBox boundingBox;

                /** Id of object to follow. */
                std::string target;

                /** Max distance between camera focus and object. */
                float maxDistance;

            protected:
                /* Override
                 */
                void loadData(TiXmlElement *element);

            public:
                /** Constructor.
                 *  @param target Id of target to follow.
                 *  @param maxDistance Max distance between focus and object.
                 *  @param level Level the camera is used for.
                 */
                Camera(const std::string &target, float maxDistance,
                        const math::Vector2D &position, Level *level);

                /** Constructor.
                 *  @param element XML element to load data from.
                 *  @param level Level the camera is used for.
                 */
                Camera(TiXmlElement *element, Level *level);

                /** Destructor.
                 */
                virtual ~Camera();

				/** Get the level the camera is used for.
                 *  @return The camera level.
                 */
                virtual Level *getLevel() const;

                /** Update the camera
				 *  @param ms Milliseconds since last update
                 */
                virtual void update(float ms);

               /** Get the camera position.
                 *  @return The camera position.
                 */
                virtual const math::Vector2D &getPosition() const;

                /** Set the camera position.
                 *  @param position The new camera position.
                 */
                virtual void setPosition(const math::Vector2D &position);

                /* Override
                 */
                virtual void transform() const;

                /* Override
                 */
                virtual math::BoundingBox& getBoundingBox();
        };
    }
}

#endif
