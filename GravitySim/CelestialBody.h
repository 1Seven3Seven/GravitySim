#ifndef GRAVITYSIM_CELESTIALBODY_H
#define GRAVITYSIM_CELESTIALBODY_H

#include "ForceAngle.h"

typedef struct _CelestialBody {
  unsigned int mass;

  double  x_position;
  double  y_position;

  double  x_velocity;
  double  y_velocity;

  double  x_acceleration;
  double  y_acceleration;
} CelestialBody;

/// Applies the internal velocity to the position over a given change in time.
void apply_velocity(CelestialBody *body, double delta_time);

/// Applies the internal acceleration to the velocity over a given change in time.
void apply_acceleration(CelestialBody *body, double delta_time);

/// Calculate the force between two celestial bodies.
/// Specifically from the first to the second.
ForceAngle calculate_force_between(CelestialBody *body_1,
                                   CelestialBody *body_2,
                                   double gravatational_constant);

/// Applies the given force to the internal acceleration over a given change in time.
void apply_force(CelestialBody *body, ForceAngle force_angle);

#endif