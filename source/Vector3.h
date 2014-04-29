/*!
**  \file Vector3.h
**  \brief Defines the Vector3 class
**
**  \author Andrew James
*/
#ifndef __Vector3
#define __Vector3

/*!
**  \class Vector3
**  \brief Defines a simple three dimensional vector class with most needed functions.
**
**  There are ten million vector libraries out there but I wanna use my own...
*/
class Vector3
{   // http://www.cs.caltech.edu/courses/cs11/material/cpp/donnie/cpp-ops.html for notes on operator overloading.
public:
    /*!
    **  \brief No args constructor creates a vector with all elements set to 0.
    */
    Vector3(void);

    /*!
    **  \brief Creates a Vector3 with all elements set to the given scalar.
    **
    **  \param _s The initial value for each Vector3 element.
    */
    Vector3(float _s);

    /*!
    **  \brief Creates a Vector3 with the specified values for each element.
    **
    **  \param _x The initial x value.
    **  \param _y The initial y value.
    **  \param _z The initial z value.
    */
    Vector3(float _x, float _y, float _z);

    /*!
    **  \brief Creates a Vector3 from the given Vector3.
    **
    **  \param rhs The Vector3 to copy.
    */
    Vector3(const Vector3 &rhs);

    /*!
    **  \brief Sets the values of this Vector3 to the values given in rhs.
    **
    **  \param rhs The Vector3 to copy.
    **  \return A reference to self.
    */
    Vector3& operator=(const Vector3 &rhs);


    /*!
    **  \brief Returns a Vector3 with elements set to the negated values of this Vector3.
    **
    **  \return The negated Vector3 (-x, -y, -z).
    */
    const Vector3 operator-(void) const;

    bool operator==(const Vector3 &rhs) const;

    bool operator!=(const Vector3 &rhs) const;


    /*!
    **  \brief Adds the rhs Vector3 to self.
    **
    **  \param rhs The Vector3 to be added.
    **  \return A reference to self.
    */
    Vector3& operator+=(const Vector3 &rhs);

    /*!
    **  \brief Subtracts the rhs Vector3 from self.
    **
    **  \param rhs The Vector3 to be subtracted.
    **  \return A reference to self.
    */
    Vector3& operator-=(const Vector3 &rhs);

    /*!
    **  \brief Multiplies this Vector3 by the given scalar.
    **
    **  \param scalar The scalar to multiply by.
    **  \return A reference to self.
    */
    Vector3& operator*=(const float &scalar);

    /*!
    **  \brief Divides this Vector3 by the given scalar.
    **
    **  \param scalar The scalar to divide by.
    **  \return A reference to self.
    */
    Vector3& operator/=(const float &scalar);


    /*!
    **  \brief Adds two vectors.
    **
    **  The operator is called as a member function of the lhs of the equation.
    **  \param rhs The rhs of the equation.
    **  \return The result of the operation.
    */
    const Vector3 operator+(const Vector3 &rhs) const;

    /*!
    **  \brief Subtracts two vectors.
    **
    **  The operator is called as a member function of the lhs of the equation.
    **  \param rhs The rhs of the equation.
    **  \return The result of the operation.
    */
    const Vector3 operator-(const Vector3 &rhs) const;

    /*!
    **  \brief Multiplies a Vector3 by a scalar.
    **
    **  The operator is called as a member function of the lhs of the equation.
    **  For this function to be called the expression must be of the form
    **  vector * scalar, not scalar * vector due to limitations of the C++ language.
    **  Ok, I could define a global operation that would allow the second form, but
    **  I don't want to.
    **  \param scalar The rhs of the equation.
    **  \return The result of the operation.
    */
    const Vector3 operator*(const float &scalar) const;

    /*!
    **  \brief Divides a Vector3 by a scalar.
    **
    **  The operator is called as a member function of the lhs of the equation.
    **  \param scalar The rhs of the equation.
    **  \return The result of the operation.
    */
    const Vector3 operator/(const float &scalar) const;


    /*!
    **  \brief Returns the cross product of two Vector3s.
    **
    **  The operator is called as a member function of the lhs of the equation.
    **  \param rhs The rhs of the equation.
    **  \return The result of the operation (a vector).
    */
    const Vector3 Cross(const Vector3 &rhs) const;

    /*!
    **  \brief Returns the dot product of two Vector3s.
    **
    **  The operator is called as a member function of the lhs of the equation.
    **  \param rhs The rhs of the equation.
    **  \return The result of the operation (a float).
    */
    const float Dot(const Vector3 &rhs) const;

    /*!
    **  \brief Returns the magnitude (or length) of the given Vector3.
    **
    **  The norm is implemented as this->dot(*this).
    **  \return The magnitude of the Vector3.
    */
    const float Norm(void) const;

    /*!
    **  \brief Normalises the Vector3.
    **
    **  Normalising a Vector3 refers to making it unit length (Vector3::Norm() == 1).
    **  Will fail if the Vector3 has all elements == 0.
    **  \return A self reference.
    */
    Vector3& Normalise(void);

    float   x,  //!< The x element.
            y,  //!< The y element.
            z;  //!< The z element.
};
#endif
