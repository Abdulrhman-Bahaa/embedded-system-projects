from vpython import *

# Classes
class Propeller:
    def __init__(self, pos=vec(0, 0, 0), propeller_length=0.5, rotation=1):
        """
        Initializes a Propeller instance.

        Parameters:
        pos (vector): The position of the propeller (default is vec(0, 0, 0)).
        propeller_length (float): The length of the propeller (default is 0.5, must be positive).
        rotation (int): The direction in which the propeller rotates (default is 1 (CW). -1 (CCW))

        Raises:
        TypeError: If pos is not a Vector instance.
        ValueError: If propeller_length is negative or rotation neither 1 nor -1.
        """
        # Validate pos
        if not isinstance(pos, vector):
            raise TypeError("pos must be an instance of vector.")

        # Validate propeller_length
        if not isinstance(propeller_length, (int, float)) or propeller_length <= 0:
            raise ValueError("propeller_length must be a positive number.")

        # Validate rotation
        if rotation not in (1, -1):
            raise ValueError("rotation must be 1 (CW) or -1 (CCW).")
        
        # Objects
        self.__part1 = sphere(pos=pos, radius=0.025 * propeller_length, color=color.black)
        self.__part2 = ellipsoid(pos=vec(pos.x + (propeller_length / 4) + self.__part1.radius - 0.01 , pos.y, pos.z) , size=vec((propeller_length/ 2), 0.01 * propeller_length, (propeller_length/ 2) / 5.5),  color=color.black)
        self.__part2.rotate(angle=radians(10 * rotation))
        self.__part3 = ellipsoid(pos=vec(pos.x - (propeller_length / 4) - self.__part1.radius + 0.01, pos.y, pos.z) , size=vec((propeller_length/ 2), 0.01 * propeller_length, (propeller_length/ 2) / 5.5),  color=color.black)
        self.__part3.rotate(angle=radians(-10 * rotation))
        self.__propeller = compound([self.__part1, self.__part2, self.__part3] , origin=pos)
    
    def move(self, pos=vec(0, 0, 0)):
        self.__propeller.pos = pos

    def rotate(self, degree, angle):
        if (degree == 1):
            angle = radians(angle)
        self.__propeller.rotate(angle=angle, axis=vec(0, 1, 0))
