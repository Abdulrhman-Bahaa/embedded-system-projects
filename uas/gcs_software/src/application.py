""" ---------- Main application for UAV ground control simulation --------- """

# Imports ---------------------------------------------------------------------
from gcs_ui import DroneSimulation, pygame, scene
import serial


# Global Variables ------------------------------------------------------------
simulation = None
joystick = None
ser = None  # Serial connection to the UAV, initialized later


# Main function ---------------------------------------------------------------

def main():
    """Main function to initialize the application and start the simulation."""
    application_initialize()
    simulation.start()


# Functions -------------------------------------------------------------------

def application_initialize():
    """Initialize the application settings and serial connection."""
    global simulation, ser
    PORTS = {
        "bluetooth": '/dev/rfcomm0',
        "simulation": '/dev/tnt1',
        "arduino": '/dev/ttyACM0'
    }
    BAUD_RATE = 57600
    JOYSTICK_INPUT = False

    try:
        ser = serial.Serial(PORTS['arduino'], BAUD_RATE, timeout=1)
    except serial.SerialException as e:
        print(f"Serial connection error: {e}")
        ser = None  # Prevent crash

    if JOYSTICK_INPUT:
        joystick_init()

    simulation = DroneSimulation(ser=ser, joystick_input=JOYSTICK_INPUT)


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


# Main calling ----------------------------------------------------------------
if __name__ == "__main__":
    main()
    ser.close()  # Close serial connection if it was opened

    if "scene" in globals():
        scene.append_to_caption('<script>window.close();</script>')
