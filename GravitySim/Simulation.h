#ifndef GRAVITYSIM_SIMULATION_H
#define GRAVITYSIM_SIMULATION_H

#include "CelestialBody.h"
#include "ForceAngle.h"

typedef struct _Simulation
{
    /// A dynamically allocated array of celestial bodies in this simulation.
    CelestialBody *bodies;

    /// The number of bodies in the simulation.
    unsigned int num_bodies;
} Simulation;

/// Initialises the simulation struct.
/// Must be called before using the struct.
void simulation_init(Simulation *simulation);

/// Sets up the simulation to handle the given number of celestial bodies.
/// Returns 0 on success, -1 on failure (a malloc failed).
/// If a failure occurs then the state is returned to just after simulation_init is called.
int simulation_setup(Simulation *simulation, unsigned int number_bodies);

/// Frees any allocated memory and returns the state to just after simulation_init is called.
void simulation_de_init(Simulation *simulation);

/// Calculates and applies the forces between CelestialBody structs in one step.
void simulation_calculate_and_apply_forces(Simulation *simulation, double gravitational_constant);

/// Applies the acceleration then the velocity of all bodies to update their positions over some delta time.
void simulation_update_positions(Simulation *simulation, double delta_time);

/// Saves the current state of the simulation to a file at the given file name.
/// Returns 0 on success, negative number on failure.
///
/// \t -1 on failure to open file.
/// \t -2 on failure to write to file.
///
/// This is written rather badly and is probably system dependant.
int simulation_save_state(Simulation *simulation, const char *file_name);

#endif