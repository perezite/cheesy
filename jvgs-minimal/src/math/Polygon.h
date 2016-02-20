#ifndef JVGS_MATH_POLYGON_H
#define JVGS_MATH_POLYGON_H

#include "Vector2d.h"
#include "BoundedObject.h"
#include "BoundingBox.h"

#include <vector>
using namespace std;

namespace jvgs 
{
	namespace math 
	{
		class Polygon : BoundedObject {
		private: 
			/** Polygon points. */
			std::vector<Vector2D> points;

			/** Bounding box. */
			math::BoundingBox boundingBox;

		public: 
			/** Constructor.
             *  @param filename The file containing the polygon coordinates
             */
			Polygon(std::string &filename);

			/** Constructor creating an empty polygon
             */
			Polygon(void);

			/** Add a point to the polygon.
			 *  @param point The point to add.
			 */
			void insertPoint(const Vector2D &point);
			
			/** Get the number of points in the polygon
			 *  @return The number of points in the polygon
			 */
			unsigned int getNumPoints() const;

			/** Get i'th point of the polygon
			 *  @param i The index of the point
			 *  @return The i'th point of the polygon
			 */ 
			const Vector2D& getPoint(int i) const; 

		    /* Override
             */
            virtual BoundingBox& getBoundingBox();

		    /** Translate the polygon by the given vector v
			 *  @param v The translation vector
             */
			void translate(Vector2D v);

			const math::Polygon& Polygon::operator=
							(const math::Polygon &other);
		
		};
	}
}

#endif