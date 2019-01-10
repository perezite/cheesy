#ifndef __PHYSMATH_H_
#define __PHYSMATH_H_

#include <math.h>
#include <vector>

using namespace std;

// ****************
// Typedefs
// ****************

typedef double real;
typedef signed long integer;
typedef real* range;

// ****************
// Class: Vector2D
// ****************

class Vector2D
{
public:
    Vector2D() {};
    Vector2D(real x, real y);

    Vector2D operator+ (Vector2D rhs);
    Vector2D operator- (Vector2D rhs);
    real     operator* (Vector2D rhs);         // Dot product
    Vector2D operator* (real rhs);             // Stretching
    Vector2D CrossP(Vector2D rhs);
    void Normalize();
    void Stretch(real length);

    void SetX(real x) {m_x = x;}
    void SetY(real y) {m_y = y;}

    real GetX() {return m_x;}
    real GetY() {return m_y;}
    integer GetXAsInteger() {return static_cast<integer>(m_x);}
    integer GetYAsInteger() {return static_cast<integer>(m_y);}
    real GetNorm();
private:
    real m_x, m_y;
};

#endif
