ADD_LIBRARY( 
    math STATIC 
    AffineTransformationMatrix.cpp
    BoundedObject.cpp
    BoundingBox.cpp
    CollisionDetector.cpp
    LineSegment.cpp
    MathManager.cpp
    Matrix.cpp
    Vector2D.cpp
)

TARGET_LINK_LIBRARIES(
    math sketch
)
