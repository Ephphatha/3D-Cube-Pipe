/*!
**  \file Quaternion.h
**  \brief Defines the Quaternion class
**
**  \author Andrew James
*/
#ifndef __Quaternion
#define __Quaternion

#include "Vector3.h"

/*!
**  \class Quaternion
**  \brief Defines a simple quaternion class with most needed functions.
**
**  Uses a Vector3 for the internal representation of the rotation axis.
**  \sa Vector3
*/
class Quaternion
{
public:
    /*!
    **  \brief No args constructor creates a Quaternion with no rotation around a null axis.
    */
    Quaternion(void);

    /*!
    **  \brief Creates a Quaternion from a vector.
    **
    **  Creates a Quaternion with w = 0, implies a rotation angle of PI radians.
    **  \param axis The rotation axis.
    */
    Quaternion(const Vector3 &axis);

    /*!
    **  \brief Creates a Quaternion with the specified w and xyz values.
    **
    **  Renormalises just in case.
    **  \param _w The w value.
    **  \param _xyz The xyz values.
    */
    Quaternion(const float &_w, const Vector3 &_xyz);

    /*!
    **  \brief Creates a Quaternion with the specified angle around the given axis.
    **
    **  The axis will be normalised before stored internally.
    **  \param axis The rotation axis.
    **  \param angle The angle of rotation.
    */
    Quaternion(const Vector3 &axis, const float &angle);

    /*!
    **  \brief Creates a Quaternion with the specified angle around the given axis.
    **
    **  The axis will be normalised before stored internally.
    **  \param _x The x value of the rotation axis.
    **  \param _y The y value of the rotation axis.
    **  \param _z The z value of the rotation axis.
    **  \param angle The angle of rotation.
    */
    Quaternion(const float &_x, const float &_y, const float &_z, const float &angle);

    /*!
    **  \brief Creates a Quaternion which is a copy of the given Quaternion.
    **
    **  \param rhs The Quaternion to be copied.
    */
    Quaternion(const Quaternion &rhs);

    /*!
    **  \brief Does nothing, declared for standards compliance.
    */
    ~Quaternion(void);

    /*!
    **  \brief Sets the values of this Quaternion to the values given in rhs.
    **
    **  \param rhs The Quaternion to copy.
    **  \return A reference to self.
    */
    Quaternion& operator=(const Quaternion &rhs);


    /*!
    **  \brief Multiplies self by the given Quaternion then assigns the result to self.
    **
    **  \param rhs The rhs of the multiplication.
    **  \return A reference to self.
    */
    Quaternion& operator*=(const Quaternion &rhs);

    /*!
    **  \brief Divides self by the given scalar then assigns the result to self.
    **
    **  \param scalar The rhs of the divison.
    **  \return A reference to self.
    */
    Quaternion& operator/=(const float &scalar);


    /*!
    **  \brief Multiplies self by the given Quaternion and returns the result.
    **
    **  \param rhs The rhs of the multiplication.
    **  \return A reference to self.
    */
    const Quaternion operator*(const Quaternion &rhs) const;


    /*!
    **  \brief Returns the angle of rotation.
    **
    **  Normalises self when called.
    **  \return A float representing the rotation amount in radians.
    */
    const float Angle(void);

    /*!
    **  \brief Returns the angle of rotation. (const version)
    **
    **  \return A float representing the rotation amount in radians.
    */
    const float Angle(void) const;

    /*!
    **  \brief Returns the axis of rotation.
    **
    **  Normalises self when called.
    **  \return A vector representing the axis of rotation.
    */
    const Vector3 Axis(void);

    /*!
    **  \brief Returns the axis of rotation. (const version)
    **
    **  \return A vector representing the axis of rotation.
    */
    const Vector3 Axis(void) const;

    /*!
    **  \brief Returns the conjugate of the Quaternion.
    **
    **  \return The conjugate of the Quaternion (w, -x, -y, -z)
    */
    const Quaternion Conjugate(void) const;

    /*!
    **  \brief Returns the magnitude (or length) of the Quaternion.
    **
    **  The magnitude is the square root of the sum of the squares of each element.
    **  \return The magnitude of the Quaternion.
    */
    const float Magnitude(void) const;

    /*!
    **  \brief Normalises the Quaternion.
    */
    Quaternion& Normalise(void);

    /*!
    **  \brief Converts degrees to radians.
    **
    **  \param degrees Value to be converted (in degrees).
    **  \return The value converted to radians.
    */
    static float DegreesToRadians(float degrees);

    /*!
    **  \brief Converts radians to degrees.
    **
    **  \param radians Value to be converted (in radians).
    **  \return The value converted to degrees.
    */
    static float RadiansToDegrees(float radians);

    Vector3 xyz;    //!< Rotation axis stored as axis/sin(angle/2). If you want the axis call Axis()
    float   w;      //!< Rotation angle stored as cos(angle/2)

    const static float PI;
};

/*!
**  \brief Rotates the lhs Vector3 around the specified Quaternion and saves the result in lhs.
**
**  lhs WILL BE NORMALISED. Save its magnitude beforehand in order to restore it to the same state.
**  \param lhs The Vector3 to be rotated.
**  \param rhs The Quaternion to rotate around.
**  \return A reference to lhs.
*/
Vector3& operator*=(Vector3 &lhs, const Quaternion &rhs);

/*!
**  \brief Rotates a Vector3 around a Quaternion.
**
**  \param lhs The Vector3 to be rotated
**  \param rhs The Quaternion to rotate around.
**  \return The rotated Vector3.
*/
const Vector3 operator*(const Vector3 &lhs, const Quaternion &rhs);

#endif
