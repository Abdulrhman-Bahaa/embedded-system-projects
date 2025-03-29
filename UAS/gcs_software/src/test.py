import pygame
from uav import *

TEST = False
running = True


# Example usage
old_range = (-1, 1)
new_range = (-45, 45)

# Drone
UAV_MIN_ALTITUDE = 0.148
UAV_ALTITUDE = 1
UAV_PROPELLERS_NUM = 4

# Functions ------------------------------------------------------------------
def keyInput(evt):
    if evt.key == "s":
        global running
        running = False

def scale_value(x, old_min, old_max, new_min, new_max):
    return ((x - old_min) / (old_max - old_min)) * (new_max - new_min) + new_min

# def joystick_init():
#     # Initialize pygame
#     pygame.init()

#     # Detect joysticks
#     pygame.joystick.init()
#     joystick_count = pygame.joystick.get_count()

#     joystick = pygame.joystick.Joystick(0)
#     joystick.init()
#     print(f"Connected to: {joystick.get_name()}")

#     if joystick_count == 0:
#         print("No joystick detected!")
#     else:
#         joystick = pygame.joystick.Joystick(0)
#         joystick.init()
#         print(f"Connected to: {joystick.get_name()}")
joysticks = []  # Store joystick objects globally

def joystick_init():
    global joystick
    # Initialize pygame
    pygame.init()
    pygame.joystick.init()

    joystick_count = pygame.joystick.get_count()
    if joystick_count == 0:
        print("No joystick detected!")
    else:
        joystick = pygame.joystick.Joystick(0)
        joystick.init()
        print(f"Connected to: {joystick.get_name()}")
        # joystick = pygame.joystick.Joystick(0)


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

joystick_init()
while running:
    for event in pygame.event.get():
        if event.type == pygame.JOYAXISMOTION:
            if event.axis == 3:
                phi = scale_value(event.value, *old_range, *new_range)
                uav1.roll(1, 0, phi) 
            elif event.axis == 4:
                theta = scale_value(event.value, *old_range, *new_range)
                uav1.pitch(1, 0, theta)
            print(f"Axis {event.axis} moved to {event.value}")

scene.append_to_caption('<script>window.close();</script>')
