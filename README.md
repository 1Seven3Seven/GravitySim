# GravitySim

An attempt at making a gravity simulation module in python.  
But with a catch, I am writing the bulk of the simulation code in c.

## Thinking things through

### The beginning

Currently, my idea is to have a bunch of celestial objects in a big ol list.  
At a time step, the forces between these objects are all calculated.  
Then applied to the acceleration, then to the velocity, then to the position.

### Working out collisions

Simple collisions: upon a collision the mass is added, and momentum conserved.  
Complex collisions: depending on the mass of the two bodies means either destruction or joining.  
I will be going through with simple collisions first.

So what is the size of the bodies?  
Well, let's have a density.  
From that we can calculate the radius.

```
Solving for radius based on density

area = pi * radius^2

density = mass / area

density = mass / (pi * radius ^ 2)
(pi * radius ^ 2) * density = mass
pi * radius ^ 2 = mass / density
radius ^ 2 = mass / (density * pi)
radius = sqrt(mass / (density * pi))
```

The radius only needs to be calculated each time the mass changes.
