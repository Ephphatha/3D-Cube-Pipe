/*!
**  \file Box.h
**  \brief Defines the Box class
**
**  \author Andrew James
**  \sa Box
*/
#ifndef __Box
#define __Box

#include "Vector3.h"
#include "Quaternion.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <SDL_OpenGL.h>

/*!
**  \class Box
**  \brief Defines a unit cube with rotation about an axis.
**
**  Due to the cube being unit dimensions, the rotation axis is also used as the translation.
**  Cubes draw themselves then the next cube in the list.
**  (Who knew learning about data structures would actually be useful?)
*/
class Box
{
public:
    const static GLfloat vertices[72];      //!< The vertices defining a box.
    const static GLfloat normals[72];       //!< The normals for each vertex.
    const static GLfloat colours[72];       //!< The colours for each vertex.

    /*!
    **  \brief Creates a unit cube with the specified rotation angle and axis.
    **
    **  The rotation axis is also used as the position in this program.
    **  \param _angle The rotation angle
    **  \param _axis The axis of rotation.
    */
    Box(const float &_angle, const Vector3 &_axis);

    /*!
    **  \brief Draws a unit cube with its own rotation and position
    **
    **  This will draw a box, rotate the world by its angle around its rotation axis,
    **  then call the next boxes draw function. This *should* mean that the chain is
    **  built correctly as all rotations will be applied to the current box and each
    **  previous box.
    */
    virtual void Draw() const;

    /*!
    **  \brief Rotates the box about its rotation axis.
    **
    **  \param angle Rotation angle in radians.
    */
    void Rotate(const float &angle);

    /*!
    **  \brief Adds a box to the next pointer.
    **
    **  Be careful about making a circular list.
    **  \param next Pointer to the next box.
    **  \return Returns true if the box was added to the list, false otherwise.
    **          If the return value is false, the list has not been modified.
    */
    bool SetNext(const boost::shared_ptr<Box> &next);

    /*!
    **  \brief Adds a box to the prev pointer.
    **
    **  Will only add a weak link, so the previous pointer needs to be held
    **  somewhere or it will be deallocated automatically.
    **  \param prev Pointer to the previous box.
    **  \return Returns true if the box was added to the list, false otherwise.
    **          If the return value is false, the list has not been modified.
    */
    bool SetPrev(const boost::shared_ptr<Box> &prev);

    /*!
    **  \brief Returns the next pointer.
    **
    **  \return The pointer to the next box.
    */
    const boost::shared_ptr<Box> Next();

    /*!
    **  \brief Returns the prev pointer.
    **
    **  \return The pointer to the previous box.
    */
    const boost::shared_ptr<Box> Prev();

    /*!
    **  \brief Sets the active state of the box.
    **
    **  (Only used to show the user what box is selected.)
    **  \param isActive New state of the box.
    */
    void Active(const bool &isActive);

protected:
    float angle;                    //!< The rotation angle.
    Vector3 axis;                   //!< The rotation axis also serves as the position.

    boost::shared_ptr<Box> next;    //!< Pointer to the next box in the list.
    boost::weak_ptr<Box> prev;      //!< Pointer to the previous box in the list.

    bool isActive;                  //!< Is the box currently selected?

    /*!
    **  \brief No args constructor is provided for internal use only.
    **
    **  Call Box(0, Vector3(0)) instead.
    */
    Box();

    /*!
    **  \brief Copy constructor is provided for internal use only.
    **
    **  \param rhs Box to copy.
    */
    Box(const Box &rhs);

    /*!
    **  \brief Assignment operator is provided for internal use only.
    **
    **  \param rhs Box to copy.
    */
    void operator=(const Box &rhs);
};

/*!
**  \class MasterBox
**  \brief Box element that acts as the head of a pipe section.
*/
class MasterBox : public Box
{
public:
    /*!
    **  \brief Creates a MasterBox from a Box (when promoting a list element to the head).
    **
    **  \param rhs Box to copy.
    */
    MasterBox(const Box &rhs);

    /*!
    **  \brief Creates a MasterBox with the specified position.
    **
    **  \param _position Vector3 describing the position of the box.
    */
    MasterBox(const Vector3 &_position);

    /*!
    ** \brief Getter for the forward vector.
    **
    ** \return A copy of the forward vector.
    */
    const Vector3 Forward() const;

    /*!
    ** \brief Getter for the right vector.
    **
    ** \return A copy of the right vector.
    */
    const Vector3 Right() const;

    /*!
    ** \brief Getter for the up vector.
    **
    ** \return A copy of the up vector.
    */
    const Vector3 Up() const;

    /*!
    **  \brief Moves the box in the specified direction.
    **
    **  \param direction Vector3 describing the direction and distance to move.
    */
    void Dolly(const Vector3 &direction);

    /*!
    **  \brief Rotates the box.
    **
    **  \param rotation Quaternion describing the angle and axis of rotation.
    */
    void Pan(const Quaternion &rotation);

    /*!
    **  \brief Rolls the box by the specified angle.
    **
    **  \param angle Angle in radians to roll by.
    */
    void Roll(float angle);

    /*!
    **  \brief Pitches the box by the specified angle.
    **
    **  \param angle Angle in radians to pitch by.
    */
    void Pitch(float angle);

    /*!
    **  \brief Yaws the box by the specified angle.
    **
    **  \param angle Angle in radians to yaw by.
    */
    void Yaw(float angle);

    /*!
    **  \brief Overloads Box::Draw to apply the correct translation.
    */
    void Draw() const;

protected:
    Vector3 position,   //!< Position of the box.
            forward,    //!< The forward direction.
            right,      //!< The right direction.
            up;         //!< The (drumroll please) up (*gasp*) direction.

    GLfloat matrix[16]; //!< The calculated rotation matrix.

    /*!
    **  \brief No args constructor is provided for internal use only.
    **
    **  Call MasterBox(Quaternion(), Vector3(0)) instead.
    */
    MasterBox();

    /*!
    **  \brief Copy constructor provided for internal use only.
    **
    **  \param rhs MasterBox to copy
    */
    MasterBox(const MasterBox &rhs);

    /*!
    **  \brief Assignment operator provided for internal use only.
    **
    **  \param rhs MasterBox to copy.
    */
    MasterBox& operator=(const MasterBox &rhs);

    /*!
    **  \brief Calculates the rotation matrix.
    */
    void CalculateMatrix();
};
#endif
