#ifndef GRAVITYSIM_FORCEANGLE_H
#define GRAVITYSIM_FORCEANGLE_H

/// Contains a force and the angle it acts along.
typedef struct _ForceAngle
{
    double force;
    double angle;
} ForceAngle;

inline
ForceAngle CreateForceAngle(double force, double angle)
{
    ForceAngle force_angle = {force, angle};
    return force_angle;
}

#endif