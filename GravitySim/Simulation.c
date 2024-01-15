#include "Simulation.h"

#include <stdlib.h>
#include <stdio.h>

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
}

void simulation_calculate_and_apply_forces(Simulation *simulation, double gravitational_constant)
{
    for (unsigned int i = 0; i < simulation->num_bodies; i++)
    {
        for (unsigned int j = i + 1; j < simulation->num_bodies; j++)
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
    for (unsigned int i = 0; i < simulation->num_bodies; i++)
    {
        apply_acceleration(&simulation->bodies[i], delta_time);
        apply_velocity(&simulation->bodies[i], delta_time);
    }
}
