# Changelog

## Major Version 0

The Beginning of the initial development.

### Minor Version 0.1

Creating the most basic types to be used.

#### Tiny Version 0.1(.0)

Added a ForceAngle type that can store a force and the angle it acts along.  
Once created they are read only in python, but they exist purely to be created then used so that is fine for me.

(The last .0 was omitted in the toml file, hence why it is in parentheses).

#### Tiny Version 0.1.1

Added the CelestialBody type.  
This type stores the mass, position, velocity and acceleration of a particular celestial body.  
Attributes are read only but can be modified through the use of a couple functions:  
`apply_velocity`, `apply_acceleration` and `apply_force` (uses the aforementioned ForceAngle type).  
There is also another function `calculate_force_between` which is used to find the interaction between two celestial
bodies.

