#include <../math/vector2.h>
#include <stdlib.h>

double Vector2::cheap_length()
{
    return abs(x) + abs(y);
}

double Vector2::length()
{
    return sqrt(pow(x, 2) + pow(y, 2));
}

Vector2 Vector2::normalize()
{
    Vector2 _vec = {x, y};

    double _length = length();
    if(_length != 0)
    {
        _vec.x /= _length;
        _vec.y /= _length;
    }

    return _vec;
}

Vector2 Vector2::operator+(Vector2 const&_other)
{
    Vector2 _this = {x, y};

    _this.x += _other.x;
    _this.y += _other.y;

    return _this;
}

Vector2 Vector2::operator-(Vector2 const&_other)
{
    Vector2 _this = {x, y};
    
    _this.x -= _other.x;
    _this.y -= _other.y;

    return _this;
}

Vector2 Vector2::operator*(Vector2 const&_other)
{
    Vector2 _this = {x, y};

    _this.x *= _other.x;
    _this.y *= _other.y;

    return _this;
}

Vector2 Vector2::operator/(Vector2 const&_other)
{
    Vector2 _this = {x, y};
    
    _this.x /= _other.x;
    _this.y /= _other.y;

    return _this;
}

Vector2 Vector2::operator*(double const&_other)
{
    Vector2 _this = {x, y};

    _this.x *= _other;
    _this.y *= _other;

    return _this;
}

Vector2 Vector2::operator/(double const&_other)
{
    Vector2 _this = {x, y};
    
    _this.x /= _other;
    _this.y /= _other;

    return _this;
}

void Vector2::operator+=(Vector2 const&_other)
{
    x += _other.x;
    y += _other.y;
}

void Vector2::operator-=(Vector2 const&_other)
{
    x -= _other.x;
    y -= _other.y;
}

void Vector2::operator*=(Vector2 const&_other)
{
    x *= _other.x;
    y *= _other.y;
}

void Vector2::operator/=(Vector2 const&_other)
{
    x /= _other.x;
    y /= _other.y;
}