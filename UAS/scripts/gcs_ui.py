# Imports ------------------------------------------------------------------
import serial, time
from uav import *
import pygame

# Variables Definitions ----------------------------------------------------
# Data -------------------------
bluetooth_port = '/dev/rfcomm0'
simulation_port = '/dev/tnt1'
arduino_board_port = '/dev/ttyACM0'
PORT = simulation_port
BAUD_RATE = 57600
SERIAL = True
JOYSTICK_INPUT = False
running = True

data_from_uav = {
    'psi': 0,
    'theta': 0,
    'phi': 0,
    'proportional_term': 0,
    'integral_term': 0,
    'derivative_term': 0,
    'pwm0': 0,
    'pwm1': 0,
    'pwm2': 0,
    'pwm3': 0,
    'debug0': 0,
    'debug1': 0,
    'debug2': 0
}

data_to_uav = {
    'motors_state': 0,
    'kp': 0,
    'ki': 0,
    'kd': 0,
    'ka': 0,
    'phi_sp': 0
}

phi_sp = data_to_uav['phi_sp']

# Min and max phi angle
new_range = (-20, 20)

# Vpython -------------------------
# Drone
UAV_MIN_ALTITUDE = 0.148
UAV_ALTITUDE = 1
UAV_PROPELLERS_NUM = 4

# Values must be put in 'wtext_text_function' function
wtext_text = '\t| Psi = {}°\t| Theta = {}°\t| Phi = {}°\t| Phi sp = {}°\t| Phi error = {}°\t| Debug 0 = {}\t| Debug 1 = {}\t| Debug 2 = {}'

graphs_xmax = 30

# Not configurable variables
old_range = (-1, 1)
xmax_reached_num = 0
scale_time = 0
run_engine_counter = 0

# Functions ------------------------------------------------------------------
def wtext_text_function():
    return [data_from_uav['psi'], data_from_uav['theta'], data_from_uav['phi'], phi_sp, phi_sp -  data_from_uav['phi'], data_from_uav['debug0'], data_from_uav['debug1'], data_from_uav['debug2']]

def send_data(data):
    data += '\n'
    ser.write(data.encode())

def send_to_uav(evt):
    global phi_sp
    data_to_uav[evt.id] = evt.number
    phi_sp = data_to_uav['phi_sp']
    send_data(','.join(map(str, data_to_uav.values())))

def keyInput(evt):
    if evt.key == 's':
        global running
        running = False

def scale_value(x, old_min, old_max, new_min, new_max):
    return ((x - old_min) / (old_max - old_min)) * (new_max - new_min) + new_min

def joystick_init():
    pygame.init()
    # Detect joysticks
    pygame.joystick.init()
    joystick_count = pygame.joystick.get_count()
    joystick = pygame.joystick.Joystick(0)
    joystick.init()

def joystick_data(data_to_uav):
    for event in pygame.event.get():
        if event.type == pygame.JOYAXISMOTION:
            # Horizontal axis on the right stick (performing roll)
            if event.axis == 3:
                global phi_sp
                phi_sp = scale_value(event.value, *old_range, *new_range)
                data_to_uav['psi_sp'] = phi_sp
                send_data(','.join(map(str, data_to_uav)))
            # Vertical axis on the right stick 
            elif event.axis == 4:
                pass

def serial_data(data_from_uav):
    if ser.in_waiting > 0:  # Check if data is available
        # Read a line from Arduino
        try:
            line = ser.readline().decode('utf-8', errors='ignore').strip()  # Read and decode
            # Split by comma
            data = line.split(',')
            data = [float(x) for x in data]
            if len(data_from_uav) == len(data):
                i = 0
                for key, value in data_from_uav.items():
                    data_from_uav[key] = data[i]
                    i = i + 1
        except Exception as e:
            print(e)

