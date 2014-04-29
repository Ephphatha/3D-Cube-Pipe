/*!
**  \file Camera.h
**  \brief Defines the Camera class
**
**  \author Andrew James
*/
#ifndef __Camera
#define __Camera

#include "Vector3.h"
#include "Quaternion.h"

#include <SDL_OpenGL.h>

/*!
**  \class Camera
**  \brief Basic camera functionality implemented (pan/dolly and derivatives).
*/
class Camera
{
public:
    /*!
    **  \brief Constructs a camera at the specified position, looking down the
    **          forward axis, with the specified up direction.
    **
    **  Defaults to a camera at the origin looking down the -z axis with +y up.
    **
    **  \param position The desired camera position.
    **  \param forward  Direction the camera is looking.
    **  \param up       Up direction of the camera.
    **  \param right    Right side of the camera (will be recalculated, provided
    **                   only to indicate the handedness of the system).
    */
    Camera(const Vector3 &position = Vector3(),
           const Vector3 &forward = Vector3(0.0f, 0.0f, -1.0f),
           const Vector3 &up = Vector3(0.0f, 1.0f, 0.0f),
           const Vector3 &right = Vector3(1.0f, 0.0f, 0.0f));

    /*!
    **  \brief Getter for the forward vector.
    **
    **  \return A copy of the forward vector.
    */
    const Vector3 Forward() const;

    /*!
    **  \brief Getter for the right vector.
    **
    **  \return A copy of the right vector.
    */
    const Vector3 Right() const;

    /*!
    **  \brief Getter for the up vector.
    **
    **  \return A copy of the up vector.
    */
    const Vector3 Up() const;

    /*!
    **  \brief Getter for the position.
    **
    **  \return A copy of the position.
    */
    const Vector3 Position() const;

    /*!
    **  \brief Rotates the view without moving the camera.
    **
    **  Positive values for the rotation angle will rotate counter-clockwise about the axis.
    **
    **  A few examples on how to use it, (where angle is the angle in radians).
    **  To Yaw (turn left or right):
    **  Camera::Pan(Quaternion(Camera::Up(), angle));
    **
    **  To Pitch (look up or down):
    **  Camera::Pan(Quaternion(Camera::Right(), angle));
    **
    **  To Roll (rotate around the direction you're looking):
    **  Camera::Pan(Quaternion(Camera::Forward(), angle));
    **
    **  To do fancy stuff specify an arbitary rotation axis.
    **
    **  Helper functions for the three examples given above are provided, see links.
    **
    **  \param rotation Quaternion defining the rotation axis and angle.
    **  \sa Roll
    **  \sa Pitch
    **  \sa Yaw
    */
    void Pan(const Quaternion &rotation);

    /*!
    **  \brief Rolls the camera by the specified angle.
    **
    **  Positive values for the rotation angle will roll counter-clockwise (tip to the right)
    **   and negative values will roll clockwise (tip to the left).
    **  \param angle Angle in radians to roll by.
    */
    void Roll(float angle);

    /*!
    **  \brief Pitches the camera by the specified angle.
    **
    **  Positive values for the rotation angle will pitch counter-clockwise (look up)
    **   and negative values will pitch clockwise (look down).
    **  \param angle Angle in radians to pitch by.
    */
    void Pitch(float angle);

    /*!
    **  \brief Yaws the camera by the specified angle.
    **
    **  Positive values for the rotation angle will yaw counter-clockwise (twist to the right)
    **   and negative values will yaw clockwise (twist to the left).
    **  \param angle Angle in radians to yaw by.
    */
    void Yaw(float angle);

    /*!
    **  \brief Pans the camera to look at the target point.
    **
    **  \param target The target point.
    **  \param up The up vector. Use Camera::Up() if you want the local up vector.
    */
    virtual void LookAt(const Vector3 &target, const Vector3 &up);

    /*!
    **  \brief Moves the camera in the given direction by the magnitude of the vector.
    **
    **  Does not change the view angle, only the position of the camera.
    **
    **  A few examples on how to use it, (where distance is a float).
    **  To surge (move forward or backward):
    **  Camera::Dolly(Camera::Forward() * distance);
    **
    **  To sway (move right or left):
    **  Camera::Dolly(Camera::Right() * distance);
    **
    **  To heave (move up or down):
    **  Camera::Dolly(Camera::Up() * distance);
    **
    **  Helper functions for the three examples given above are provided.
    **  \param direction The direction to move in (the magnitude of this vector defines the move distance).
    **  \sa Heave
    **  \sa Surge
    **  \sa Sway
    */
    void Dolly(const Vector3 &direction);

    /*!
    **  \brief Moves the camera up or down.
    **
    **  Positive values for distance will move the camera up, negative values will move it down.
    **  \param distance The distance to move.
    */
    void Heave(float distance);

    /*!
    **  \brief Moves the camera forward or backward.
    **
    **  Positive values for distance will move the camera forward, negative values will move it backward.
    **  \param distance The distance to move.
    */
    void Surge(float distance);

    /*!
    **  \brief Moves the camera right or left.
    **
    **  Positive values for distance will move the camera right, negative values will move it left.
    **  \param distance The distance to move.
    */
    void Sway(float distance);

    /*!
    **  \brief Moves the camera to the specified position
    **
    **  \param position The position for the camera to occupy.
    */
    virtual void MoveTo(const Vector3 &position);

    /*!
    **  \brief Sets the view matrix in OpenGL.
    */
    void Render() const;

protected:
    Vector3 position,   //!< Position of the camera.
            forward,    //!< The forward direction.
            right,      //!< The side direction.
            up;         //!< The (drumroll please) up (*gasp*) direction.

    GLfloat matrix[16]; //!< The calculated view matrix.

    /*!
    **  \brief Calculates the view matrix.
    */
    void CalculateMatrix();
};
#endif
