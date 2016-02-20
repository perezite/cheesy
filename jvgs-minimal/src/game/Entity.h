#ifndef JVGS_GAME_ENTITY_H
#define JVGS_GAME_ENTITY_H

#include "Level.h"
#include "CollisionResponsePositioner.h"
#include "Controller.h"
#include "../math/BoundingBox.h"
#include "../math/BoundedObject.h"
#include "../math/Vector2D.h"
#include "../math/Polygon.h"
#include "../core/XMLLoadable.h"
#include "../core/PropertyMap.h"
#include "../core/View.h"
#include "../sketch/Sketch.h"
#include <map>
#include <string>

class TiXmlElement;

namespace jvgs
{
    namespace game
    {
        //class Controller;
        class Sprite;

        class Entity : public core::View, public math::BoundedObject,
                public core::PropertyMap
        {
            private:
               /** Entity id. */
                std::string id;

                /** Level the entity is in. */
                Level *level;

                /** The entity position. */
                math::Vector2D position;

                /** The entity velocity. */
                math::Vector2D velocity;

                /** Speed of the entity. */
                float speed;

                /** Radius used for collision detection. */
                math::Vector2D radius;

                /** If the entity actively checks for collision with other
                 *  entities. */
                bool collisionChecker;

                /** Entity controller. */
                Controller *controller;

                /** Entity positioner. */
                CollisionResponsePositioner *positioner;

                /** Event script. */
                std::string script;

                /** For time events. */
                float timer;

                /** Bounding box. */
                math::BoundingBox boundingBox;

				/** Sketch of the entity */
				//sketch::Sketch* sketch;

				/** Shape polygon */
				math::Polygon *shapePolygon;

				/** World polygon (shape polygon in world coordinates) */
				math::Polygon worldPolygon;

            protected:
                /* Override
                 */
                void loadData(TiXmlElement *element);

            public:
                /** Constructor.
                 *  @param element TiXmlElement to load entity from.
                 *  @param level Level the entity is in.
                 */
                Entity(TiXmlElement *element, Level *level);

                /** Destructor.
                 */
                virtual ~Entity();

                /** Get a controller instance
				 *  @param type The type of the newly created controller
				 *  @param controllerElement The XML data element
                 *  @return The controller instance
                 */
				Controller *getControllerInstance(
					const std::string &type, TiXmlElement *controllerElement);

                /** Get a positioner instance
				 *  @param type The type of the newly created positioner
				 *  @param positionerElement The XML data element
                 *  @return The positioner instance
                 */
				CollisionResponsePositioner *getPositionerInstance(
					const std::string &type, TiXmlElement *positionerElement);


                /** Get the entity id.
                 *  @return The id for this entity.
                 */
                virtual const std::string &getId() const;

                /** Set the entity id. Use with care.
                 *  @param id The new id.
                 */
                virtual void setId(const std::string &id);

	            /** Get the entity level.
                *  @return The level the entity is in.
                */
				virtual Level *getLevel() const;

                /** Get the entity position.
                 *  @return The entity position.
                 */
                virtual const math::Vector2D &getPosition() const;

                /** Set the entity position.
                 *  @param position The new entity position.
                 */
                virtual void setPosition(const math::Vector2D &position);

                /** Get the entity velocity.
                 *  @return The entity velocity.
                 */
                virtual const math::Vector2D &getVelocity() const;

                /** Set the entity velocity.
                 *  @param velocity The new entity velocity.
                 */
                virtual void setVelocity(const math::Vector2D &velocity);

                /** Get the speed for this entity.
                 *  @return The speed for this entity.
                 */
                virtual float getSpeed() const;

                /** Set the speed for this entity.
                 *  @param speed The new speed for this entity.
                 */
                virtual void setSpeed(float speed);

                /** Get the radius used for collision detection.
                 *  @return The radius used for collision detection.
                 */
                virtual const math::Vector2D &getRadius() const;

                /** Set the radius used for collision detection.
                 *  @param radius The new radius.
                 */
                virtual void setRadius(const math::Vector2D &radius);

                /** See if this object actively checks for collisions.
                 *  @return If this object actively checks for collisions.
                 */
                virtual bool isCollisionChecker() const;

                /** Set whether this object actively checks for collisions.
                 *  @param collisionChecker If this object checks collisions.
                 */
                 virtual void setCollisionChecker(bool collisionChecker);

                /** Set the controller of this entity.
                 *  @param controller The controller of this entity.
                 */
#               ifdef SWIG
                    %apply SWIGTYPE *DISOWN {Controller* controller};
#               endif
                virtual void setController(Controller *controller);

                /** Get the controller of this entity.
                 *  @return The controller of this entity.
                 */
                virtual Controller *getController() const;

                /** Set the positioner of this entity.
                 *  @param positioner The positioner of this entity.
                 */
#               ifdef SWIG
                    %apply SWIGTYPE *DISOWN {CollisionResponsePositioner* positioner};
#               endif
                virtual void setPositioner(CollisionResponsePositioner *positioner);

                /** Get the positioner of this entity.
                 *  @return The positioner of this entity.
                 */
                virtual CollisionResponsePositioner *getPositioner() const;

                /** Set the event script.
                 *  @param script The event script.
                 */
                virtual void setScript(const std::string &script);

                /** Get the event script.
                 *  @return The event script.
                 */
                virtual const std::string &getScript() const;

                /** Get the timer time.
                 *  @return The timer time.
                 */
                virtual float getTimer() const;

                /** Set the timer event time.
                 *  @param timer The new timer event time.
                 */
                virtual void setTimer(float timer);

                /** Get a bounding box for the entity.
                 *  @return A bounding box for the entity.
                 */
                virtual math::BoundingBox& getBoundingBox();

                /** Get a bounding polygon for the entity.
                 *  @return A bounding polygon for the entity.
                 */
				virtual math::Polygon *getWorldPolygon();

                /* Override
                 */
                virtual void update(float ms);

                /* Override
                 */
                virtual void render();

                /* Override
                 */
                virtual void set(const std::string &key,
                        const std::string &value);

                /* Override
                 */
                virtual void setBool(const std::string &key, bool value);

                /* Override
                 */
                virtual void setGarbage();
        };
    }
}

#endif
