# GravitySim
An attempt at making a gravity simulation module in python.  
But with a catch, I am writing the bulk of the simulation code in c.

## Thinking things through
Currently, my idea is to have a bunch of celestial objects in a big ol list.
At a time step, the forces between these objects are all calculated.
Then applied to the acceleration, then to the velocity, then to the position.
