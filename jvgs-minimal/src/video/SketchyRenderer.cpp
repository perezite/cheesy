#include "SketchyRenderer.h"

#include "../math/MathManager.h"
#include <iostream>
using namespace jvgs::math;

namespace jvgs
{
    namespace video
    {
        SketchyRenderer::SketchyRenderer()
        {
        }

        SketchyRenderer::~SketchyRenderer()
        {
        }

        void SketchyRenderer::begin(RenderType type)
        {
            Renderer::begin(type);
            vectorList.clear();
        }

        void SketchyRenderer::end()
        {
            Renderer::end();

			glBegin(rendering);
			for(int i = 0; i < (int)vectorList.size(); i++) {
				Vector2D vector2d = vectorList[i];
				glVertex2f(vector2d.getX(), vector2d.getY());
			}
			glEnd();

            vectorList.clear();
        }

        void SketchyRenderer::vector(const Vector2D &vector)
        {
            vectorList.push_back(vector);
        }
    };
};
