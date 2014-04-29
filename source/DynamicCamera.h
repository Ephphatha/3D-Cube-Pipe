/*!
**  \file DynamicCamera.h
**  \brief Defines an abstract Camera class that will be updated often.
**
**  For example, a camera following a planned path, or a third person view following a character
**
**  \author Andrew James
*/
#ifndef __DynamicCamera
#define __DynamicCamera

#include "Camera.h"

/*!
**  \class DynamicCamera
**  \brief Abstract class that ensures derived classes implement an Update() function.
*/
class DynamicCamera : public Camera
{
public:
    /*!
    **  \brief Passes the specified values to the Camera constructor.
    **
    **  Defaults to a camera at the origin looking down the -z axis with +y up.
    **
    **  \param position The desired camera position.
    **  \param forward  Direction the camera is looking.
    **  \param up       Up direction of the camera.
    **  \param right    Right side of the camera (will be recalculated, provided
    **                   only to indicate the handedness of the system).
    */
    DynamicCamera(const Vector3 &position = Vector3(),
                  const Vector3 &forward = Vector3(0.0f, 0.0f, -1.0f),
                  const Vector3 &up = Vector3(0.0f, 1.0f, 0.0f),
                  const Vector3 &right = Vector3(1.0f, 0.0f, 0.0f))
                  :
                  Camera(position,
                         forward,
                         up,
                         right)
                  {}

    /*!
    **  \brief Template for the Update function.
    **
    **  \param dt Time elapsed since last update. (Some classes may ignore this value).
    */
    virtual void Update(float dt) = 0;
};
#endif
