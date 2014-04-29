/*!
**  \file ShakyCamera.h
**  \brief Defines a Camera class that moves smoothly along a path with a delay.
**
**  B-Spline curve calculation is based on code originally written by Tim Lambert
**   (from http://www.cse.unsw.edu.au/~lambert/splines/source.html)
**
**  \author Andrew James
*/
#ifndef __ShakyCamera
#define __ShakyCamera
#include "DynamicCamera.h"

#include <boost/random.hpp>

class ShakyCamera : public virtual DynamicCamera
{
public:
    /*!
    **  \brief Passes the specified values to the Camera constructor.
    **
    **  Defaults to a camera at the origin looking down the -z axis with +y up.
    **
    **  \param position     The desired camera position.
    **  \param strength     Strength of the shake effect.
    **  \param shakeRate    How often the camera moves per second.
    */
    ShakyCamera(const Vector3 &position = Vector3(),
                float strength = 1.0f,
                float shakeRate = 60.0f);

    virtual void Update(float dt);

    virtual void Shake(float duration = 1.0f);

    virtual void SetShakeStrength(float strength = 1.0f);

    virtual void SetShakeRate(float shakeRate = 60.0f);

protected:
    boost::mt19937 generator;
    boost::variate_generator<boost::mt19937&, boost::uniform_real<float> > randomFloat;

    float strength,
          shakeRate,
          duration;
};
#endif
