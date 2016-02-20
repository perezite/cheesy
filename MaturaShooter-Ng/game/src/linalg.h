
#include <math.h>

typedef struct
{
    double x, y;
}VECTOR2, *LPVECTOR2;


double Pi(void);

double dotP(VECTOR2 v1, VECTOR2 v2);

VECTOR2 ScalarMult(double a, VECTOR2 v);

VECTOR2 VecAdd(VECTOR2 v1, VECTOR2 v2);

float VectorLength(float v[2]);

float vabs(float v[2]);

float dotp(float v1[2], float v2[2]);

float angle(float v1[2], float v2[2]);

float angle_360_cw(float v[2]);

float angle_360_ccw(float v[2]);

void Rotate2D(float v[2], float alpha, float result[2], float s);

// Signum (Note: 0-Singularität wurde nicht beachtet)
float sgn(float a);
