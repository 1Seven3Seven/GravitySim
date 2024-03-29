#include "CelestialBody.h"
#include "ForceAngle.h"

#define _USE_MATH_DEFINES

#include <math.h>

void setup_celestial_body(CelestialBody *body, unsigned int mass, double density)
{
    body->mass = mass;
    body->density = density;

    body->size = sqrt(mass / (density * M_PI));

    body->x_position = 0;
    body->y_position = 0;
    body->x_velocity = 0;
    body->y_velocity = 0;
    body->x_acceleration = 0;
    body->y_acceleration = 0;
}

void recalculate_size(CelestialBody *body)
{
    body->size = sqrt(body->mass / (body->density * M_PI));
}

void apply_velocity(CelestialBody *body, double delta_time)
{
    body->x_position += body->x_velocity * delta_time;
    body->y_position += body->y_velocity * delta_time;
}

void apply_acceleration(CelestialBody *body, double delta_time)
{
    body->x_velocity += body->x_acceleration * delta_time;
    body->y_velocity += body->y_acceleration * delta_time;
}

ForceAngle calculate_force_between(CelestialBody *body_1, CelestialBody *body_2, double gravitational_constant)
{
    double delta_x = body_2->x_position - body_1->x_position;
    double delta_y = body_2->y_position - body_1->y_position;

    double distance_squared = delta_x * delta_x + delta_y * delta_y;

    // F = G * m1 * m2 / r^2
    double force = gravitational_constant * body_1->mass * body_2->mass / distance_squared;

    double angle = atan2(delta_y, delta_x);

    return CreateForceAngle(force, angle);
}

void apply_force(CelestialBody *body, ForceAngle force_angle)
{
    /*
     * F = m * a
     * a = F / m
     */

    double acceleration = force_angle.force / body->mass;

    body->x_acceleration += acceleration * cos(force_angle.angle);
    body->y_acceleration += acceleration * sin(force_angle.angle);
}
