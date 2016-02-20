INCLUDE_DIRECTORIES(
    ${SDL_INCLUDE_DIR}
)

ADD_LIBRARY( 
    game STATIC 
    Affector.cpp
    Camera.cpp
    CollisionResponsePositioner.cpp
    Controller.cpp
    Entity.cpp
    InputController.cpp
    Positioner.cpp
    Level.cpp
    LevelManager.cpp
)

TARGET_LINK_LIBRARIES(
    game input
)
