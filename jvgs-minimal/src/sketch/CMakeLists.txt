ADD_LIBRARY( 
    sketch STATIC 
    Sketch.cpp
    SketchManager.cpp
)

TARGET_LINK_LIBRARIES(
    sketch
    tinyxml
    core
    video
)
