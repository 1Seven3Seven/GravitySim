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

### Working out collisions 2

The simulation is set up so that the number of bodies is constant.  
But if bodies can combine when colliding, then the number of bodies is not constant.  
A simple solution is to have an array that stores which bodies should be updated.
Or have this stored in the body struct.  
Or I could go the dynamic array route and reallocate and move the data around when necessary.  
But I feel like an in-use array would be the easiest for now.

Actually, now that I think about, I am going to do something like a vector.  
If a body is now not in use, then I swap it with one in use at the end.  
This means that all the in use bodies are at the beginning of the array, whereas all the not in use ones are at the end.

### Thinking through the QuadTree

Interaction:

- Insert
- Get the contents of neighbouring cells
- Clear
