struct Vector2
{
    double x = 0;
    double y = 0;

    Vector2 normalize();
};

Vector2 Vector2::normalize()
{
    Vector2 _vec = {x, y};

    double _length = sqrt(pow(_vec.x, 2) + pow(_vec.y, 2));
    if(_length != 0)
    {
        _vec.x /= _length;
        _vec.y /= _length;
    }

    return _vec;
}