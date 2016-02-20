#ifndef JVGS_MATH_COLLISIONDETECTOR_H
#define JVGS_MATH_COLLISIONDETECTOR_H

#include "../sketch/Sketch.h"
#include "Vector2D.h"
#include <vector>

namespace jvgs
{
    namespace math
    {
        class BoundedObject;
        class MathManager;

        class CollisionDetector
        {
			private: 
				static const double SMALL_NUM;
            private:
                /** The objects to collide with. */
                std::vector<BoundedObject*> objects;

                /** MathManager to perform calculations. */
                MathManager *mathManager;

				/** Points to collide with*/
				//std::vector<math::Vector2D> *points;
				//Polygon *points;
				sketch::Sketch *sketch;

            public:
                /** Constructor.
                 *  @param sketch Sketch with lines to collide against.
                 */
                CollisionDetector(sketch::Sketch *_sketch);

                /** Destructor.
                 */
                virtual ~CollisionDetector();

				/** Check for collision of polygon shape with world
				*  @param polygon The bounding polygon to check against.
				*		The polygon must have counter-clockwise winding
				*  @return If there is a collision.
				*/
				bool isColliding(math::Polygon *polygon);

				/** Non-continuous collision check of drifting polygon
				*  @param boundingPolygon The bounding polygon to check against.
				*		The polygon must have counter-clockwise winding
				*  @param velocity Velocity of the shape
				*  @param ms Milliseconds in the future to check for collisions
				*  @return If there is a collision after exactly ms milliseconds
				*/
				bool willCollide(math::Polygon *shape, const Vector2D &velocity,
					float ms);

            protected:
				/** Polygon-Box intersection test
				*  Taken from http://geomalgorithms.com/a13-_intersect-4.html#intersect2D_SegPoly%28%29
				*  @param p0 Start point of line
				*  @param p1 End point of line (must no be equal to p0)
				*  @param V[] Array of n vertex points in clockwise ordering
				*  @param n Number of 2D points in the polygon
				*  @param *is0 Start point of intersection segment
				*  @param *is1 End point of intersection segment
				*  @return If S and V intersect
				*/
				bool lineIntersectsPolygon(math::Vector2D p0, math::Vector2D p1, 
					math::Vector2D *V, int n, math::Vector2D& is0, math::Vector2D& is1);
		};
    }
}

#endif
