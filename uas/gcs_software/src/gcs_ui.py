# Imports ------------------------------------------------------------------
from dataclasses import dataclass, field, is_dataclass, fields
import serial
import time
from uav import *
import pygame

# Variables Definitions ----------------------------------------------------
# Min and max phi angle
new_range = (-40, 40)

# Not configurable variables
old_range = (-1, 1)

# Functions ------------------------------------------------------------------


def scale_value(x, old_min, old_max, new_min, new_max):
    return ((x - old_min) / (old_max - old_min)) * (new_max - new_min) + new_min


def iterate_dataclass(obj, data):
    for f in fields(obj):
        value = getattr(obj, f.name)
        if is_dataclass(value):
            iterate_dataclass(value, data)
        else:
            data.append(value)
    return data


def do_nothing(evt):
    pass

# Classes --------------------------------------------------------------------


@dataclass
class UAVData:
    psi: float = 0.0
    theta: float = 0.0
    phi: float = 0.0
    pwm0: int = 0
    pwm1: int = 0
    pwm2: int = 0
    pwm3: int = 0

    yaw_proportional_term: float = 0.0
    yaw_integral_term: float = 0.0
    yaw_derivative_term: float = 0.0
    pitch_proportional_term: float = 0.0
    pitch_integral_term: float = 0.0
    pitch_derivative_term: float = 0.0
    roll_proportional_term: float = 0.0
    roll_integral_term: float = 0.0
    roll_derivative_term: float = 0.0

    debug0: float = 0.0
    debug1: float = 0.0
    debug2: float = 0.0


@dataclass
class PidParameters:
    """ PID parameters for UAV control """
    """ kp: proportional gain
        ki: integral gain
        kd: derivative gain
        ka: ant-windup gain
        sp: setpoint (desired value)
    """
    kp: float = 0.0
    ki: float = 0.0
    kd: float = 0.0
    ka: float = 0.0
    sp: float = 0.0


@dataclass
class UAVCommand:
    motors_state: int = 0
    yaw_controller: PidParameters = field(default_factory=PidParameters)
    pitch_controller: PidParameters = field(default_factory=PidParameters)
    roll_controller: PidParameters = field(default_factory=PidParameters)


