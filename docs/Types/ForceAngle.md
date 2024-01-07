# ForceAngle

<!-- TOC -->
* [ForceAngle](#forceangle)
  * [Overview](#overview)
  * [C Implementation](#c-implementation)
    * [ForceAngle struct](#forceangle-struct)
    * [CreateForceAngle](#createforceangle)
  * [Python Implementation](#python-implementation)
<!-- TOC -->

## Overview

This is a simple object whose entire purpose is to be passed around and read from.    
It is mainly to simplify the signatures of the functions.

## C Implementation

### ForceAngle struct

```c
typedef struct _ForceAngle
{
    double force;
    double angle;
} ForceAngle;
```

A simple struct that contains the force as a magnitude and the angle it acts upon in radians.

### CreateForceAngle

```c
ForceAngle CreateForceAngle(double force, double angle)
```

A function to facilitate the easy creation of a ForceAngle struct.

## Python Implementation

```Python
class ForceAngle:
    def __init__(self, force: float, angle: float):
        force: float = ...
        angle: float = ...
```

A basic class to facilitate the passing around of forces.  
Both attributes are readonly.
