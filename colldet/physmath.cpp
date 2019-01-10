
#include "physmath.hpp"


Vector2D::Vector2D(real x, real y)
{
    m_x = x;
    m_y = y;
}

Vector2D Vector2D::operator+ (Vector2D rhs)
{
    real newx = m_x + rhs.GetX();
    real newy = m_y + rhs.GetY();
    Vector2D temp(newx, newy);
    return temp;
}

Vector2D Vector2D::operator- (Vector2D rhs)
{
    real newx = m_x - rhs.GetX();
    real newy = m_y - rhs.GetY();
    Vector2D temp(newx, newy);
    return temp;
}

real Vector2D::operator* (Vector2D rhs)           // Dot product
{
    return (m_x * rhs.GetX() + m_y * rhs.GetY());
}

Vector2D Vector2D::operator* (real rhs)                     // Stretching
{
   real newx = m_x * rhs;
   real newy = m_y * rhs;
   Vector2D temp(newx, newy);
   return temp;
}

Vector2D CrossP(Vector2D rhs)
{
   Vector2D dummy;
   return dummy;
}

real Vector2D::GetNorm()
{
    return sqrt(m_x*m_x + m_y*m_y);
}

void Vector2D::Normalize()
{
    real norm = GetNorm();
    m_x /= norm;
    m_y /= norm;
}

void Vector2D::Stretch(real length)
{
    real norm = GetNorm();
    m_x = (m_x/norm)*length;
    m_y = (m_y/norm)*length;
}

