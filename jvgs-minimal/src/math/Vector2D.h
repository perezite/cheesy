#ifndef JVGS_MATH_VECTOR_H
#define JVGS_MATH_VECTOR_H

class TiXmlElement;

namespace jvgs
{
    namespace math
    {
        /** A 2D vector that represents a point in 2D
         *  space, or a distance, or...
         */
        class Vector2D
        {
            private:
                float x, y;

            public:
                /** Constructor.
                 *  @param x X value.
                 *  @param y Y value.
                 */
                Vector2D(float x=0, float y=0);

                /** Copy constructor.
                 *  @param other Object to copy.
                 */
                Vector2D(const Vector2D &other);

                /** Destructor.
                 */
                ~Vector2D();

                /** Get the X element.
                 *  @return The X element.
                 */
                float getX() const;

                /** Set the X element.
                 *  @param x The new X element.
                 */
                void setX(const float &x);

                /** Get the Y element.
                 *  @return The Y element.
                 */
                float getY() const;

                /** Set the Y element.
                 *  @param y The new Y element.
                 */
                void setY(const float &y);

                const Vector2D &operator=(const Vector2D &other);

                const Vector2D operator+(const Vector2D &other) const;
                const Vector2D &operator+=(const Vector2D &other);

                const Vector2D operator-(const Vector2D &other) const;
                const Vector2D &operator-=(const Vector2D &other);

                const Vector2D operator*(const float &scalar) const;
                const Vector2D &operator*=(const float &scalar);

                const Vector2D operator/(const float &scalar) const;
                const Vector2D &operator/=(const float &scalar);
				
				bool operator==(const Vector2D &other) const; 
				bool operator!=(const Vector2D &other) const; 				

                const Vector2D operator-() const;

                /** Dot product.
                 *  @param other Vector to perform the dot product with.
                 *  @return The dot product.
                 */
                float operator*(const Vector2D &other) const;

                /** Perp product.
                 *  @param other Vector to perform the perp product with.
                 *  @return The perp product.
                 */
                float perp(const Vector2D &other) const;

                /** Get this vector, but with the X element as 0.
                 *  @return This vector without X element.
                 */
                Vector2D onlyX() const;

                /** Get this vector, but with the Y element as 0.
                 *  @return This vector without Y element.
                 */
                Vector2D onlyY() const;

                /** Get the squared length of this vector.
                 *  @return The squared length of this vector.
                 */
                float getSquaredLength() const;

                /** Get the length of this vector.
                 *  @return The length of this vector.
                 */
                float getLength() const;

                /** Set the length of this vector.
                 *  @param length The new length for this vector.
                 */
                void setLength(float length);

                /** Get the distance from this vector to another.
                 *  @param other Vector to get the distance to.
                 *  @return The distance to other.
                 */
                float getDistance(const Vector2D &other) const;

                /** Return a normalized version of this vector.
                 *  @return a normalized version of this vector.
                 */
                Vector2D normalized() const;

                /** Return a vector with the X and Y elements inverted.
                 *  That is: (x, y) -> (1/x, 1/y).
                 *  @return A vector with inverted elements.
                 */
                Vector2D inverted() const;

                /** Reflects the other vector around this vector and
                 *  return the result.
                 *  @param other Vector to be reflected.
                 *  @return The reflection of the parameter around this vector.
                 */
                Vector2D reflect(const Vector2D &other) const;

                /** Flip the y coordinate of the vector
                 *  @return The vector with flipped y coordinate
                 */
				Vector2D Vector2D::flipY(void);

                /** Get the angle to another vector.
                 *  @param other Vector to get the angle to.
                 *  @return The requested angle in degrees.
                 */
                 float getAngle(const Vector2D &other) const;

                /** Load a vector from xml data..
                 *  @param element TiXmlElement to load data from. Can be 0.
                 *  @param vector Vector2D to fill in.
                 */
                static void fromXML(TiXmlElement *element, Vector2D *vector);

                /** Create a vector from polar coordinates.
                 *  @param radius Radius.
                 *  @param theta Angle in degrees.
                 *  @param The vector in cartesian coordinates.
                 */
                static Vector2D fromPolar(float radius, float theta);
        };
    };
};

#endif