def data_visualization(data_from_uav):
    global scale_time
    global xmax_reached_num
    current_time = time.time() - start_time

    # Text output
    state_variables_wtext.text = wtext_text.format(*wtext_text_function())

    # Animate the drone orientation
    uav1.roll(1, 0, data_from_uav['phi'])  

    # Propellers speed labels
    uav1.propellers_labels[0].text = str((data_from_uav['pwm0'] / 256) * 100) + '%'
    uav1.propellers_labels[1].text = str((data_from_uav['pwm1'] / 256) * 100) + '%'
    uav1.propellers_labels[2].text = str((data_from_uav['pwm2'] / 256) * 100) + '%'
    uav1.propellers_labels[3].text = str((data_from_uav['pwm3'] / 256) * 100) + '%'

    # Ploting on graphs
    # Check if scaling required
    if (round(current_time % graphs_xmax, 1) == 0) and ((current_time - scale_time) > 1) :
        scale_time = current_time
        phi_sp_curve.delete()
        phi_pv_curve.delete()
        phi_control_curve.delete()
        phi_proportional_term_curve.delete()
        phi_integral_term_curve.delete()
        phi_derivative_term_curve.delete()
        phi_sp_curve.data = []
        phi_pv_curve.data = []
        phi_control_curve.data = []
        phi_proportional_term_curve.data = []
        phi_integral_term_curve.data = []
        phi_derivative_term_curve.data = []

        graph1.xmin = graphs_xmax + (graphs_xmax * xmax_reached_num)
        graph2.xmin = graphs_xmax + (graphs_xmax * xmax_reached_num)

        graph1.xmax = graph1.xmin + graphs_xmax
        graph2.xmax = graph2.xmin + graphs_xmax

        xmax_reached_num = xmax_reached_num + 1
    else:
        phi_sp_curve.plot(current_time, data_to_uav['phi_sp'])
        phi_pv_curve.plot(current_time, data_from_uav['phi'])
        phi_control_curve.plot(current_time, data_from_uav['proportional_term'] + data_from_uav['integral_term'] + data_from_uav['derivative_term'])
        phi_proportional_term_curve.plot(current_time, data_from_uav['proportional_term'])
        phi_integral_term_curve.plot(current_time, data_from_uav['integral_term'])
        phi_derivative_term_curve.plot(current_time, data_from_uav['derivative_term'])

# Vpython scene configurations -----------------------------------------------------
scene.width = 1900
scene.height = 390
scene.background=color.cyan
scene.autoscale = False
scene.range = 1
scene.center = vec(0, UAV_MIN_ALTITUDE + 1, 0)
scene.forward = vec(1,  -0.5, 0)
scene.align = 'none'
scene.bind('keydown', keyInput)

# Vpython objects -----------------------------------------------------------------
# Ground plane
ground = box(
    pos=vector(0, -0.05, 0),  # Position slightly below the origin
    size=vector(100, 0.1, 100),  # Length (x), height (y), width (z)
    color=color.green,  # Choose a color (e.g., green for grass)
    texture=textures.rough  # Optional: Add a texture (e.g., wood, gravel)
)

# The drone body
uav1 = Drone(propellers_number=UAV_PROPELLERS_NUM)
uav1.pos = vec(0, UAV_MIN_ALTITUDE + UAV_ALTITUDE, 0)

scene.append_to_caption('\n\t')
# Inputs
for key, value in data_to_uav.items():
    scene.append_to_caption(' ')
    wtext(text=key + ' = ')
    winput(bind=send_to_uav, type='numeric', id=key)

# Outputs
scene.append_to_caption('\t')
state_variables_wtext = wtext()

scene.append_to_caption('\n' * 5)
# Graphs 
graph1 = graph(title='Process variable φ(t)', align='left', xtitle='Time(s)', fast=False, ytitle='Angle(°)',xmin=0, xmax=graphs_xmax, height=440, width=800, scroll=False)
graph2 = graph(title='Control signal u(t)', align='right', xtitle='Time(s)', fast=False, xmin=0, xmax=graphs_xmax, height=440, width=800, scroll=False)

# Curves
phi_pv_curve = gcurve(color=color.green, label='φ(t)', graph=graph1)
phi_sp_curve = gcurve(color=color.black, label='sp', graph=graph1)
phi_control_curve = gcurve(color=color.blue, label='u(t)', graph=graph2)
phi_proportional_term_curve = gcurve(color=color.cyan, label='P', graph=graph2)
phi_integral_term_curve = gcurve(color=color.magenta, label='I', graph=graph2)
phi_derivative_term_curve = gcurve(color=color.purple, label='D', graph=graph2)

# Initialization -----------------------------------------------------------------
if SERIAL == True:
    ser = serial.Serial(PORT, BAUD_RATE, timeout=1)  # Adjust COM port as needed

if JOYSTICK_INPUT == True:
    joystick_init()

start_time = time.time()
# Main while loop -----------------------------------------------------------------
while running == True:
    if JOYSTICK_INPUT == True:
        joystick_data(data_to_uav)
    if SERIAL == True:
        serial_data(data_from_uav)
        data_visualization(data_from_uav)

# If the interpreter comes here, close the bowser tab
scene.append_to_caption('<script>window.close();</script>')

 













