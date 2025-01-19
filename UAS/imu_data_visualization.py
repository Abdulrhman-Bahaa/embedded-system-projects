from vpython import *
from math import sin, cos, radians
import numpy
import serial




ser = serial.Serial('/dev/ttyACM0', 9600)  # 9600 baud rate, adjust timeout as necessary


scene.autoscale = False
scene.range = 1



# Positive psi make it rotate right
def yaw(obj, degree, psi):
    if (degree == 1):
        psi = radians(psi)
    obj.axis = vector(1, 0, 0)
    obj.axis = ((obj.axis) * cos(psi)) + (cross(obj.axis, obj.up) * sin(psi))

# Positive theta make it tilt forward
def pitch(obj, degree, theta):
    if (degree == 1):
        theta = radians(theta)
    nexAxisVector = cross(obj.axis, vector(0, 1, 0))
    obj.axis = ((obj.axis) * cos(theta)) + (cross(obj.axis, vector(0, 0, 1)) * sin(theta))

# Positive phi make it tilt right
def roll(obj, degree, phi):
    if (degree == 1):
        phi = radians(phi)
    obj.up = ((obj.up) * cos(phi)) + (cross(obj.axis, obj.up) * sin(phi))
    
        
# Quadcopter frame
frame_length = 2 
frame_thickness = 0.05
        
        
ref_axis_x = arrow(pos=vector(0, 0, 0), axis=vector(1, 0, 0), color=color.red)
ref_axis_y = arrow(pos=vector(0, 0, 0), axis=vector(0, 1, 0), color=color.green)
ref_axis_z = arrow(pos=vector(0, 0, 0), axis=vector(0, 0, 1), color=color.blue)

 
local_axis_x = arrow(pos=vector(0, 0, 0), axis=vector(1, 0, 0), color=color.red, shaftwidth=0.05)
local_axis_y = arrow(pos=vector(0, 0, 0), axis=vector(0, 1, 0), color=color.green, shaftwidth=0.05)
local_axis_z = arrow(pos=vector(0, 0, 0), axis=vector(0, 0, 1), color=color.blue, shaftwidth=0.05)


# The drone center, align with y axis
center = box(pos=vector(0, 0, 0), axis = vector(0, 0, 0), size=vector(0.2, 0.2, 0.2), color=color.white)
# The drone head
head = cone(pos=vector(0.1, 0, 0), axis = vector(1, 0, 0), size=vector(0.2, 0.2, 0.2), color=color.red)
# Arm 1, second quarter
arm1 = box(pos=vector(0, 0, 0), axis = vector(1, 0, -1), size=vector(frame_length, frame_thickness, frame_thickness), color=color.white)
# Arm 2, first quarter
arm2 = box(pos=vector(0, 0, 0), axis = vector(1, 0, 1), size=vector(frame_length, frame_thickness, frame_thickness), color=color.white)

# Create a group
drone = compound( [local_axis_x, local_axis_y, local_axis_z, center, head, arm1, arm2], origin=vector(0, 0, 0))
drone.axis = vector(1, 0, 0)
drone.up = vector(0, 1, 0)
prev = 0

ser.reset_input_buffer()  # Clears buffered data
sleep(2)  # Give time for the Arduino to stabilize
prev = 0
while True:
    # Read data from MPU6050 (ensure data format is: psi,theta,phi)
    line = ser.readline().decode('utf-8').strip()
    psi, theta, phi = map(float, line.split(','))
    psi = radians(psi)
    theta = radians(theta)
    phi = radians(phi)
    print(psi, theta, phi)
    # Update the object's orientation
    drone.axis = vector(cos(theta) * cos(psi), -sin(theta), -sin(psi) * cos(theta))
    v = vector(drone.axis.z, 0, -drone.axis.x)
    a = cross(drone.axis, v)
    drone.up = ((a) * cos(phi)) + (cross(a, drone.axis) * sin(phi))


   



