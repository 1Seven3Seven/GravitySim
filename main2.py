# noinspection PyPackageRequirements
import pygame
import sys
import random

import GravitySim


def main():
    pygame.init()
    pygame.display.set_caption("Title")
    window_size = (1280, 720)
    screen = pygame.display.set_mode(window_size)
    clock = pygame.time.Clock()

    running = False

    simulation = GravitySim.Simulation(1200)

    for i in range(simulation.num_bodies):
        simulation[i] = GravitySim.CelestialBody(100,
                                                 10,
                                                 random.randint(0, window_size[0]),
                                                 random.randint(0, window_size[1]))

    simulation.handle_collisions()

    # Main loop
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.display.quit()
                sys.exit("Pygame screen close")

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    pygame.display.quit()
                    sys.exit("Pygame screen close")

                if event.key == pygame.K_SPACE:
                    running = not running

        screen.fill((0, 0, 0))

        """BELOW"""

        if running:
            simulation.step(0.01, 0.1)

        for i in range(simulation.num_bodies):
            pygame.draw.circle(
                screen,
                (255, 255, 255),
                simulation.get_coordinates_of(i),
                simulation[i].size
            )

        """ABOVE"""

        pygame.display.flip()
        try:
            fps = str(round(1000 / clock.tick()))
        except ZeroDivisionError:
            fps = "NA"
        pygame.display.set_caption(fps)


if __name__ == "__main__":
    main()
