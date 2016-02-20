#include "Sketch.h"
#include "Parser.h"
#include "../video/SketchyRenderer.h"
#include <iostream>
#include <fstream>

#include "../core/LogManager.h"
using namespace jvgs::core;

#include "../video/Renderer.h"
#include "../video/VideoManager.h"
using namespace jvgs::video;

#include "../math/LineSegment.h"
using namespace jvgs::math;

using namespace std;

namespace jvgs
{
    namespace sketch
    {
        Sketch::Sketch(const string &fileName)
        {
            this->fileName = fileName;
            size = Vector2D(2000.0f, 2000.0f);

			/* Parse points file */
			std::ifstream pointsFile(fileName.c_str());
			if (pointsFile.is_open()) {
				Vector2D point0;
				Vector2D point1;
				unsigned int lineNumber = 0;
				while(pointsFile.good()) {
					char buf[128];
					pointsFile.getline(buf, 128);
					std::string line(buf);
					unsigned int sep = line.find(" ");
					if (sep != std::string::npos && sep < (line.size()-1)) {
						float x = (float)atoi(line.substr(0, sep).c_str());
						float y = (float)atoi(line.substr(sep+1, std::string::npos).c_str());
						if (lineNumber%2 == 0) {
							point0 = Vector2D(x,y);
						} else {
							point1 = Vector2D(x,y);
							LineSegment segment(point0, point1);
							segments.push_back(segment);
						}
						lineNumber++;	
					}
				}
				pointsFile.close();
			} else {
				LogManager::getInstance()->error("could not load sketch points file %s", 
					fileName.c_str());
			}
        }

        Sketch::Sketch(const Vector2D &size)
        {
            this->fileName = "not loaded from file";
            this->size = size;
        }

        Sketch::~Sketch()
        {
        }

        const string &Sketch::getFileName() const
        {
            return fileName;
        }

        void Sketch::setSize(const Vector2D &size)
        {
            this->size = size;
        }

        const Vector2D &Sketch::getSize() const
        {
            return size;
        }

        void Sketch::render()
        {
			VideoManager *videoManager = VideoManager::getInstance();
			SketchyRenderer sketchyRenderer;
			sketchyRenderer.begin(RENDERTYPE_LINES);
			for (unsigned int i = 0; i < segments.size(); i++) {
				sketchyRenderer.vector(segments[i].getStart());
				sketchyRenderer.vector(segments[i].getEnd());
			}
			sketchyRenderer.end();
        }
    }
}
