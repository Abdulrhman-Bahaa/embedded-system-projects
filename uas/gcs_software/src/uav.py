import os
os.environ["BROWSER"] = "chrome"
from vpython import *
import numpy as np

def propeller(pos=vec(0, 0, 0), propeller_length=0.5, rotation=1):  
    # Objects
    part1 = sphere(pos=pos, radius=0.025 * propeller_length, color=color.black)
    part2 = ellipsoid(pos=vec(pos.x + (propeller_length / 4) + part1.radius - 0.01 , pos.y, pos.z) , size=vec((propeller_length/ 2), 0.01 * propeller_length, (propeller_length/ 2) / 5.5),  color=color.black)
    part2.rotate(angle=radians(10 * rotation))
    part3 = ellipsoid(pos=vec(pos.x - (propeller_length / 4) - part1.radius + 0.01, pos.y, pos.z) , size=vec((propeller_length/ 2), 0.01 * propeller_length, (propeller_length/ 2) / 5.5),  color=color.black)
    part3.rotate(angle=radians(-10 * rotation))
    return compound([part1, part2, part3] , origin=pos)


# Classes
class Drone:
    def __init__(self, propellers_number=4, propellers_length=0.55, frame_length=2 * 0.55, frame_thickness=0.05, motor_shaft_length = 0.25, motor_height=0.2, show_labels=True):
        # Variables
        self.__propellers_number = propellers_number
        self.__motor_shaft_length = motor_shaft_length
        self.__frame_length = frame_length
        self.__arms_number = int(propellers_number / 2)
        self.__frame_to_frame_angle = (pi * 2) / propellers_number
        self.__propeller_y = motor_shaft_length
        propeller_rotation = -1

        self.__static_parts = []
        self.dynamic_parts = []
        self.propellers_labels = []
        self.show_labels = show_labels

        self.psi = 0
        self.theta = 0
        self.phi = 0

        self.__prev_psi = 0
        self.__prev_theta = 0
        self.__prev_phi = 0

        # Objects
        self.__center = box(pos=vec(0, 0, 0), size=vec(propellers_length, frame_thickness * 3, propellers_length / 2), color=color.gray(0.5))
        self.__camera_holder = box(pos=vec(propellers_length / 2.2, -0.05, 0), size=vec(propellers_length / 5, 0.1, 0.15), axis=vec(1, 0, 0), color=color.black)
        self.__camera = sphere(pos=vec(propellers_length / 2, -0.05, 0), axis=vec(1, 0, 0), radius=propellers_length / 10, color=color.gray(0.3))
        self.__static_parts.append(self.__center)
        self.__static_parts.append(self.__camera_holder)
        self.__static_parts.append(self.__camera)

        for i in range(self.__arms_number):
            # Variables
            self.__frame_to_x_angle = (1 + (i * 2)) * (self.__frame_to_frame_angle / 2)
            self.__motor_shaft_x = ( self.__frame_length / 2) * cos(self.__frame_to_x_angle)
            self.__motor_shaft_z = ( self.__frame_length / 2) * sin(self.__frame_to_x_angle)
            propeller_rotation = propeller_rotation * -1

            # Objects
            # Drone arms
            self.__created_arm = box(pos=vec(0, 0, 0), axis=vec(cos(self.__frame_to_x_angle), 0, sin(self.__frame_to_x_angle)),size=vec( self.__frame_length, frame_thickness, frame_thickness), color=color.gray(0.5))
            self.__static_parts.append(self.__created_arm)

            # Motor
            self.__motor1 = cylinder(pos=vec(self.__motor_shaft_x, 0, self.__motor_shaft_z), axis = vec(0, 1, 0), size=vec(motor_height, 1, 0.1), radius=0.05, color=color.gray(0.5))
            self.__motor2 = cylinder(pos=vec(-self.__motor_shaft_x, 0, -self.__motor_shaft_z), axis = vec(0, 1, 0), size=vec(motor_height, 1, 0.1), radius=0.05, color=color.gray(0.5))
            self.__static_parts.append(self.__motor1)
            self.__static_parts.append(self.__motor2)

            # Motor shaft
            self.__shaft1 = cylinder(pos=vec( self.__motor_shaft_x, 0, self.__motor_shaft_z), axis=vec(0, 1, 0), length=motor_shaft_length,radius=0.001, color=color.black)
            self.__shaft2 = cylinder(pos=vec( -self.__motor_shaft_x, 0, -self.__motor_shaft_z), axis=vec(0, 1, 0), length=motor_shaft_length, radius=0.001, color=color.black)
            self.__static_parts.append(self.__shaft1)
            self.__static_parts.append(self.__shaft2)

            # Motor base
            self.__motor_base1 = box(pos=vec(self.__motor_shaft_x, 0, self.__motor_shaft_z), axis=vec(0, -1, 0), size=vec(0.3, 0.03, 0.05), color=color.gray(0.5))
            self.__motor_base2 = box(pos=vec(-self.__motor_shaft_x, 0, -self.__motor_shaft_z), axis=vec(0, -1, 0), size=vec(0.3, 0.03, 0.05), color=color.gray(0.5))
            self.__motor_base1.rotate(angle=self.__frame_to_x_angle)
            self.__motor_base2.rotate(angle=self.__frame_to_x_angle)
            self.__static_parts.append(self.__motor_base1)
            self.__static_parts.append(self.__motor_base2)

            # Propellers
            propeller1 = propeller(pos=vec(self.__motor_shaft_x, self.__propeller_y, self.__motor_shaft_z), propeller_length=propellers_length, rotation=propeller_rotation)
            propeller2 = propeller(pos=vec(-self.__motor_shaft_x, self.__propeller_y, -self.__motor_shaft_z), propeller_length=propellers_length, rotation=propeller_rotation)

            if self.show_labels == True:
                propeller1_label = label(pos=vec(self.__motor_shaft_x, self.__propeller_y - 0.1, self.__motor_shaft_z), text='1', xoffset=20, yoffset=50, space=30, height=16, border=4, font='sans')
                propeller2_label = label(pos=vec(-self.__motor_shaft_x, self.__propeller_y - 0.1, -self.__motor_shaft_z), text='1', xoffset=20, yoffset=50, space=30, height=16, border=4, font='sans')
                self.propellers_labels.append(propeller1_label)
                self.propellers_labels.append(propeller2_label)

            self.dynamic_parts.append(propeller1)
            self.dynamic_parts.append(propeller2)

        self.__drone = compound(self.__static_parts, origin=vec(0, 0, 0))


    @property
    def pos(self): 
        return self.__drone.pos 

    @pos.setter
    def pos(self, pos):
        self.__drone.pos = pos
        # Propellers
        self.__propeller_y = pos.y + self.__motor_shaft_length
        for i in range(self.__propellers_number):
            self.__frame_to_x_angle = (1 + (i * 2)) * (self.__frame_to_frame_angle / 2)
            self.__propeller_x = ((self.__frame_length / 2) * cos(self.__frame_to_x_angle)) + pos.x
            self.__propeller_z = ((self.__frame_length / 2) * sin(self.__frame_to_x_angle)) + pos.z

            self.dynamic_parts[i].pos = vec(self.__propeller_x, self.__propeller_y, self.__propeller_z)
            if self.show_labels == True:
                self.propellers_labels[i].pos = vec(self.__propeller_x, self.__propeller_y - 0.1 , self.__propeller_z)

    def rotate_propellers(self):
        for i in range(self.__propellers_number):
            self.dynamic_parts[i].rotate(axis=self.dynamic_parts[i].up, angle=degrees(0.0005), origin=self.dynamic_parts[i].origin)

    def yaw(self, degree, cumulate, psi):
        if (degree == 1):
            psi = radians(psi)

        k = vec(0, 1, 0)
        steps = psi - self.__prev_psi

        self.__drone.rotate(axis=k, angle=steps, origin=self.__drone.origin)
        for i in range(self.__propellers_number):
            self.dynamic_parts[i].rotate(axis=k, angle=steps, origin=self.__drone.origin)   
            if self.show_labels == True:
                self.propellers_labels[i].rotate(axis=k, angle=steps, origin=self.__drone.origin)   

        self.__prev_psi = psi
        self.psi = psi
        self.rotate_propellers()

    def pitch(self, degree, cumulate, theta):
        if (degree == 1):
            theta = radians(theta)
        
        k = cross(self.__drone.axis, self.__drone.up)
        if (cumulate == 0):
            steps = theta - self.__prev_theta
            self.theta = theta
        else:
            steps = theta
            self.theta = theta + self.theta

        self.__drone.rotate(axis=k, angle=steps, origin=self.__drone.origin)
        for i in range(self.__propellers_number):
            self.dynamic_parts[i].rotate(axis=k, angle=steps, origin=self.__drone.origin)
            if self.show_labels == True:
                self.propellers_labels[i].rotate(axis=k, angle=steps, origin=self.__drone.origin)

        self.__prev_theta = theta
        self.rotate_propellers()

    def roll(self, degree, cumulate, phi):
        if (degree == 1):
            phi = radians(phi)

        k = self.__drone.axis
        steps = phi - self.__prev_phi

        self.__drone.rotate(axis=k, angle=steps , origin=self.__drone.origin)
        for i in range(self.__propellers_number):
            self.dynamic_parts[i].rotate(axis=k, angle=steps , origin=self.__drone.origin)  
            if self.show_labels == True:
                self.propellers_labels[i].rotate(axis=k, angle=steps, origin=self.__drone.origin) 

        self.__prev_phi = phi
        self.theta = phi
        self.rotate_propellers()

