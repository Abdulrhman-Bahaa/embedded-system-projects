# Imports ---------------------------------------------------------------------
from dataclasses import dataclass, field, asdict
from serial import Serial


# Data Types ------------------------------------------------------------------

@dataclass
class DataFromUAV:
    """Data structure to hold the data received from the UAV.

    Attributes:
        psi (float): Yaw angle in degrees.
        theta (float): Pitch angle in degrees.
        phi (float): Roll angle in degrees.
        pwm0 (int): PWM value for motor 0.
        pwm1 (int): PWM value for motor 1.
        pwm2 (int): PWM value for motor 2.
        pwm3 (int): PWM value for motor 3.
        yaw_proportional_term (float): Yaw proportional term for PID controller.
        yaw_integral_term (float): Yaw integral term for PID controller.
        yaw_derivative_term (float): Yaw derivative term for PID controller.
        pitch_proportional_term (float): Pitch proportional term for PID controller.
        pitch_integral_term (float): Pitch integral term for PID controller.
        pitch_derivative_term (float): Pitch derivative term for PID controller.
        roll_proportional_term (float): Roll proportional term for PID controller.
        roll_integral_term (float): Roll integral term for PID controller.
        roll_derivative_term (float): Roll derivative term for PID controller.
        debug0 (float): Debug variable 0.
        debug1 (float): Debug variable 1.
        debug2 (float): Debug variable 2.
    """
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
class PIDParameters:
    """Data structure to hold PID parameters for a controller.

    Attributes:
        kp (float): Proportional gain.
        ki (float): Integral gain.
        kd (float): Derivative gain.
        ka (float): Anti-windup gain.
        sp (float): Setpoint value.
    """
    kp: float = 0.0
    ki: float = 0.0
    kd: float = 0.0
    ka: float = 0.0
    sp: float = 0.0


@dataclass
class DataToUAV:
    """Data structure to hold the data to be sent to the UAV.

    Attributes:
        yaw_controller (PIDParameters): PID parameters for the yaw controller.
        pitch_controller (PIDParameters): PID parameters for the pitch controller.
        roll_controller (PIDParameters): PID parameters for the roll controller.
    """
    yaw_controller: PIDParameters = field(default_factory=PIDParameters)
    pitch_controller: PIDParameters = field(default_factory=PIDParameters)
    roll_controller: PIDParameters = field(default_factory=PIDParameters)


# Functions -------------------------------------------------------------------

def send_to_uav(ser: Serial, data_type: str, controller: str, data_to_uav: DataToUAV):
    """Sends data to the UAV via serial communication.

    This function sends either control data or configuration data for a specific controller
    (yaw, pitch, or roll) to the UAV. The data is sent in a specific format that the UAV can interpret.

    Args:
        ser (Serial): The serial connection to the UAV.
        data_type (str): The type of data to send ('control' or 'config').
        controller (str): The controller to send data for ('yaw', 'pitch', or 'roll').
        data_to_uav (DataToUAV): The data structure containing the PID parameters.

    Raises:
        RuntimeError: If the serial connection is not provided or if the controller or data type is invalid.
    """
    if ser is None:
        RuntimeError('No serial provided, cannot send data to UAV')
    else:

        if controller == 'yaw':
            controller_local = 0
            controller_object = data_to_uav.yaw_controller
        elif controller == 'pitch':
            controller_local = 1
            controller_object = data_to_uav.pitch_controller
        elif controller == 'roll':
            controller_local = 2
            controller_object = data_to_uav.roll_controller
        else:
            raise RuntimeError('Invalid controller')

        if data_type == 'control':
            data_type_local = 'l'
            data_local = [controller_object.sp]
        elif data_type == 'config':
            data_type_local = 'g'
            data_local = [controller_object.kp, controller_object.ki,
                          controller_object.kd, controller_object.ka]
        else:
            raise RuntimeError('Invalid data type')

        data_local = [data_type_local, controller_local,
                      ','.join(map(str, data_local))]
        data_local = ','.join(map(str, data_local)).encode()

        if ser.is_open and ser.writable() and ser.out_waiting == 0:
            try:
                ser.write(data_local + b'\n')
            except Exception as e:
                print("Serial error during click:", e)


def receive_from_uav(ser: Serial, data_from_uav: DataFromUAV):
    """Receives data from the UAV via serial communication.

    Args:
        ser (Serial): The serial connection to the UAV.
        data_from_uav (DataFromUAV): The data structure to populate with received data.

    Raises:
        RuntimeError: If the serial connection is not provided.
    """
    if ser is None:
        RuntimeError('No serial provided, cannot send data to UAV')
    else:
        if ser.in_waiting > 0:  # Check if data is available
            try:
                line = ser.readline().decode(
                    'utf-8', errors='ignore').strip()
                data = line.split(',')
                if data[0] == 'm' and len(data) == 20:
                    i = 1
                    for key in asdict(data_from_uav).keys():
                        if data[i] != '':
                            setattr(data_from_uav, key, float(data[i]))
                        i = i + 1
                else:
                    print(line)

            except Exception as e:
                print(e)
