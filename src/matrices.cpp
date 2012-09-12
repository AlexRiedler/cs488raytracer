#include "algebra.hpp"

// Return a matrix to represent a counterclockwise rotation of "angle"
// degrees around the axis "axis", where "axis" is one of the
// characters 'x', 'y', or 'z'.
Matrix4x4 rotation(double angle, char axis)
{
    angle *= M_PI/180.0;
    switch (axis) {
    case 'x':
    {
        double row[4][4] =
        {
            { 1, 0, 0, 0, },
            { 0, cos(angle), -sin(angle), 0 },
            { 0, sin(angle), cos(angle), 0 },
            { 0, 0, 0, 1 }
        };
        return Matrix4x4((double*)row);
    }
    break;
    case 'y':
    {
        double row[4][4] =
        {
            { cos(angle), 0, sin(angle), 0 },
            { 0, 1, 0, 0, },
            { -sin(angle), 0, cos(angle), 0 },
            { 0, 0, 0, 1 }
        };
        return Matrix4x4((double*)row);
    }
    break;
    case 'z':
    {
        double row[4][4] =
        {
            { cos(angle), -sin(angle),0, 0 },
            { sin(angle), cos(angle),0, 0 },
            { 0, 0, 1, 0, },
            { 0, 0, 0, 1 }
        };
        return Matrix4x4((double*)row);
    }
    break;
    }
    // should never reach here
    Matrix4x4 r;
    return r;
}

// Return a matrix to represent a displacement of the given vector.
Matrix4x4 translation(const Vector3D& displacement)
{
    double row[4][4] =
    {
        { 1, 0, 0, displacement[0] },
        { 0, 1, 0, displacement[1] },
        { 0, 0, 1, displacement[2] },
        { 0, 0, 0, 1 },
    };
    return Matrix4x4((double*)row);
}

// Return a matrix to represent a nonuniform scale with the given factors.
Matrix4x4 scaling(const Vector3D& scale)
{
    double row[4][4] =
    {
        { scale[0], 0, 0, 0 },
        { 0, scale[1], 0, 0 },
        { 0, 0, scale[2], 0 },
        { 0, 0, 0, 1 },
    };
    return Matrix4x4((double*)row);
}

