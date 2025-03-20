# Imports ------------------------------------------------------------------
import serial, time
from uav import *
import vpython as vp


# Variables Definitions ----------------------------------------------------
# Serial
# PORT = '/dev/rfcomm0'
PORT = '/dev/tnt1'
BAUD_RATE = 57600
SERIAL = False

# Drone
UAV_MIN_ALTITUDE = 0.148
UAV_ALTITUDE = 1

# Code
stop = 0

# psi, theta, phi, proportional_term, integral_term, derivative_term. pwm0, pwm1, pwm2, pwm3
data_from_uav = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

# Spin motors(1)/Stop motors(0), kp, ki, kd, ka, sp
data_to_uav = [0, 0, 0, 0, 0, 0]

input_text = ['Spin motors', 'Stop motors', 'Kp', 'Ki', 'Kd', 'Ka', 'SP']

sp = 0

# Functions ------------------------------------------------------------------
def send_data(data):
    data += "\n"
    ser.write(data.encode())

def interrupt_handler(evt):
    if evt.id < 8:
        global sp;
        data_to_uav[evt.id] = evt.number
        sp = data_to_uav[5]
        send_data(','.join(map(str, data_to_uav)))

def keyInput(evt):
    if evt.key == "s":
        global stop
        stop = 1

def data_visualization():
    current_time = time.time() - start_time

    # Animate the drone orientation
    uav1.roll(1, 0, -data_from_uav[2])  

    # Propellers speed labels
    propeller1_speed_label.text = str((data_from_uav[6] / 256) * 100) + '%'
    propeller2_speed_label.text = str((data_from_uav[7] / 256) * 100) + '%'
    propeller3_speed_label.text = str((data_from_uav[8] / 256) * 100) + '%'
    propeller4_speed_label.text = str((data_from_uav[9] / 256) * 100) + '%'

    # Text output
    state_variables_wtext.text=' |\tError = {}°\tPsi = {}°\tTheta = {}°\tPhi = {}°\tAltitude = {}m'.format(sp - data_from_uav[2], data_from_uav[0], data_from_uav[1], data_from_uav[2], 0)

    # Ploting on graphs
    if (round(current_time) % 60) != 0 :
        phi_sp_curve.plot(current_time, sp)
        phi_pv_curve.plot(current_time, data_from_uav[2])
        phi_control_curve.plot(current_time, data_from_uav[3] + data_from_uav[4] + data_from_uav[5])
    else :
        phi_sp_curve.delete()
        phi_pv_curve.delete()
        phi_control_curve.delete()
        phi_sp_curve.data = []
        phi_pv_curve.data = []
        phi_control_curve.data = []


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
# Buttons ------------------------
for i in range(0, 2):
    button(bind=interrupt_handler, text=input_text[i], number=1 - i, id=0)
    scene.append_to_caption('\t')

# Text inputs --------------------
for i in range(2, 7):
    scene.append_to_caption(' ')
    wtext(text=input_text[i] + ' = ')
    winput(bind=interrupt_handler, type='numeric', id=i - 1)

# Text outputs -------------------
scene.append_to_caption('\t')
state_variables_wtext = wtext()

# Ground plane -------------------
ground = box(
    pos=vector(0, -0.05, 0),  # Position slightly below the origin
    size=vector(100, 0.1, 100),  # Length (x), height (y), width (z)
    color=color.green,  # Choose a color (e.g., green for grass)
    texture=textures.rough  # Optional: Add a texture (e.g., wood, gravel)
)

# The drone body -----------------
uav1 = Drone()
uav1.pos = vec(0, UAV_MIN_ALTITUDE + UAV_ALTITUDE, 0)

# motors speed labels ------------
propeller1_speed_label = label(pos=vec(0.4, UAV_MIN_ALTITUDE + 1.1, 0.4), text='1', xoffset=20, yoffset=50, space=30, height=16, border=4, font='sans')
propeller2_speed_label = label(pos=vec(-0.4, UAV_MIN_ALTITUDE + 1.1, 0.4), text='2', xoffset=20, yoffset=50, space=30, height=16, border=4, font='sans')
propeller3_speed_label = label(pos=vec(-0.4, UAV_MIN_ALTITUDE + 1.1, -0.4), text='3', xoffset=20, yoffset=50, space=30, height=16, border=4, font='sans')
propeller4_speed_label = label(pos=vec(0.4, UAV_MIN_ALTITUDE + 1.1, -0.4), text='4', xoffset=20, yoffset=50, space=30, height=16, border=4, font='sans')

# Graphs -------------------------
graph1 = graph(title='Process variable φ(t)', align='left', xtitle='Time(ms)', fast=True, ytitle='Angle(°)',xmin=0, xmax=30, height=440, width=800, scroll=True)
graph2 = graph(title='Control signal u(t)', align='right', xtitle='Time(ms)', fast=True, xmin=0, xmax=30, height=440, width=800, scroll=True)

# Curves -------------------------
phi_pv_curve = gcurve(color=color.green, label='φ(t)', graph=graph1)
phi_sp_curve = gcurve(color=color.black, label='sp', graph=graph1)
phi_control_curve = gcurve(color=color.blue, label='u(t)', graph=graph2)


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
                data_from_uav = [float(x) for x in data]  # Convert all to integers
                data_visualization()
            except Exception as er:
                pass
else :
    start_time = time.time()
    while stop == 0:
        pass


# If the interpreter comes here, close the bowser tab
vp.scene.append_to_caption('<script>window.close();</script>')

 













