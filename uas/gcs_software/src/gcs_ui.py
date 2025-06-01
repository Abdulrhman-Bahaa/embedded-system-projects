# Imports ---------------------------------------------------------------------
import gcs_backend
import time
import uav
from vpython import *
import pygame


# Variables Definitions -------------------------------------------------------

# Min and max phi angle
old_range = (-1, 1)
new_range = (-40, 40)


# Functions -------------------------------------------------------------------

def scale_value(x, old_min, old_max, new_min, new_max):
    return ((x - old_min) / (old_max - old_min)) * (new_max - new_min) + new_min


def do_nothing(evt):
    pass


# Classes ---------------------------------------------------------------------

class DroneSimulation:
    def __init__(self, ser=None, joystick_input=False):

        if ser is None:
            RuntimeError('No serial provided, cannot send data to UAV')

        self.ser = ser
        self.graphs_xmax = 30
        self.drone_3d_model_min_altitude = 0.148
        self.first_visualization = True
        self.visualization_start_time = 0
        self.visualization_current_time = 0
        self.scale_time = 0
        self.xmax_reached_num = 0
        self.joystick_input = joystick_input
        self.send_data = False
        self.winput_values = []
        self.controllers_menu = None
        self.data_from_uav = gcs_backend.DataFromUAV()
        self.data_to_uav = gcs_backend.DataToUAV()
        self.selected_controller_object = None
        self.btns = []
        self.wts = []
        self.sliders = []
        self.running = True

        self.pid_winputs = {
            'kp': None,
            'ki': None,
            'kd': None,
            'ka': None
        }

        scene.bind('keydown', self.evt_handler)

        # Create ground
        self.ground = box(
            pos=vector(0, -0.05, 0),
            size=vector(100, 0.1, 100),
            color=color.green,
            texture=textures.rough
        )

        self.drone_3d_model = uav.Drone()
        self.drone_3d_model.pos = vec(
            0, self.drone_3d_model_min_altitude + 1, 0)

        # Scene configs
        scene.width = 1900
        scene.height = 390
        scene.background = color.cyan
        scene.autoscale = False
        scene.range = 1
        scene.center = vec(0, self.drone_3d_model_min_altitude + 1, 0)
        scene.forward = vec(1,  -0.5, 0)
        scene.align = 'none'

        # Create input fields
        self.create_inputs()

        # Create text output
        self.wtext_text = '\t| Psi = {}°\t| Theta = {}°\t| Phi = {}°\t| Debug 0 = {}\t| Debug 1 = {}\t| Debug 2 = {}'
        self.state_variables_wtext = wtext(
            text=self.wtext_text.format(0, 0, 0, 0, 0, 0))

        # Create graphs
        self.create_graphs()

    def evt_handler(self, evt):
        """Handle events from the UI."""

        if hasattr(evt, 'key') and evt.key == 'esc':
            self.running = False

        if hasattr(evt, 'id'):
            # Controllers menu selection
            if evt.id == 0:
                # Update selected controller object based on menu selection
                if self.controllers_menu.selected == 'yaw':
                    self.selected_controller_object = self.data_to_uav.yaw_controller
                elif self.controllers_menu.selected == 'pitch':
                    self.selected_controller_object = self.data_to_uav.pitch_controller
                elif self.controllers_menu.selected == 'roll':
                    self.selected_controller_object = self.data_to_uav.roll_controller
                else:
                    raise RuntimeError('Invalid controller selected')

                # Update input fields with selected controller values
                # PID inputs
                self.pid_winputs['kp'].text = self.selected_controller_object.kp
                self.pid_winputs['ki'].text = self.selected_controller_object.ki
                self.pid_winputs['kd'].text = self.selected_controller_object.kd
                self.pid_winputs['ka'].text = self.selected_controller_object.ka
                self.sliders[0].value = self.selected_controller_object.sp

            # Send Configs button
            if evt.id == 5:
                self.selected_controller_object.kp = 0 if self.pid_winputs[
                    'kp'].number is None else self.pid_winputs['kp'].number
                self.selected_controller_object.ki = 0 if self.pid_winputs[
                    'ki'].number is None else self.pid_winputs['ki'].number
                self.selected_controller_object.kd = 0 if self.pid_winputs[
                    'kd'].number is None else self.pid_winputs['kd'].number
                self.selected_controller_object.ka = 0 if self.pid_winputs[
                    'ka'].number is None else self.pid_winputs['ka'].number

                gcs_backend.send_to_uav(
                    self.ser, 'config', self.controllers_menu.selected, self.data_to_uav)

            # Setpoint slider
            if evt.id == 6:
                self.selected_controller_object.sp = evt.value

                self.wts[6].text = '{:1.1f}°'.format(evt.value)
                gcs_backend.send_to_uav(
                    self.ser, 'control', self.controllers_menu.selected, self.data_to_uav)

    def create_inputs(self):
        """Create user input fields for control variables."""
        inputs_number = 0

        self.wts.append(wtext(text=' ' + 'Selected controller' +
                              ' : ', id=inputs_number))
        self.controllers_menu = menu(
            bind=self.evt_handler, choices=['yaw', 'pitch', 'roll'], selected='yaw', id=inputs_number)
        self.selected_controller_object = self.data_to_uav.yaw_controller

        inputs_number = inputs_number + 1

        for key in self.pid_winputs.keys():
            self.wts.append(wtext(text=' ' + key + ' : ', id=inputs_number))
            self.pid_winputs[key] = winput(
                bind=do_nothing, type='numeric', id=inputs_number)
            inputs_number = inputs_number + 1

        scene.append_to_caption(' ')

        self.btns.append(button(bind=self.evt_handler,
                         text='Send Configs', id=inputs_number))
        inputs_number = inputs_number + 1

        self.wts.append(wtext(text=' ' + 'Setpoint' + ' : ', id=inputs_number))
        self.sliders.append(slider(bind=self.evt_handler, min=-90,
                                   max=90, id=inputs_number, value=0, step=1))
        inputs_number = inputs_number + 1

        self.wts.append(wtext(text='0.0°', id=inputs_number))

    def create_graphs(self):
        """Create graphs for the simulation."""
        scene.append_to_caption('\n' * 5)

        # Graphs
        self.graph1 = graph(title='Process variable', align='left',
                            xtitle='Time(s)', fast=False, ytitle='Angle(°)',
                            xmin=0, xmax=self.graphs_xmax, height=440, width=800, scroll=False)

        self.graph2 = graph(title='Control signal u(t)', align='right',
                            xtitle='Time(s)', fast=False,
                            xmin=0, xmax=self.graphs_xmax, height=440, width=800, scroll=False)

        # Curves
        self.pv_curve = gcurve(
            color=color.green, label='φ(t)', graph=self.graph1)
        self.sp_curve = gcurve(
            color=color.black, label='sp', graph=self.graph1)
        self.control_curve = gcurve(
            color=color.blue, label='u(t)', graph=self.graph2)
        self.proportional_term_curve = gcurve(
            color=color.cyan, label='P', graph=self.graph2)
        self.integral_term_curve = gcurve(
            color=color.magenta, label='I', graph=self.graph2)
        self.derivative_term_curve = gcurve(
            color=color.purple, label='D', graph=self.graph2)

    def receive_from_uav(self):
        gcs_backend.receive_from_uav(self.ser, self.data_from_uav)

    def joystick_data_handler(self):
        for event in pygame.event.get():
            if event.type == pygame.JOYAXISMOTION:
                # Horizontal axis on the right stick (performing roll)
                if event.axis == 3:
                    self.data_to_uav.phi_sp = scale_value(
                        event.value, *old_range, *new_range)
                    self.send_to_uav()

    def data_visualize(self):
        if self.first_visualization:
            self.visualization_start_time = time.time()
            self.first_visualization = False
        self.visualization_current_time = time.time() - self.visualization_start_time

        # Text output
        state_variables_wtext_values = [self.data_from_uav.psi, self.data_from_uav.theta,
                                        self.data_from_uav.phi, self.data_from_uav.debug0,
                                        self.data_from_uav.debug1, self.data_from_uav.debug2]
        self.state_variables_wtext.text = self.wtext_text.format(
            *state_variables_wtext_values)

        # Animate the drone orientation
        # self.drone_3d_model.yaw(1, 0, self.data_from_uav.psi)
        # self.drone_3d_model.pitch(1, 0, self.data_from_uav.theta)
        self.drone_3d_model.roll(1, 0, self.data_from_uav.phi)

        # Propellers speed labels
        self.drone_3d_model.propellers_labels[0].text = str(
            (self.data_from_uav.pwm0 / 256) * 100) + '%'
        self.drone_3d_model.propellers_labels[1].text = str(
            (self.data_from_uav.pwm1 / 256) * 100) + '%'
        self.drone_3d_model.propellers_labels[2].text = str(
            (self.data_from_uav.pwm2 / 256) * 100) + '%'
        self.drone_3d_model.propellers_labels[3].text = str(
            (self.data_from_uav.pwm3 / 256) * 100) + '%'

        Ploting on graphs
        Check if scaling required
        if (round(self.visualization_current_time % self.graphs_xmax, 1) == 0) and ((self.visualization_current_time - self.scale_time) > 1):
            self.scale_time = self.visualization_current_time
            self.sp_curve.delete()
            self.pv_curve.delete()
            self.control_curve.delete()
            self.proportional_term_curve.delete()
            self.integral_term_curve.delete()
            self.derivative_term_curve.delete()
            self.sp_curve.data = []
            self.pv_curve.data = []
            self.control_curve.data = []
            self.proportional_term_curve.data = []
            self.integral_term_curve.data = []
            self.derivative_term_curve.data = []

            self.graph1.xmin = self.graphs_xmax + \
                (self.graphs_xmax * self.xmax_reached_num)
            self.graph2.xmin = self.graphs_xmax + \
                (self.graphs_xmax * self.xmax_reached_num)

            self.graph1.xmax = self.graph1.xmin + self.graphs_xmax
            self.graph2.xmax = self.graph2.xmin + self.graphs_xmax

            self.xmax_reached_num = self.xmax_reached_num + 1
        else:

            if self.controllers_menu.selected == 'yaw':
                self.control_curve.plot(self.visualization_current_time,
                                        self.data_from_uav.yaw_proportional_term + self.data_from_uav.yaw_integral_term + self.data_from_uav.yaw_derivative_term)
                self.proportional_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.yaw_proportional_term)
                self.integral_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.yaw_integral_term)
                self.derivative_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.yaw_derivative_term)
                # self.sp_curve.plot(self.visualization_current_time,
                #                    self.data_to_uav.yaw_controller.sp)
                self.pv_curve.plot(
                    self.visualization_current_time, self.data_from_uav.psi)
                self.graph1.title = 'Psi angle ψ(t)'
                self.graph2.title = 'Yaw control signal u(t)'
            elif self.controllers_menu.selected == 'pitch':
                self.control_curve.plot(self.visualization_current_time,
                                        self.data_from_uav.pitch_proportional_term + self.data_from_uav.pitch_integral_term + self.data_from_uav.pitch_derivative_term)
                self.proportional_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.pitch_proportional_term)
                self.integral_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.pitch_integral_term)
                self.derivative_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.pitch_derivative_term)
                # self.sp_curve.plot(self.visualization_current_time,
                #                    self.data_to_uav.pitch_controller.sp)
                self.pv_curve.plot(
                    self.visualization_current_time, self.data_from_uav.theta)
                self.graph1.title = 'Theta angle θ(t)'
                self.graph2.title = 'Pitch control signal u(t)'
            elif self.controllers_menu.selected == 'roll':
                self.control_curve.plot(self.visualization_current_time,
                                        self.data_from_uav.roll_proportional_term + self.data_from_uav.roll_integral_term + self.data_from_uav.roll_derivative_term)
                self.proportional_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.roll_proportional_term)
                self.integral_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.roll_integral_term)
                self.derivative_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.roll_derivative_term)
                # self.sp_curve.plot(self.visualization_current_time,
                #                    self.data_to_uav.roll_controller.sp)
                self.pv_curve.plot(
                    self.visualization_current_time, self.data_from_uav.phi)
                self.graph1.title = 'Phi angle φ(t)'
                self.graph2.title = 'Roll control signal u(t)'
            else:
                pass

    def start(self):
        """Start the drone simulation."""
        while self.running:
            # Receive data from UAV
            self.receive_from_uav()
            # Handle joystick input if enabled
            if self.joystick_input:
                self.joystick_data_handler()
            # Visualize data
            self.data_visualize()
