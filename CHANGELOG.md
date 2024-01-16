# Changelog

<!-- TOC -->
* [Changelog](#changelog)
  * [Major Version 0 — The Beginning of the initial development](#major-version-0--the-beginning-of-the-initial-development)
    * [Minor Version 0.1 — Creating the most basic types to be used](#minor-version-01--creating-the-most-basic-types-to-be-used)
      * [Tiny Version 0.1(.0) — Creating the ForceAngle type](#tiny-version-010--creating-the-forceangle-type)
      * [Tiny Version 0.1.1 — Creating the CelestialBody type](#tiny-version-011--creating-the-celestialbody-type)
      * [Tiny Version 0.1.2 — QOL changes to CelestialBody](#tiny-version-012--qol-changes-to-celestialbody)
    * [Minor Version 0.2 — Creating the simulation handler](#minor-version-02--creating-the-simulation-handler)
      * [Tiny Version 0.2.0 — Creating the Simulation type](#tiny-version-020--creating-the-simulation-type)
    * [Minor Version 0.3 — Implementing collisions](#minor-version-03--implementing-collisions)
      * [Tiny Version 0.3.0 Adding density and size to CelestialBody](#tiny-version-030-adding-density-and-size-to-celestialbody)
      * [Tiny Version 0.3.1 Improvement to Simulation](#tiny-version-031-improvement-to-simulation)
      * [Tiny Version 0.3.2 Implementing Collisions to C Simulation](#tiny-version-032-implementing-collisions-to-c-simulation)
<!-- TOC -->

## Major Version 0 — The Beginning of the initial development

### Minor Version 0.1 — Creating the most basic types to be used

#### Tiny Version 0.1(.0) — Creating the ForceAngle type

Added a ForceAngle type that can store a force and the angle it acts along.  
Once created, they are read only in python, but they exist purely to be created then used so that is fine for me.

(The last .0 was omitted in the toml file, hence why it is in parentheses).

#### Tiny Version 0.1.1 — Creating the CelestialBody type

Added the CelestialBody type.  
This type stores the mass, position, velocity and acceleration of a particular celestial body.  
Attributes are read only but can be modified through the use of a couple functions:  
`apply_velocity`, `apply_acceleration` and `apply_force` (uses the aforementioned ForceAngle type).  
There is also another function `calculate_force_between` which is used to find the interaction between two celestial
bodies.

#### Tiny Version 0.1.2 — QOL changes to CelestialBody

Quality of life change to the CelestialBody.  
When creating an object, you can now specify the x and y velocities; their defaults are 0.

```python
from GravitySim import CelestialBody

body = CelestialBody(1, 2, 3, x_velocity=1, y_velocity=-2)
```

### Minor Version 0.2 — Creating the simulation handler

#### Tiny Version 0.2.0 — Creating the Simulation type

When initialising, you specify the number of CelestialBody objects in the simulation.  
The underlying CelestialBody objects can be set and retrieved using brackets.  
Or just the position using `get_coordinates_of`.

```python
from GravitySim import CelestialBody, Simulation

sim = Simulation(10)

sim[5] = CelestialBody(1, 2, 3)

body = sim[2]

coords = sim.get_coordinates_of(1)
```  

CelestialBody objects outside the simulation cannot affect ones inside.

```python
from GravitySim import CelestialBody, Simulation

sim = Simulation(10)

body = CelestialBody(1, 2, 3)
sim[0] = body
body.x_position = 5

print(sim[0].x_position)  # 2
print(body.x_position)  # 5
```

The simulation can also be stepped through, finally making it possible to use it to simulate something.  
The underling CelestialBody data can be retrieved to display the result.

```python
from GravitySim import Simulation

sim = Simulation(10)

# Set bodies to initial state

delta_time = 0.1
gravitational_constant = 1

# Perform 10 steps
for _ in range(10):
    sim.step(delta_time, gravitational_constant)
```

### Minor Version 0.3 — Implementing collisions

#### Tiny Version 0.3.0 Adding density and size to CelestialBody

CelestialBody now has two extra members, `density` and `size`.  
`size` is calculated based on the density and mass of the body and is read only.  
The density of a body is provided upon initialisation.

The `__init__` function now looks like:

```python
def __init__(self, mass, density, x_position, y_position, x_acceleration=0, y_acceleration=0): ...
```

#### Tiny Version 0.3.1 Improvement to Simulation

I realised that the way I perform steps is very inefficient.

1. Calculate the force between each body and store it in a matrix.
2. Loop over the bodies and apply the necessary forces in the matrix.

This leads to the force from body i to body j being calculated twice.

I have removed the forces matrix, and when calculating the forces I apply them immediately flipping the angle when
applying it to the other body.

A little bit of a tangent, but I felt it was necessary to do.

#### Tiny Version 0.3.2 Implementing Collisions to C Simulation

We have a new function `simulation_handle_collisions` that does what it says on the tin.  
Collisions are handled by creating a new body with an averaged position, velocity and density.

This was one of the most challenging pieces of code to work out due to restraints I placed upon myself.  
Namely, that I did not want to have any dynamically allocated memory in the function.  
This meant that everything was done only using the bodies' array.

Well, because collisions reduce the number of bodies in the simulation, the simulation now tracks the number of bodies
actually being updated.  
The bodies not being updated are moved to the end of the array.    
This does mean that, when looping over the bodies in between steps, the bodies may not be in the same positions.

The collision check is not done in the most elegant of ways, namely a double for loop, but it was the simplest method,
and will be improved later on.

Hopefully it all works when I add it to the python type.  
Hopefully...

Good luck future me.
