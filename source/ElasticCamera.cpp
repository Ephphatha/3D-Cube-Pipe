#include "ElasticCamera.h"

ElasticCamera::ElasticCamera(const Vector3 &position, float timeFactor)
                             :
                             DynamicCamera(position),
                             points(5, position),
                             t(),
                             timeFactor()
{
    SetTimeFactor(timeFactor);
    InterpolatePosition();
}

void ElasticCamera::Update(float dt)
{
    this->t += dt * this->timeFactor;
    if(this->t >= 1.0f)
    {
        while(this->t >= 1.0f)
        {
            this->t -= 1.0f;
        }

        this->points.erase(this->points.begin());
        while(this->points.size() < 4)
        {
            this->points.push_back(this->points.back());
        }
    }

    InterpolatePosition();
}

void ElasticCamera::MoveTo(const Vector3 &point)
{
    this->points.push_back(point);
}

void ElasticCamera::SetPosition(const Vector3 &position)
{
    this->points.assign(4, position);
}

void ElasticCamera::SetTimeFactor(float timeFactor)
{
    if(timeFactor >= 0.0f)
    {
        this->timeFactor = timeFactor;
    }
}

void ElasticCamera::AdjustSpeed(float scalar)
{
    if(scalar >= 0.0f)
    {
        this->timeFactor *= scalar;
    }
}

float ElasticCamera::Basis(int i, float t)
{   // The performance of this function can be improved.
    switch (i)
    {
    case -2:
        return (((-t + 3.0f) * t - 3.0f) * t + 1.0f) / 6.0f;
    case -1:
        return (((3.0f * t - 6.0f) * t) * t + 4.0f) / 6.0f;
    case 0:
        return (((-3.0f * t + 3.0f) * t + 3.0f) * t + 1.0f) / 6.0f;
    case 1:
        return (t * t * t) / 6.0f;
    default:
        return 0.0f;
    }
}

void ElasticCamera::InterpolatePosition()
{
    Vector3 position;
    std::list<Vector3>::const_iterator point = this->points.begin();
    for (int i = -2; i <= 1; ++i)
    {
        position += *point * Basis(i, this->t);

        if(++point == this->points.end())
        {   //If we hit this on anything other than the last loop we've got problems.
            break;
        }
    }

    this->position = position;
}
