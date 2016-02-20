#ifndef JVGS_MATH_LINE_SEGMENT_H
#define JVGS_MATH_LINE_SEGMENT_H

#include "Vector2d.h"

namespace jvgs 
{
	namespace math 
	{
		class LineSegment
		{
			private:
				math::Vector2D start;
				math::Vector2D end;
			
			public:
				LineSegment::LineSegment(
								const Vector2D& _start=Vector2D(0.0f, 0.0f), 
								const Vector2D& _end=Vector2D(0.0f, 0.0f)) :
					start(_start), end(_end)
				{
				}

				const math::Vector2D getStart() const 
				{
					return start;
				}

				const math::Vector2D getEnd() const 
				{
					return end;
				}

				void setStart(const Vector2D _start) 
				{
					start = _start;
				}

				void setEnd(const Vector2D _end) 
				{
					end = _end;
				}
		};
	}
}

#endif