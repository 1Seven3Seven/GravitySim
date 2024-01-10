#ifndef GRAVITYSIM_FORCEANGLE_H
#define GRAVITYSIM_FORCEANGLE_H
/// @file

/**
 * @brief Contains a force and the angle it acts along.
 */
struct _ForceAngle
{
    /// The force.
    double force;
    /// The angle the force acts along.
    double angle;
};

typedef struct _ForceAngle ForceAngle;

/**
 * @brief A simple way to create a ForceAngle struct.
 * @param force The force.
 * @param angle The angle.
 * @return The ForceAngle struct.
 */
inline ForceAngle CreateForceAngle(double force, double angle)
{
    ForceAngle force_angle = {force, angle};
    return force_angle;
}

#endif