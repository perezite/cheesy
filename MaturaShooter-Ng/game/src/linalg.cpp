#include <math.h>
#include <stdlib.h>
#include "linalg.h"

double Pi(void)
{
    return 3.141593;
}

double dotP(VECTOR2 v1, VECTOR2 v2)
{
    return v1.x*v2.x + v1.y*v2.y;
}

VECTOR2 ScalarMult(double a, VECTOR2 v)
{
    VECTOR2 res;
    res.x = v.x*a;
    res.y = v.y*a;
    return res;
}
/*
float* ScalarMult(double a, float v[2])
{
    float res[2];
    res[0] = v[0]*a;
    res[1] = v[1]*a;
    return res;
}*/

float* ScalarMult(double a, float v[2])
{
    float *res = new float[2];
    res[0] = v[0]*a;
    res[1] = v[1]*a;
    return res;
}

VECTOR2 VecAdd(VECTOR2 v1, VECTOR2 v2)
{
    VECTOR2 res;
    res.x = v1.x+v2.x;
    res.y = v1.y+v2.y;
    return res;
}

//-----------Berechnet den Betrag eines Vektors---------------------------------------------------
float VectorLength(float v[2])
{
    return sqrt(pow(v[0],2)+pow(v[1],2));
}

float vabs(float v[2])
{
    return sqrt(pow(v[0],2)+pow(v[1],2));
}

float dotp(float v1[2], float v2[2])
{
    return v1[0]*v2[0]+v1[1]*v2[1];
}

float angle(float v1[2], float v2[2])
{
    float phi = acos(dotp(v1, v2)/(vabs(v1)*vabs(v2)));
    return phi;
}

// Berechnet Umdrehung von v bzgl. [1 0] bis 360 im CW-Sinn (Clockwise)
float angle_360_cw(float v[2])
{
    const float pi = 3.141593;
    float base[2] = {1, 0};
    float phi = angle(base, v);
    if (v[1] < 0)
        return fmod(phi, 2*pi);
    phi = 2*pi-phi;
    return fmod(phi, 2*pi);
}

// Berechnet Umdrehung von v bzgl. [1 0] bis 360 im CCW-Sinn (Counter-Clockwise)
float angle_360_ccw(float v[2])
{
    return ((2*Pi()) - angle_360_cw(v));           // "Komplementärwinkel"
}

// Rotiert v um Winkel alpha und speichert Resultat in result
// s wird für Normalisierung gebraucht
void Rotate2D(float v[2], float alpha, float result[2], float s)
{
    result[0] = cos(-alpha)*v[0]-sin(-alpha)*v[1];
    result[1] = sin(-alpha)*v[0]+cos(-alpha)*v[1];
    // ab und zu normalisieren
    if(rand()%20)
    {
        float norm = vabs(result);
        result = ScalarMult(s/norm, result);
    }
    return;
}

// Signum (Note: 0-Singularität wurde nicht beachtet)
float sgn(float a)
{
    if(a > 0)
        return 1.0;
    else
        return -1.0;
}
