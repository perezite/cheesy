#include "Level.h"
#include "LevelManager.h"
#include "Entity.h"
#include "Camera.h"

#include "../core/LogManager.h"
using namespace jvgs::core;

#include "../video/VideoManager.h"
using namespace jvgs::video;

#include "../tinyxml/tinyxml.h"

using namespace jvgs::sketch;
using namespace jvgs::math;

#include <iostream>
using namespace std;

namespace jvgs
{
    namespace game
    {
        void Level::loadData(TiXmlElement *element)
        {
            /* Play some music. */
            if(element->Attribute("music")) {
                string music = element->Attribute("music");
            }

            /* Load the world. */
            if(element->Attribute("world")) {
                if(world)
                    delete world;

                string worldFileName = element->Attribute("world");
                world = new Sketch(worldFileName);
                collisionDetector = new CollisionDetector(world);
                boundingBox = BoundingBox(Vector2D(0.0f, 0.0f),
                        world->getSize());
            } else {
                LogManager::getInstance()->warning(
                        "No world attribute specified in the level xml.");
                world = 0;
            }

            /* Walk through the file, adding entities. */
            TiXmlElement *entityElement = element->FirstChildElement("entity");
            while(entityElement) {
                Entity *entity = new Entity(entityElement, this);
                addEntity(entity);
                entityElement = entityElement->NextSiblingElement("entity");
            }

            /* Add the camera. */
            TiXmlElement *cameraElement = element->FirstChildElement("camera");
            if(cameraElement) {
                if(!cameraElement->Attribute("type"))
                    LogManager::getInstance()->error(
                            "Camera element always needs a type attribute.");

                if(camera)
                    delete camera;

                string type = cameraElement->Attribute("type");
				camera = new Camera(cameraElement, this);
            } else {
                camera = 0;
            }
        }

        Level::Level()
        {
            world = 0;
            collisionDetector = 0;
            camera = 0;
        }

        Level::Level(TiXmlElement *element)
        {
            world = 0;
            collisionDetector = 0;
            camera = 0;
            load(element);
        }

        Level::Level(const string &fileName)
        {
            world = 0;
            collisionDetector = 0;
            camera = 0;
            load(fileName);
        }

        Level::~Level()
        {
            if(world)
                delete world;
            if(collisionDetector)
                delete collisionDetector;
            for(vector<Entity*>::iterator iterator = entities.begin();
                    iterator != entities.end(); iterator++)
                delete (*iterator);
            if(camera)
                delete camera;
        }

        Sketch *Level::getWorld() const
        {
            return world;
        }

        CollisionDetector *Level::getCollisionDetector() const
        {
            return collisionDetector;
        }

        int Level::getNumberOfEntities() const
        {
            return (int) entities.size();
        }

        Entity *Level::getEntity(int index) const
        {
            return entities[index];
        }

        void Level::addEntity(Entity *entity)
        {
            /* Guard from id duplicates. */
            if(getEntityById(entity->getId()))
                LogManager::getInstance()->warning("Duplicate entity id: %s",
                        entity->getId().c_str());

            entities.push_back(entity);
            entitiesById[entity->getId()] = entity;
        }

        Entity *Level::getEntityById(const string &id)
        {
            map<string, Entity*>::iterator result = entitiesById.find(id);
            if(result != entitiesById.end())
                return result->second;
            else
                return 0;
        }

        void Level::update(float ms)
        {
            if(camera)
                camera->update(ms);

            for(vector<Entity*>::iterator iterator = entities.begin();
                    iterator != entities.end(); iterator++) {
                Entity *entity = *iterator;

                entity->update(ms);

                /* When below the level, they become garbage. */
                if(entity->getPosition().getY() - entity->getRadius().getY() >
                        world->getSize().getY())
                    entity->setGarbage();
            }

			/* queue current level if player became garbage */
			for(vector<Entity*>::iterator iterator = entities.begin();
				iterator != entities.end(); iterator++) {
				Entity *entity = (*iterator);
				if (entity->getId() == "player" && entity->isGarbage()) {
					//LevelManager::getInstance()->getLevel()->
					LevelManager::getInstance()->queueLevel("resources/level-simple/level.xml");
				}
			}

            /* Remove garbage. */
            vector<Entity*> originalEntities = entities;
            entities.clear();
            for(vector<Entity*>::iterator iterator = originalEntities.begin();
                    iterator != originalEntities.end(); iterator++) {
                if(!(*iterator)->isGarbage()) {
                    entities.push_back(*iterator);
                } else {
                    entitiesById.erase((*iterator)->getId());
                    delete (*iterator);
                }
            }
        }

        void Level::render()
        {
            BoundingBox *cameraBoundingBox;
            if(camera) {
                camera->transform();
                cameraBoundingBox = &camera->getBoundingBox();
            }

            if(world)
                world->render();

            Entity *entity;
            for(vector<Entity*>::iterator iterator = entities.begin();
                    iterator != entities.end(); iterator++) {
                entity = *iterator;
                if(camera) {
                    /* Try not too render all entities by using a bounding box
                     * test. */
                    if(entity->getBoundingBox().intersectsWith(
                            cameraBoundingBox))
                        entity->render();
                /* No camera, so we could be anywhere. Render anyway. */
                } else {
                    entity->render();
                }
            }
        }
    }
}
