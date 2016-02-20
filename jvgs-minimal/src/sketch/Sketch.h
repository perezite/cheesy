#ifndef JVGS_SKETCH_SKETCH_H
#define JVGS_SKETCH_SKETCH_H

#include <string>
#include <vector>
#include <map>
#include "../math/Vector2D.h"
#include "../video/Renderer.h"
#include "../math/BoundingBox.h"
#include "../math/Polygon.h"
#include "../math/LineSegment.h"

namespace jvgs
{
    namespace sketch
    {
        class SketchElement;

        class Sketch
        {
            private:
                /** Name of the file where this sketch was loaded from. */
                std::string fileName;

                /** Size of the sketch in pixels. */
                math::Vector2D size;

				/** Line segments of the sketch */
				std::vector<math::LineSegment> segments;

            public:
                /** Constructor.
                 *  @param fileName Points file to parse.
                 */
                Sketch(const std::string &fileName);

                /** Constructor. Creates a sketch of the given size.
                 *  @param size Size of the sketch.
                 */
                Sketch(const math::Vector2D &size);


                /** Destructor.
                 */
                virtual ~Sketch();

                /** Get the filename this sketch was loaded from.
                 *  @return The filename this sketch was loaded from.
                 */
                virtual const std::string &getFileName() const;

                /** Set the size of this sketch in pixels.
                 *  @param size The new size.
                 */
                virtual void setSize(const math::Vector2D &size);

                /** Get the size from this sketch in pixels.
                 *  @return The sketch size in pixels.
                 */
                virtual const math::Vector2D &getSize() const;
				
				/** Get the number of line segments in the sketch
				 *  @return The number of line segments
				 */
				unsigned int getNumSegments() const { return segments.size();}

                /** Get a line segment
				 *  @param i The index of the line segment
                 *  @return The line segment
                 */				
				math::LineSegment *getSegment(unsigned int i) { return &segments.at(i); }

                /** Render the sketch
                 */
                virtual void render();

        };
    }
}

#endif
