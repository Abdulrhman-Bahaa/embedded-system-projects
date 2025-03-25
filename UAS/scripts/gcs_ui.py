# Imports ------------------------------------------------------------------
import serial, time
from uav import *

# Variables Definitions ----------------------------------------------------
# Serial communication
bluetooth_port = '/dev/rfcomm0'
simulation_port = '/dev/tnt1'
arduino_board_port = '/dev/ttyACM0'
PORT = simulation_port
BAUD_RATE = 57600
SERIAL = 1

# Drone
UAV_MIN_ALTITUDE = 0.148
UAV_ALTITUDE = 1
UAV_PROPELLERS_NUM = 4

# psi, theta, phi, proportional_term, integral_term, derivative_term. pwm0, pwm1, pwm2, pwm3, debug0, debug1, debug2
data_from_uav = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
# Values must be put in 'update_wtext_values' function
wtext_text = '\t| Psi = {}°\t| Theta = {}°\t| Phi = {}°\t| Phi sp = {}°\t| Phi error = {}°\t| Debug 0 = {}\t| Debug 1 = {}\t| Debug 2 = {}'

# Spin motors(1)/Stop motors(0), kp, ki, kd, ka, sp
data_to_uav = [0, 0, 0, 0, 0, 0]
input_text = ['Motors state', 'Kp', 'Ki', 'Kd', 'Ka', 'SP']

psi = data_from_uav[0]
theta = data_from_uav[1]
phi = data_from_uav[2]
phi_sp = data_to_uav[5]
debug0 = data_from_uav[10]
debug1 = data_from_uav[11]
debug2 = data_from_uav[12]

graphs_xmax = 60

# Not configurable variables
xmax_reached_num = 0
scale_time = 0
stop = 0

# Functions ------------------------------------------------------------------
def update_wtext_values():
    global psi, theta, phi
    psi = data_from_uav[0]
    theta = data_from_uav[1]
    phi = data_from_uav[2]
    debug0 = data_from_uav[10]
    debug1 = data_from_uav[11]
    debug2 = data_from_uav[12]
    return [psi, theta, phi, phi_sp, phi_sp - phi, debug0, debug1, debug2]

def send_data(data):
    data += "\n"
    ser.write(data.encode())

def send_to_uav(evt):
    global phi_sp
    data_to_uav[evt.id] = evt.number
    phi_sp = data_to_uav[5]
    send_data(','.join(map(str, data_to_uav)))

def keyInput(evt):
    if evt.key == "s":
        global stop
        stop = 1

def data_visualization():
    global scale_time
    global xmax_reached_num
    current_time = time.time() - start_time

    # Animate the drone orientation
    uav1.roll(1, 0, data_from_uav[2])  

    # # Propellers speed labels
    for i in range(0, UAV_PROPELLERS_NUM):
        uav1.propellers_labels[i].text = str((data_from_uav[i + 6] / 256) * 100) + '%'

    # Text output
    state_variables_wtext.text = wtext_text.format(*update_wtext_values())

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
        phi_sp_curve.plot(current_time, data_to_uav[5])
        phi_pv_curve.plot(current_time, round(data_from_uav[2], 1))
        phi_control_curve.plot(current_time, data_from_uav[3] + data_from_uav[4] + data_from_uav[5])
        phi_proportional_term_curve.plot(current_time, data_from_uav[3])
        phi_integral_term_curve.plot(current_time, data_from_uav[4])
        phi_derivative_term_curve.plot(current_time, data_from_uav[5])

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

# Inputs
for i in range(0, len(data_to_uav)):
    scene.append_to_caption(' ')
    wtext(text=input_text[i] + ' = ')
    winput(bind=send_to_uav, type='numeric', id=i)

# Outputs
scene.append_to_caption('\t')
state_variables_wtext = wtext()

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

# Main while loop -----------------------------------------------------------------
if SERIAL == 1:
    ser = serial.Serial(PORT, BAUD_RATE, timeout=1)  # Adjust COM port as needed
    start_time = time.time()
    while stop == 0:
        if ser.in_waiting > 0:  # Check if data is available
            # Read a line from Arduino
            try:
                line = ser.readline().decode('utf-8', errors='ignore').strip()  # Read and decode
                # Split by comma
                data = line.split(',')
                if (len(data) == len(data_from_uav)) :
                    data_from_uav = [float(x) for x in data]  # Convert all to integers
                    data_visualization()
            except Exception as e:
                print(e)
else :
    start_time = time.time()
    while stop == 0:
        pass

# If the interpreter comes here, close the bowser tab
scene.append_to_caption('<script>window.close();</script>')

 













