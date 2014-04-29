#include "Quaternion.h"

#include <cmath>

const float Quaternion::PI = 3.14159f;

Quaternion::Quaternion(void):w(1.0f),xyz()
{
}

Quaternion::Quaternion(const Vector3 &axis):w(0),xyz(axis)
{
    this->Normalise();
}

Quaternion::Quaternion(const float &_w, const Vector3 &_xyz):w(_w),xyz(_xyz)
{
    this->Normalise();
}

Quaternion::Quaternion(const Vector3 &axis, const float &angle):w(cos(angle/2.0f)),xyz(axis * sin(angle/2.0f))
{
    this->Normalise();
}

Quaternion::Quaternion(const float &_x, const float &_y, const float &_z, const float &angle):w(cos(angle/2.0f)),xyz(_x * sin(angle/2.0f), _y * sin(angle/2.0f), _z * sin(angle/2.0f))
{
    this->Normalise();
}

Quaternion::Quaternion(const Quaternion &rhs):w(rhs.w),xyz(rhs.xyz)
{
}

Quaternion::~Quaternion(void)
{
}

Quaternion& Quaternion::operator=(const Quaternion &rhs)
{
    if(this != &rhs)
    {
        this->w = rhs.w;
        this->xyz = rhs.xyz;
    }

    return *this;
}


Quaternion& Quaternion::operator*=(const Quaternion &rhs)
{
    *this = *this * rhs;

    return *this;
}

Quaternion& Quaternion::operator/=(const float &scalar)
{
    this->w /= scalar;
    this->xyz /= scalar;

    return *this;
}

const Quaternion Quaternion::operator*(const Quaternion &rhs) const
{
    return Quaternion(this->w * rhs.w - this->xyz.Dot(rhs.xyz), this->xyz * rhs.w + rhs.xyz * this->w + this->xyz.Cross(rhs.xyz)).Normalise();
}


const float Quaternion::Angle(void)
{
    this->Normalise();

    return 2.0f * acos(this->w);
}

const float Quaternion::Angle(void) const
{
    return 2.0f * acos(this->w);
}

const Vector3 Quaternion::Axis(void)
{
    this->Normalise();

    return Vector3(this->xyz).Normalise();
}

const Vector3 Quaternion::Axis(void) const
{
    return Vector3(this->xyz).Normalise();
}

const Quaternion Quaternion::Conjugate(void) const
{
    return Quaternion(this->w, -(this->xyz));
}

const float Quaternion::Magnitude(void) const
{
    return sqrt(this->w * this->w + this->xyz.Dot(this->xyz));
}

Quaternion& Quaternion::Normalise(void)
{
    if(this->Magnitude() != 0.0f)
    {
        *this /= this->Magnitude();
    }

    return *this;
}

float Quaternion::DegreesToRadians(float degrees)
{
    return degrees * (Quaternion::PI / 180.0f);
}

float Quaternion::RadiansToDegrees(float radians)
{
    return radians * (180.0f / Quaternion::PI);
}

Vector3& operator*=(Vector3 &lhs, const Quaternion &rhs)
{
    lhs = lhs * rhs;

    return lhs;
}

const Vector3 operator*(const Vector3 &lhs, const Quaternion &rhs)
{
    return Quaternion(rhs * (Quaternion(lhs) * rhs.Conjugate())).Axis();
}
