#include "Vector3.h"

#include <cmath>

Vector3::Vector3(void):x(0),y(0),z(0)
{
}

Vector3::Vector3(float _s):x(_s),y(_s),z(_s)
{
}

Vector3::Vector3(float _x, float _y, float _z):x(_x),y(_y),z(_z)
{
}

Vector3::Vector3(const Vector3 &rhs):x(rhs.x),y(rhs.y),z(rhs.z)
{
}

Vector3& Vector3::operator=(const Vector3 &rhs)
{
    if(this != &rhs)
    {
        this->x = rhs.x;
        this->y = rhs.y;
        this->z = rhs.z;
    }

    return *this;
}

const Vector3 Vector3::operator-(void) const
{
    return Vector3(-this->x, -this->y, -this->z);
}

bool Vector3::operator==(const Vector3 &rhs) const
{
    const static float threshold = 0.001f;

    return
        fabs(this->x - rhs.x) > threshold ? false :
        fabs(this->y - rhs.y) > threshold ? false :
        fabs(this->z - rhs.z) > threshold ? false :
        true;
}

bool Vector3::operator!=(const Vector3 &rhs) const
{
    return !(*this == rhs);
}


Vector3& Vector3::operator+=(const Vector3 &rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;

    return *this;
}

Vector3& Vector3::operator-=(const Vector3 &rhs)
{
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;

    return *this;
}

Vector3& Vector3::operator*=(const float &scalar)
{
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;

    return *this;
}

Vector3& Vector3::operator/=(const float &scalar)
{
    this->x /= scalar;
    this->y /= scalar;
    this->z /= scalar;

    return *this;
}


const Vector3 Vector3::operator+(const Vector3 &rhs) const
{
    return Vector3(*this) += rhs;
}

const Vector3 Vector3::operator-(const Vector3 &rhs) const
{
    return Vector3(*this) -= rhs;
}

const Vector3 Vector3::operator*(const float &scalar) const
{
    return Vector3(*this) *= scalar;
}

const Vector3 Vector3::operator/(const float &scalar) const
{
    return Vector3(*this) /= scalar;
}


const Vector3 Vector3::Cross(const Vector3 &rhs) const
{
    return Vector3(this->y * rhs.z - this->z * rhs.y, this->z * rhs.x - this->x * rhs.z, this->x * rhs.y - this->y * rhs.x);
}

const float Vector3::Dot(const Vector3 &rhs) const
{
    return (this->x * rhs.x) + (this->y * rhs.y) + (this->z * rhs.z);
}

const float Vector3::Norm(void) const
{
    return sqrt(this->Dot(*this));
}

Vector3& Vector3::Normalise(void)
{
    if(this->Norm() != 0.0f)
    {   //Gotta watch out for a division by 0.
        *this /= this->Norm();
    }

    return *this;
}
