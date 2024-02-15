from GravitySim import *

s = Simulation(10)
print(s[0].mass, s[0].x_position, s[0].y_position)

s[0] = CelestialBody(0, 1, 2)
print(s[0].mass, s[0].x_position, s[0].y_position)

s[1] = CelestialBody(1, 2, 3, x_velocity=0.1, y_velocity=-0.1)
print(s[1].mass, s[1].x_position, s[1].y_position, s[1].x_velocity, s[1].y_velocity)

print(s.get_coordinates_of(0))
print(s.get_coordinates_of(1))
