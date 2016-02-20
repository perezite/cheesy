
#include "Polygon.h"

#include "../core/LogManager.h"
using namespace jvgs::core;

#include <algorithm>
#include <fstream>

namespace jvgs 
{
	namespace math 
	{				
		Polygon::Polygon(std::string &filename) {
			std::ifstream pointsFile(filename.c_str());
			if (pointsFile.is_open()) {
				while(pointsFile.good()) {
					char buf[128];
					pointsFile.getline(buf, 128);
					std::string line(buf);
					unsigned int sep = line.find(" ");
					if (sep != std::string::npos && sep < (line.size()-1)) {
						float x = (float)atoi(line.substr(0, sep).c_str());
						float y = (float)atoi(line.substr(sep+1, std::string::npos).c_str());
						insertPoint(Vector2D(x,y));
					}
				}
				pointsFile.close();
			} else {
				LogManager::getInstance()->error("could not open point file %s", filename.c_str());
			}
		}

		Polygon::Polygon(void) {
		}

		void Polygon::insertPoint(const Vector2D &point) 
		{
			points.push_back(point);
		}

		unsigned int Polygon::getNumPoints() const
		{
			return points.size();
		}

		const Vector2D& Polygon::getPoint(int i) const
		{
			return points.at(i);
		}

		bool compX(Vector2D i, Vector2D j) 
		{
			return i.getX() < j.getX();
		}

		bool compY(Vector2D i, Vector2D j) 
		{
			return i.getY() < j.getY();
		}

		BoundingBox &Polygon::getBoundingBox() {
			float minX = 0, minY = 0, maxX = 0, maxY = 0;
			if (points.empty() == false) {
				minX = std::min_element(points.begin(), 
					points.end(), compX)->getX();
				minY = std::min_element(points.begin(), 
					points.end(), compY)->getY();
				maxX = std::max_element(points.begin(), 
					points.end(), compX)->getX();
				maxY = std::max_element(points.begin(), 
					points.end(), compY)->getY();
			}
			
			boundingBox = BoundingBox(Vector2D(minX, minY), 
							Vector2D(maxX, maxY));

			return boundingBox;
		}

		void Polygon::translate(Vector2D v) {
			for (int i = 0; i < (int)points.size(); i++) {
				points[i] += v;
			}
		}

		const math::Polygon& Polygon::operator=(const math::Polygon &other) {
			points.clear();
			for (int i = 0; i < (int)other.getNumPoints(); i++)
			{
				insertPoint(other.getPoint(i));
			}

			return *this;
		}
	}
}