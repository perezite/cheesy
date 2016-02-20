#include "CollisionDetector.h"
#include "BoundingBox.h"
#include "MathManager.h"

using namespace jvgs::sketch;

#include <math.h>
#include <iostream>
using namespace std;

namespace jvgs
{
    namespace math
    {
		const double CollisionDetector::SMALL_NUM = 0.00000001f;

        CollisionDetector::CollisionDetector(Sketch *_sketch)
		{
			sketch = _sketch;
        }
        
        CollisionDetector::~CollisionDetector()
        {
			
        }

		bool CollisionDetector::isColliding(math::Polygon *polygon) 
		{
			bool collision = false;
			if(polygon->getNumPoints() <= 0) {
				return false;	
			}

			/* flip the y-component of all coordinates, 
			   since the intersection tests uses an upwards y-axis */
			Vector2D *polygonPoints = new Vector2D[polygon->getNumPoints()];
			for (int i = 0; i < (int)polygon->getNumPoints(); i++) {
				polygonPoints[i] = polygon->getPoint(i);
				polygonPoints[i].flipY();
			}

			/* now, do the intersection test */
			int nSegments = sketch->getNumSegments();
			for (int i = 0; i < nSegments; i++) {
				Vector2D p0 = sketch->getSegment(i)->getStart();
				p0.flipY();
				Vector2D p1 = sketch->getSegment(i)->getEnd();
				p1.flipY();

				Vector2D intersectionStart;
				Vector2D intersectionEnd;
				bool res = lineIntersectsPolygon(p0, p1, polygonPoints, 
									polygon->getNumPoints(), intersectionStart,
									intersectionEnd);
				if (res == true) 
					collision = true;
			}

			delete [] polygonPoints;
			return collision;

			return false;

		}

		bool CollisionDetector::willCollide(math::Polygon *shape, 
						const Vector2D &velocity, float ms) 
		{
			Polygon newShape = *shape;
			newShape.translate(velocity*ms);
			return isColliding(&newShape);
		}


		bool CollisionDetector::lineIntersectsPolygon(math::Vector2D p0, math::Vector2D p1, 
			math::Vector2D *V, int n, math::Vector2D& is0, math::Vector2D& is1)
		{
			float  tE = 0;              // the maximum entering segment parameter
			float  tL = 1;              // the minimum leaving segment parameter
			float  t, N, D;             // intersect parameter t = N / D
			math::Vector2D dS = p1-p0;  // the  segment direction vector
			math::Vector2D e;           // edge vector
			// Vector ne;               // edge outward normal (not explicit in code)

			for (int i=0; i < n; i++)		// process polygon edge V[i]V[i+1]
			{
				e = V[(i+1)%n] - V[i];
				N = e.perp(p0-V[i]);		// = -dot(ne, S.P0 - V[i])
				D = -e.perp(dS);			// = dot(ne, dS)
				if (fabs(D) < SMALL_NUM) {  // S is nearly parallel to this edge
					if (N < 0)				// P0 is outside this edge, so
						return false;		// S is outside the polygon
					else                    // S cannot cross this edge, so
						continue;			// ignore this edge
				}

				t = N / D;
				if (D < 0) {            // segment S is entering across this edge
					if (t > tE) {       // new max tE
						tE = t;
						if (tE > tL)	// S enters after leaving polygon
							return false;
					}
				}
				else {                  // segment S is leaving across this edge
					if (t < tL) {       // new min tL
						tL = t;
						if (tL < tE)	// S leaves before entering polygon
							return false;
					}
				}
			}

			// tE <= tL implies that there is a valid intersection subsegment
			is0 = p0 + math::Vector2D(tE*dS.getX(),tE*dS.getY());	// = P(tE) = point where S enters polygon
			is1 = p0 + math::Vector2D(tL*dS.getX(),tL*dS.getY());	// = P(tL) = point where S leaves polygon
			return true;
			
		}
    }
}
