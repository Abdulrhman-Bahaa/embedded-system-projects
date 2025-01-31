from uav import *
import serial

# Serial communication
PORT = '/dev/ttyACM0'
BAUD_RATE = 9600

# Configure the scene
scene.autoscale = False 
scene.range = 1.5
scene.width = 1850
scene.height = 900
scene.background=color.cyan

# The global reference frame        
global_axis_x = arrow(pos=vec(0, 0, 0), axis=vec(1, 0, 0), color=color.red, shaftwidth=0.05)
global_axis_y = arrow(pos=vec(0, 0, 0), axis=vec(0, 1, 0), color=color.green, shaftwidth=0.05)
global_axis_z = arrow(pos=vec(0, 0, 0), axis=vec(0, 0, 1), color=color.blue, shaftwidth=0.05)

# Create Drone
uav1 = Drone(propellers_number=4, propellers_length=0.55)

# Initiate serial connection
ser = serial.Serial(port=PORT, baudrate=BAUD_RATE)

# Consider first 20 lines as noise
for i in range(20):
    ser.readline()

while True:
    # Read euler angles (format: psi,theta,phi\n)
    try:
        line = ser.readline().decode('utf-8').strip()
        psi, theta, phi = map(float, line.split(','))
    except ValueError:
        print(f"Could not convert to float.")

    # Rotate propellers
    for i in range(4):
        uav1.dynamic_parts[i].rotate(angle=radians(20), axis=uav1.dynamic_parts[i].up)

    # Perform rotations
    uav1.yaw(1, psi)
    uav1.pitch(1, -theta)
    uav1.roll(1, -phi)
