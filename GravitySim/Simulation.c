#include "Simulation.h"

#include <stdlib.h>

#define _USE_MATH_DEFINES

#include <math.h>


void simulation_init(Simulation *simulation)
{
    simulation->bodies = NULL;
    simulation->num_bodies = 0;
}

int simulation_setup(Simulation *simulation, unsigned int number_bodies)
{
    simulation->num_bodies = number_bodies;
    simulation->num_in_use_bodies = number_bodies;

    // Allocating space for the bodies
    simulation->bodies = malloc(sizeof(CelestialBody) * number_bodies);
    if (simulation->bodies == NULL) goto cleanup;

    return 0;

    // Cleanup performed when a malloc fails
    cleanup:
    simulation->num_bodies = 0;

    free(simulation->bodies);
    simulation->bodies = NULL;

    return -1;
}

void simulation_de_init(Simulation *simulation)
{
    if (simulation->bodies != NULL)
    {
        free(simulation->bodies);
        simulation->bodies = NULL;
    }

    simulation->num_bodies = 0;
    simulation->num_in_use_bodies = 0;
}

void simulation_calculate_and_apply_forces(Simulation *simulation, double gravitational_constant)
{
    for (unsigned int i = 0; i < simulation->num_in_use_bodies; i++)
    {
        for (unsigned int j = i + 1; j < simulation->num_in_use_bodies; j++)
        {
            ForceAngle force_angle = calculate_force_between(&simulation->bodies[i],
                                                             &simulation->bodies[j],
                                                             gravitational_constant);

            apply_force(&simulation->bodies[i], force_angle);
            force_angle.angle += M_PI;
            apply_force(&simulation->bodies[j], force_angle);
        }
    }
}

void simulation_update_positions(Simulation *simulation, double delta_time)
{
    for (unsigned int i = 0; i < simulation->num_in_use_bodies; i++)
    {
        apply_acceleration(&simulation->bodies[i], delta_time);
        apply_velocity(&simulation->bodies[i], delta_time);
    }
}

/*
 * Helper function for handle_collisions.
 * Combines the CelestialBodies at index i and j and returns the result.
 */
static CelestialBody combine_bodies(Simulation *simulation, unsigned int i, unsigned int j)
{
    CelestialBody body_1 = simulation->bodies[i];
    CelestialBody body_2 = simulation->bodies[j];
    CelestialBody result;

    // Combine the masses
    result.mass = body_1.mass + body_2.mass;

    // Now grab the average position weighted by their mass
    result.x_position = (body_1.x_position * body_1.mass + body_2.x_position * body_2.mass) / result.mass;
    result.y_position = (body_1.y_position * body_1.mass + body_2.y_position * body_2.mass) / result.mass;

    // Now grab the combined velocity of the bodies
    // Converted to momentum then back to velocity
    result.x_velocity = (body_1.x_velocity * body_1.mass + body_2.x_velocity * body_2.mass) / result.mass;
    result.y_velocity = (body_1.y_velocity * body_1.mass + body_2.y_velocity * body_2.mass) / result.mass;

    // Finally, combined their density weighted by their mass
    result.density = (body_1.density * body_1.mass + body_2.density * body_2.mass) / result.mass;

    // Set the size
    recalculate_size(&result);

    // Done
    return result;
}

/*
 * Going with a simple implementation for now.
 * Check if each of the bodies is colliding with each other.
 * If there is a collision, then combine them.
 *
 * After a combination, the newly created body is placed at the front of the array.
 * And the other bodies moved around to facilitate this.
 * The moved bodies are moved in a way to prevent them from their handling being skipped.
 * Finally, we then re-do the collision checks for the combined bodies as their positions and sizes have changed.
 * This is repeated until there are no new collisions and thus combinations.
 */
void handle_collisions(Simulation *simulation)
{
    unsigned int bodies_to_update = simulation->num_in_use_bodies;

    while (bodies_to_update)
    {
        unsigned int number_combined_bodies = 0;

        for (unsigned int i = 0; i < bodies_to_update; i++)
        {
            for (unsigned int j = i + 1; j < simulation->num_in_use_bodies; j++)
            {
                CelestialBody body_1 = simulation->bodies[i];
                CelestialBody body_2 = simulation->bodies[j];

                double delta_x = body_1.x_position - body_2.x_position;
                double delta_y = body_1.y_position - body_2.y_position;

                double distance = sqrt(delta_x * delta_x + delta_y * delta_y);

                if (distance > body_1.size + body_2.size) continue;

                // We have a collision

                // Store the first non-combined body
                CelestialBody temp = simulation->bodies[number_combined_bodies];

                // We have a collision, combine the two offending bodies and store the result at the start
                simulation->bodies[number_combined_bodies] = combine_bodies(simulation, i, j);

                // Handle the case of index number_combined_bodies not being the same as i
                if (number_combined_bodies != i)
                {
                    // If they are not the same, then the body at index i is a dud, so set to temp
                    simulation->bodies[i] = temp;
                }

                // The body at j is now a dud, we swap with the end
                simulation->bodies[j] = simulation->bodies[simulation->num_in_use_bodies - 1];

                // One less in use
                simulation->num_in_use_bodies--;

                // As we combined one
                number_combined_bodies++;

                // We are done here
                break;
            }
        }

        // The number of bodies we need to update is only the ones we have changed
        bodies_to_update = number_combined_bodies;
    }
}
