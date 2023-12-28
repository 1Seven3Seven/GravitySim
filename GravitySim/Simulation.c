#include "Simulation.h"

#include <stdlib.h>

void simulation_init(Simulation *simulation)
{
    simulation->bodies = NULL;
    simulation->num_bodies = 0;

    simulation->forces = NULL;
}

int simulation_setup(Simulation *simulation, unsigned int number_bodies)
{
    unsigned int rows_allocated = 0;
    simulation->num_bodies = number_bodies;

    // Allocating space for the bodies
    simulation->bodies = malloc(sizeof(CelestialBody) * number_bodies);
    if (simulation->bodies == NULL) goto cleanup;

    // Allocating space for pointers to the rows of the ForceAngle matrix
    // Calloc to make sure each pointer is NULL
    simulation->forces = calloc(number_bodies, sizeof(ForceAngle *));
    if (simulation->forces == NULL) goto cleanup;

    // Allocating space for each row of the matrix
    for (unsigned int i = 0; i < number_bodies; i++)
    {
        simulation->forces[i] = malloc(sizeof(ForceAngle) * number_bodies);
        if (simulation->forces[i] == NULL)
        {
            rows_allocated = i;
            goto cleanup;
        }
    }

    return 0;

    // Cleanup performed when a malloc fails
    cleanup:
    simulation->num_bodies = 0;

    if (simulation->bodies != NULL)
    {
        free(simulation->bodies);
        simulation->bodies = NULL;
    }

    if (simulation->forces != NULL)
    {
        for (unsigned int i = 0; i < rows_allocated; i++)
        {
            free(simulation->forces[i]);
        }

        free(simulation->forces);
        simulation->forces = NULL;
    }

    return -1;
}

void simulation_de_init(Simulation *simulation)
{
    if (simulation->bodies != NULL)
    {
        free(simulation->bodies);
        simulation->bodies = NULL;
    }

    if (simulation->forces != NULL)
    {
        for (unsigned int i = 0; i < simulation->num_bodies; i++)
        {
            if (simulation->forces[i] != NULL)
            {
                free(simulation->forces[i]);
                simulation->forces[i] = NULL;
            }
        }

        free(simulation->forces);
        simulation->forces = NULL;
    }

    simulation->num_bodies = 0;
}

void simulation_calculate_forces(Simulation *simulation)
{
    for (unsigned int i = 0; i < simulation->num_bodies; i++)
    {
        for (unsigned int j = 0; j < simulation->num_bodies; j++)
        {
            if (i == j) continue;

            simulation->forces[i][j] = calculate_force_between(&simulation->bodies[i], &simulation->bodies[j]);
        }
    }
}

void simulation_apply_forces(Simulation *simulation)
{
    for (unsigned int i = 0; i < simulation->num_bodies; i++)
    {
        for (unsigned int j = 0; j < simulation->num_bodies; j++)
        {
            if (i == j) continue;

            apply_force(&simulation->bodies[i], simulation->forces[i][j]);
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
