/*!
**  \file ThirdPersonCamera.h
**  \brief Defines a specific DynamicCamera implementation that is meant to emulate a third person perspective.
**
**  Will constantly look at the target point, but will not move unless told to.
**
**  \author Andrew James
*/
#ifndef __ThirdPersonCamera
#define __ThirdPersonCamera
#include "DynamicCamera.h"

/*!
**  \class ThirdPersonCamera
**  \brief Camera that rotates to face the target point every time Update is called.
*/
class ThirdPersonCamera : public virtual DynamicCamera
{
public:
    /*!
    **  \brief Creates a camera at the specified position looking at the target.
    **
    **  \param position The camera position.
    **  \param target   The focal point of the camera.
    **  \param targetUp The local up direction.
    */
    ThirdPersonCamera(const Vector3 &position = Vector3(),
                      const Vector3 &target = Vector3(0.0f, 0.0f, -1.0f),
                      const Vector3 &targetUp = Vector3(0.0f, 1.0f, 0.0f));

    /*!
    **  \brief Rotates the camera to look at the target point.
    **
    **  \param dt Time elapsed since last update.
    */
    virtual void Update(float dt);

    /*!
    **  \brief Sets a new target point (overloads Camera::LookAt()).
    **
    **  \param target   The new target point.
    **  \param targetUp The new up direction.
    */
    void LookAt(const Vector3 &target, const Vector3 &targetUp);

protected:
    Vector3 target,
            targetUp;
};
#endif
