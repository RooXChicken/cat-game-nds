#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <math.h>

static double min(double _value, double _min)
{
    if(_value > _min)
        return _min;

    return _value;
}

struct Vector2
{
    double x = 0;
    double y = 0;

    double length();
    double cheap_length();
    Vector2 normalize();

    Vector2 operator+(Vector2 const&_other);
    Vector2 operator-(Vector2 const&_other);
    Vector2 operator*(Vector2 const&_other);
    Vector2 operator/(Vector2 const&_other);
    
    Vector2 operator*(double const&_other);
    Vector2 operator/(double const&_other);

    void operator+=(Vector2 const&_other);
    void operator-=(Vector2 const&_other);
    void operator*=(Vector2 const&_other);
    void operator/=(Vector2 const&_other);
};

#endif