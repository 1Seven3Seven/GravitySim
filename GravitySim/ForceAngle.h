#ifndef GRAVITYSIM_FORCEANGLE_H
#define GRAVITYSIM_FORCEANGLE_H

/**
 * @breif Contains a force and the angle it acts along.
 */
typedef struct
{
    double force;
    double angle;
} ForceAngle;

/**
 * @breif A simple way to create a ForceAngle struct.
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