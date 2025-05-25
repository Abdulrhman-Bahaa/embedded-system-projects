""" ---------- Main application for UAV ground control simulation  """
 
# Imports ------------------------------------------------------------
from gcs_ui import *

# Global Variables ---------------------------------------------------
running = True
simulation = None
joystick = None

# Main function ------------------------------------------------------
def main():
    application_initialize()    
    while running:
        simulation.gcs_interface()
        simulation.data_visualize()

# Functions ----------------------------------------------------------
def application_initialize():
    global simulation
    UAV_PROPELLERS_NUM = 4   
    PORTS = {
        "bluetooth": '/dev/rfcomm0',
        "simulation": '/dev/tnt1',
        "arduino": '/dev/ttyACM0'
    }
    BAUD_RATE = 57600
    SERIAL = True
    JOYSTICK_INPUT = False
    GRAPHS_XMAX = 30
    ser = None

    uav1 = Drone(propellers_number=UAV_PROPELLERS_NUM)

    data_from_uav = UAVData()
    data_to_uav = UAVCommand()

    if SERIAL:
        try:
            ser = serial.Serial(PORTS['simulation'], BAUD_RATE, timeout=1)
        except serial.SerialException as e:
            print(f"Serial connection error: {e}")
            ser = None  # Prevent crash

    if JOYSTICK_INPUT:
        joystick_init()

    simulation = DroneSimulation(uav=uav1, data_from_uav=data_from_uav, data_to_uav=data_to_uav, 
                                 graphs_xmax=GRAPHS_XMAX, ser=ser, joystick_input=JOYSTICK_INPUT)

    scene.bind('keydown', keyInput)

def keyInput(evt):
    global running
    if evt.key == 'esc':
        running = False
    elif evt.key == 's':
        simulation.send_data = True

def joystick_init():
    """Initialize pygame and detect joysticks"""
    global joystick
    pygame.init()
    pygame.joystick.init()

    joystick_count = pygame.joystick.get_count()
    if joystick_count == 0:
        print("No joystick detected!")
        return

    joystick = pygame.joystick.Joystick(0)
    joystick.init()
    print(f"Connected to: {joystick.get_name()}")

# Classes ----------------------------------------------------------

# Main calling -------------------------------------------------------
if __name__ == "__main__":
    main()
    # Close scene if initialized
    if "scene" in globals():
        scene.append_to_caption('<script>window.close();</script>')
