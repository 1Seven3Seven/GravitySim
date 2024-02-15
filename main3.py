import os
import random
import time
import tkinter

# noinspection PyPackageRequirements
import pygame

import GravitySim


class SimulationHandlerInWindow(tkinter.Tk):
    """
    Embed done based on answer here: https://stackoverflow.com/a/23464185
    """

    def __init__(self,
                 simulation_num_bodies: int = 500,
                 simulation_bodies_start_mass: int = 100,
                 simulation_bodies_start_density: float = 10,
                 pygame_window_size: tuple[int, int] = (1280, 720),
                 control_frame_size: tuple[int, int] = (300, 720),
                 window_title: str = "Gravity Simulation Test"):
        super().__init__()
        self.title(window_title)

        self.pygame_window_size = pygame_window_size
        self.control_frame_size = control_frame_size

        self.simulation = GravitySim.Simulation(simulation_num_bodies)
        self._create_random_bodies(simulation_bodies_start_mass, simulation_bodies_start_density)
        self.simulation.handle_collisions()

        self.simulation_initial_state: list[GravitySim.CelestialBody] = []

        # Setting up the pygame embed
        self.pygame_embed = tkinter.Frame(self, width=pygame_window_size[0], height=pygame_window_size[1])
        self.pygame_embed.pack(side=tkinter.LEFT)
        os.environ["SDL_WINDOWID"] = str(self.pygame_embed.winfo_id())
        os.environ["SDL_VIDEODRIVER"] = "windib"

        self.screen = pygame.display.set_mode(pygame_window_size)
        self.screen.fill((0, 0, 0))
        pygame.display.init()
        pygame.display.update()

        self.offset = [0, 0]
        self.center_of_mass = [0, 0]
        self.center_of_bodies = [0, 0]
        self.mouse_pressed = False

        self.control_frame = tkinter.Frame(width=control_frame_size[0], height=control_frame_size[1])
        self.control_frame.pack(side=tkinter.LEFT)
        self.control_frame.pack_propagate(False)

        # region - Display fps
        self.fps_string_var = tkinter.StringVar()
        self.fps_string_var.set("FPS: xxx")
        self.fps_label = tkinter.Label(self.control_frame, textvariable=self.fps_string_var)
        self.fps_label.pack(side=tkinter.TOP)
        # endregion - Display fps

        # region - Display number of bodies
        self.num_bodies_string_var = tkinter.StringVar()
        self.num_bodies_string_var.set("Number of bodies: xxx")
        self.num_bodies_label = tkinter.Label(self.control_frame, textvariable=self.num_bodies_string_var)
        self.num_bodies_label.pack(side=tkinter.TOP)
        # endregion - Display number of bodies

        # region - Display center of offset
        self.offset_string_var = tkinter.StringVar()
        self.offset_string_var.set("Offset: [0, 0]")
        self.offset_label = tkinter.Label(self.control_frame, textvariable=self.offset_string_var)
        self.offset_label.pack(side=tkinter.TOP)
        # endregion - Display center of offset

        # region - Display center of mass
        self.center_of_mass_string_var = tkinter.StringVar()
        self.center_of_mass_string_var.set("Centre of mass: [0, 0]")
        self.center_of_mass_label = tkinter.Label(self.control_frame, textvariable=self.center_of_mass_string_var)
        self.center_of_mass_label.pack(side=tkinter.TOP)
        # endregion - Display center of mass

        # region - Display center of bodies
        self.center_of_bodies_string_var = tkinter.StringVar()
        self.center_of_bodies_string_var.set("Centre of bodies: [0, 0]")
        self.center_of_bodies_label = tkinter.Label(self.control_frame, textvariable=self.center_of_bodies_string_var)
        self.center_of_bodies_label.pack(side=tkinter.TOP)
        # endregion - Display center of bodies

        self.tracking_string_var = tkinter.StringVar(value="Tracking: nothing")
        self.tracking_label = tkinter.Label(self.control_frame, textvariable=self.tracking_string_var)
        self.tracking_label.pack(side=tkinter.TOP)

        self.reset_tracking_button = tkinter.Button(self.control_frame, text="Reset tracking",
                                                    command=self._reset_tracking)
        self.reset_tracking_button.pack(side=tkinter.TOP)

        self.tracking_int_var = tkinter.IntVar(value=-1)

        self.tracking_int_to_name = {
            -1: "nothing",
            0:  "center of bodies",
            1:  "specific body",
        }

        self.track_nothing_radiobutton = tkinter.Radiobutton(self.control_frame,
                                                             value=-1,
                                                             variable=self.tracking_int_var,
                                                             text="Track nothing")
        self.track_nothing_radiobutton.pack(side=tkinter.TOP)

        self.track_center_of_bodies_radiobutton = tkinter.Radiobutton(self.control_frame,
                                                                      value=0,
                                                                      variable=self.tracking_int_var,
                                                                      text="Track center of bodies")
        self.track_center_of_bodies_radiobutton.pack(side=tkinter.TOP)

        self.track_body_at_index_radiobutton = tkinter.Radiobutton(self.control_frame,
                                                                   value=1,
                                                                   variable=self.tracking_int_var,
                                                                   text="Track body at index")
        self.track_body_at_index_radiobutton.pack(side=tkinter.TOP)

        self.track_body_at_index_spinbox = tkinter.Spinbox(self.control_frame,
                                                           from_=-1,
                                                           to=self.simulation.num_bodies)
        self.track_body_at_index_spinbox.pack(side=tkinter.TOP)

    def _reset_tracking(self):
        self.offset = [0, 0]
        self.tracking_int_var.set(-1)
        self.track_body_at_index_spinbox.delete(0, tkinter.END)
        self.track_body_at_index_spinbox.insert(0, "-1")

    def _create_random_bodies(self, start_mass, start_density):
        for i in range(self.simulation.num_bodies):
            self.simulation[i] = GravitySim.CelestialBody(start_mass,
                                                          start_density,
                                                          random.randint(0, self.pygame_window_size[0]),
                                                          random.randint(0, self.pygame_window_size[1]))

    def _update_num_bodies(self, num_bodies: int):
        self.num_bodies_string_var.set(f"Number of bodies: {num_bodies}")

    def _update_offset(self):
        self.offset_string_var.set(f"Offset: {self.offset}")

    def _update_center_of_mass_and_bodies(self):
        self.center_of_bodies = [0, 0]
        self.center_of_mass = [0, 0]
        sum_mass = 0
        for i in range(self.simulation.num_bodies):
            celestial_body = self.simulation[i]

            self.center_of_bodies[0] += celestial_body.x_position
            self.center_of_bodies[1] += celestial_body.y_position

            self.center_of_mass[0] += celestial_body.x_position * celestial_body.mass
            self.center_of_mass[1] += celestial_body.y_position * celestial_body.mass
            sum_mass += celestial_body.mass

        self.center_of_bodies[0] /= self.simulation.num_bodies
        self.center_of_bodies[1] /= self.simulation.num_bodies
        self.center_of_bodies[0] = round(self.center_of_bodies[0] - self.pygame_window_size[0] // 2)
        self.center_of_bodies[1] = round(self.center_of_bodies[1] - self.pygame_window_size[1] // 2)

        self.center_of_mass[0] /= sum_mass
        self.center_of_mass[1] /= sum_mass
        self.center_of_mass[0] = round(self.center_of_mass[0] - self.pygame_window_size[0] // 2)
        self.center_of_mass[1] = round(self.center_of_mass[1] - self.pygame_window_size[1] // 2)

        self.center_of_bodies_string_var.set(f"Centre of bodies: {self.center_of_bodies}")
        self.center_of_mass_string_var.set(f"Centre of mass: {self.center_of_mass}")

    def _update_tracking_label(self):
        self.tracking_string_var.set(f"Tracking: {self.tracking_int_to_name[self.tracking_int_var.get()]}")

    def _update_track_body_at_index(self):
        try:
            index = int(self.track_body_at_index_spinbox.get())
            if index >= self.simulation.num_bodies:
                self.track_body_at_index_spinbox.delete(0, tkinter.END)
                self.track_body_at_index_spinbox.insert(0, "-1")
        except ValueError:
            self.track_body_at_index_spinbox.delete(0, tkinter.END)
            self.track_body_at_index_spinbox.insert(0, "-1")

        self.track_body_at_index_spinbox.config(to=self.simulation.num_bodies)

    def pygame_loop_once(self):
        for event in pygame.event.get():
            if event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1:
                    self.mouse_pressed = True
                    # print("LMB Down")

            if event.type == pygame.MOUSEBUTTONUP:
                if event.button == 1:
                    self.mouse_pressed = False
                    # print("LMB Up")

            if event.type == pygame.MOUSEMOTION:
                if self.mouse_pressed and self.tracking_int_var.get() == -1:
                    self.offset[0] += event.rel[0]
                    self.offset[1] += event.rel[1]
                    # print(f"Movement: {event.rel}")

        if self.tracking_int_var.get() == 0:
            # self.offset = self.center_of_bodies
            self.offset[0] = -self.center_of_bodies[0]
            self.offset[1] = -self.center_of_bodies[1]

        elif self.tracking_int_var.get() == 1:
            try:
                index = int(self.track_body_at_index_spinbox.get())
            except ValueError:
                index = -1

            if index == -1:
                self.tracking_int_var.set(-1)
            else:
                coords = self.simulation.get_coordinates_of(index)

                self.offset[0] = -round(coords[0] - self.pygame_window_size[0] // 2)
                self.offset[1] = -round(coords[1] - self.pygame_window_size[1] // 2)

        self.screen.fill((0, 0, 0))

        self.simulation.step(0.01, 0.1)

        self._update_num_bodies(self.simulation.num_bodies)
        self._update_offset()
        self._update_center_of_mass_and_bodies()
        self._update_tracking_label()
        self._update_track_body_at_index()

        for i in range(self.simulation.num_bodies):
            coords = list(self.simulation.get_coordinates_of(i))
            coords[0] += self.offset[0]
            coords[1] += self.offset[1]

            colour = (255, 255, 255)

            if self.tracking_int_var.get() == 1:
                if i == int(self.track_body_at_index_spinbox.get()):
                    colour = (255, 0, 255)

            if i == int(self.track_body_at_index_spinbox.get()):
                colour = (255, 0, 0)

            pygame.draw.circle(
                self.screen,
                # (255, 255, 255),
                colour,
                coords,
                self.simulation[i].size
            )

        pygame.display.update()

    def mainloop(self, n=0):
        if n:
            for _ in range(n):
                self.update()
                self.pygame_loop_once()

        else:
            while True:
                start = time.perf_counter_ns()

                try:
                    self.winfo_exists()
                    self.update()
                    self.pygame_loop_once()

                except tkinter.TclError:
                    break

                duration = time.perf_counter_ns() - start
                self.fps_string_var.set(f"FPS: {round(1_000_000_000 / duration)}")


def main():
    window = SimulationHandlerInWindow(simulation_num_bodies=1200)
    window.mainloop()

    print("Done de done")


if __name__ == "__main__":
    main()