class DroneSimulation:
    def __init__(self, uav: Drone, data_from_uav: UAVData, data_to_uav: UAVCommand, graphs_xmax, ser=None, joystick_input=False):
        """Initialize the simulation with all VPython objects."""

        # Store external parameters
        self.data_to_uav = data_to_uav
        self.data_from_uav = data_from_uav
        self.data_to_uav_length = len(vars(data_to_uav))
        self.data_from_uav_length = len(vars(data_from_uav))
        self.graphs_xmax = graphs_xmax
        self.uav_min_altitude = 0.148
        self.first_visualization = True
        self.visualization_start_time = 0
        self.visualization_current_time = 0
        self.scale_time = 0
        self.xmax_reached_num = 0
        self.joystick_input = joystick_input
        self.send_data = False
        self.winput_values = []
        self.controllers_menu = None

        if ser is None:
            print("No serial provided")
        else:
            self.ser = ser

        # Scene configs
        self.scene_setup()

        # Create ground
        self.ground = box(
            pos=vector(0, -0.05, 0),
            size=vector(100, 0.1, 100),
            color=color.green,
            texture=textures.rough
        )

        # Create the drone 3d model
        self.uav = uav
        self.uav.pos = vec(0, self.uav_min_altitude + 1, 0)

        # Create input fields
        self.create_inputs()

        # Create text output
        self.wtext_text = '\t| Psi = {}°\t| Theta = {}°\t| Phi = {}°\t| Debug 0 = {}\t| Debug 1 = {}\t| Debug 2 = {}'
        self.state_variables_wtext = wtext(
            text=self.wtext_text.format(0, 0, 0, 0, 0, 0))

        # Create graphs
        self.create_graphs()

    def scene_setup(self):
        """Vpython scene configurations"""
        scene.width = 1900
        scene.height = 390
        scene.background = color.cyan
        scene.autoscale = False
        scene.range = 1
        scene.center = vec(0, self.uav_min_altitude + 1, 0)
        scene.forward = vec(1,  -0.5, 0)
        scene.align = 'none'

    def create_inputs(self):
        """Create user input fields for control variables."""
        controllers_choice_list = []
        scene.append_to_caption('\n\t')

        for key, value in vars(self.data_to_uav).items():
            if not is_dataclass(value):
                scene.append_to_caption(key + ' = ')
                self.winput_values.append(
                    winput(bind=do_nothing, type='numeric', id=key))
            else:
                controllers_choice_list.append(key)

        for key, value in vars(self.data_to_uav.pitch_controller).items():
            scene.append_to_caption(key + ' = ')
            self.winput_values.append(
                winput(bind=do_nothing, type='numeric', id=key))

        self.controllers_menu = menu(
            bind=do_nothing, choices=controllers_choice_list, index=0)

        scene.append_to_caption('\t')

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
        if self.ser.in_waiting > 0:  # Check if data is available
            # Read a line from Arduino
            try:
                line = self.ser.readline().decode(
                    'utf-8', errors='ignore').strip()  # Read and decode
                # Split by comma
                data = line.split(',')
                if data[0] == "monitor" and len(data) == self.data_from_uav_length + 1:
                    data = [float(x) for x in data if x not in ["monitor", ""]]
                    i = 0
                    for key, value in vars(self.data_from_uav).items():
                        setattr(self.data_from_uav, key, data[i])
                        i = i + 1
            except Exception as e:
                print(e)

    def send_to_uav(self):
        data = []
        data = iterate_dataclass(self.data_to_uav, data)
        data = ','.join(map(str, data))
        data += '\n'
        self.ser.write(data.encode())
        # print(data.encode())

    def vpython_user_input_handler(self):
        """Function to send user input values to the UAV."""
        setattr(self.data_to_uav, 'motors_state',
                0 if self.winput_values[0].number is None else self.winput_values[0].number)

        controller_selected = getattr(
            self.data_to_uav, str(self.controllers_menu.selected))

        controller_selected.kp = 0 if self.winput_values[
            1].number is None else self.winput_values[1].number
        controller_selected.ki = 0 if self.winput_values[
            2].number is None else self.winput_values[2].number
        controller_selected.kd = 0 if self.winput_values[
            3].number is None else self.winput_values[3].number
        controller_selected.ka = 0 if self.winput_values[
            4].number is None else self.winput_values[4].number
        controller_selected.sp = 0 if self.winput_values[
            5].number is None else self.winput_values[5].number

    def joystick_data_handler(self):
        for event in pygame.event.get():
            if event.type == pygame.JOYAXISMOTION:
                # Horizontal axis on the right stick (performing roll)
                if event.axis == 3:
                    self.data_to_uav.phi_sp = scale_value(
                        event.value, *old_range, *new_range)
                    self.send_to_uav()

    def gcs_interface(self):
        """ Responsible of getting data from uav and send commands to it"""
        if self.send_data:
            # Send data to uav
            self.vpython_user_input_handler()
            self.send_to_uav()
            self.send_data = False
        # Get data from uav
        if self.ser:
            self.receive_from_uav()
        # Send data to uav
        if self.joystick_input:
            self.joystick_data_handler()

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
        # self.uav.yaw(1, 0, self.data_from_uav.psi)
        # self.uav.pitch(1, 0, self.data_from_uav.theta)
        self.uav.roll(1, 0, self.data_from_uav.phi)

        # Propellers speed labels
        self.uav.propellers_labels[0].text = str(
            (self.data_from_uav.pwm0 / 256) * 100) + '%'
        self.uav.propellers_labels[1].text = str(
            (self.data_from_uav.pwm1 / 256) * 100) + '%'
        self.uav.propellers_labels[2].text = str(
            (self.data_from_uav.pwm2 / 256) * 100) + '%'
        self.uav.propellers_labels[3].text = str(
            (self.data_from_uav.pwm3 / 256) * 100) + '%'

        # Ploting on graphs
        # Check if scaling required
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

            if self.controllers_menu.selected == 'yaw_controller':
                self.control_curve.plot(self.visualization_current_time,
                                        self.data_from_uav.yaw_proportional_term + self.data_from_uav.yaw_integral_term + self.data_from_uav.yaw_derivative_term)
                self.proportional_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.yaw_proportional_term)
                self.integral_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.yaw_integral_term)
                self.derivative_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.yaw_derivative_term)
                self.sp_curve.plot(self.visualization_current_time,
                                   self.data_to_uav.yaw_controller.sp)
                self.pv_curve.plot(
                    self.visualization_current_time, self.data_from_uav.psi)
                self.graph1.title = 'Psi angle ψ(t)'
                self.graph2.title = 'Yaw control signal u(t)'
            elif self.controllers_menu.selected == 'pitch_controller':
                self.control_curve.plot(self.visualization_current_time,
                                        self.data_from_uav.pitch_proportional_term + self.data_from_uav.pitch_integral_term + self.data_from_uav.pitch_derivative_term)
                self.proportional_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.pitch_proportional_term)
                self.integral_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.pitch_integral_term)
                self.derivative_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.pitch_derivative_term)
                self.sp_curve.plot(self.visualization_current_time,
                                   self.data_to_uav.pitch_controller.sp)
                self.pv_curve.plot(
                    self.visualization_current_time, self.data_from_uav.theta)
                self.graph1.title = 'Theta angle θ(t)'
                self.graph2.title = 'Pitch control signal u(t)'
            elif self.controllers_menu.selected == 'roll_controller':
                self.control_curve.plot(self.visualization_current_time,
                                        self.data_from_uav.roll_proportional_term + self.data_from_uav.roll_integral_term + self.data_from_uav.roll_derivative_term)
                self.proportional_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.roll_proportional_term)
                self.integral_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.roll_integral_term)
                self.derivative_term_curve.plot(
                    self.visualization_current_time, self.data_from_uav.roll_derivative_term)
                self.sp_curve.plot(self.visualization_current_time,
                                   self.data_to_uav.roll_controller.sp)
                self.pv_curve.plot(
                    self.visualization_current_time, self.data_from_uav.phi)
                self.graph1.title = 'Phi angle φ(t)'
                self.graph2.title = 'Roll control signal u(t)'
            else:
                pass
