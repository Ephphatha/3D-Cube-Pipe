#include "ShakyCamera.h"

#include <boost/date_time/posix_time/posix_time.hpp>

ShakyCamera::ShakyCamera(const Vector3 &position,
                         float strength,
                         float shakeRate)
                         :
                         DynamicCamera(position),
                         strength(),
                         shakeRate(),
                         generator(static_cast<uint32_t>(boost::posix_time::microsec_clock::local_time().time_of_day().total_milliseconds())),
                         randomFloat(generator, boost::uniform_real<float>(-0.5f, 0.5f))
{
    SetShakeStrength(strength);
    SetShakeRate(shakeRate);
}

void ShakyCamera::Update(float dt)
{
    static float timeElapsed = 0.0f;
    static Vector3 offset;

    if(this->duration > 0.0f)
    {
        this->duration -= dt;

        timeElapsed += dt;

        if(timeElapsed > 1.0f / this->shakeRate)
        {
            while(timeElapsed > 1.0f / this->shakeRate)
            {   //Could potentially be an infinite loop if timeElapsed is ever a large value.
                timeElapsed -= 1.0f / this->shakeRate;
            }
            offset = Vector3(this->randomFloat(), this->randomFloat(), this->randomFloat()) * this->strength;
        }

        this->position += offset;
    }
}

void ShakyCamera::Shake(float duration)
{
    this->duration = duration;
}

void ShakyCamera::SetShakeStrength(float strength)
{
    if(strength >= 0.0f)
    {
        this->strength = strength;
    }
}

void ShakyCamera::SetShakeRate(float shakeRate)
{
    if(shakeRate >= 0.0f)
    {
        this->shakeRate = shakeRate;
    }
}
