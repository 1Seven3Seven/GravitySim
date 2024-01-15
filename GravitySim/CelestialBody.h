#ifndef GRAVITYSIM_CELESTIALBODY_H
#define GRAVITYSIM_CELESTIALBODY_H
/// @file

#include "ForceAngle.h"

/**
 * @brief A structure containing information about the position and movement of an object in space.
 */
struct _CelestialBody
{
    /// @brief The mass of this body.
    unsigned int mass;

    /// @brief The density of this body.
    double density;

    /// @brief The size of this body.
    double size;

    /// @brief The x position of this body.
    double x_position;
    /// @brief The y position of this body.
    double y_position;

    /// @brief The x velocity of this body.
    double x_velocity;
    /// @brief The y velocity of this body.
    double y_velocity;

    /// @brief The instantaneous x acceleration of this body.
    double x_acceleration;
    /// @brief The instantaneous y acceleration of this body.
    double y_acceleration;
};

typedef struct _CelestialBody CelestialBody;

/**
 * @brief Sets up the celestial body to have the given mass and density.
 * @param mass The mass of the celestial body.
 * @param density The density of the celestial body.
 *
 * @paragraph Specifics
 * Sets the mass and density of the given CelestialBody struct.
 * The size is then calculated based on the mass and density.
 * The position, velocity and acceleration are all zeroed.
 */
void setup_celestial_body(CelestialBody *body, unsigned int mass, double density);

/**
 * @brief Recalculates the size of the given body, only needs to be called if the mass changed.
 * @param body The body whose size is being recalculated.
 */
void recalculate_size(CelestialBody *body);

/**
 * @brief Applies the internal velocity to the position over a given change in time.
 * @param body The CelestialBody whose internal acceleration is applied.
 * @param delta_time The time interval over which the velocity is applied.
 */
void apply_velocity(CelestialBody *body, double delta_time);

/**
 * @brief Applies the internal acceleration to the velocity over a given change in time.
 * @param body The CelestialBody whose internal acceleration is applied.
 * @param delta_time The time interval over which the velocity is applied.
 */
void apply_acceleration(CelestialBody *body, double delta_time);

/**
 * @brief Calculate the force from one CelestialBody to another.
 * @param body_1 The CelestialBody to calculate the force from.
 * @param body_2 the CelestialBody to calculate the force to.
 * @param gravitational_constant The gravitational constant to be used in the calculation.
 * @return The force as a ForceAngle struct from body_1 to body_2.
 */
ForceAngle calculate_force_between(CelestialBody *body_1,
                                   CelestialBody *body_2,
                                   double gravitational_constant);

/**
 * @brief Applies the given force to the internal acceleration.
 * @param body The CelestialBody the force is applied to.
 * @param force_angle The force to be applied.
 */
void apply_force(CelestialBody *body, ForceAngle force_angle);

#endif