#ifndef JVGS_VIDEO_SKETCHYRENDERER_H
#define JVGS_VIDEO_SKETCHYRENDERER_H

#include "Renderer.h"

namespace jvgs
{
    namespace video
    {
        class SketchyRenderer: public Renderer
        {
            private:
                /** List of vectors to render. */
                std::vector<math::Vector2D> vectorList;

            public:
                /** Constructor.
                 */
                SketchyRenderer();

                /** Destructor.
                 */
                virtual ~SketchyRenderer();

                /* Override
                 */
                virtual void begin(RenderType type);

                /* Override
                 */
                virtual void end();

                /* Override
                 */
                virtual void vector(const math::Vector2D &vector);
        };
    }
}

#endif
