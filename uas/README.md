# QuadVista: Embedded Flight Controller and Ground Station Platform

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

QuadVista is an unmanned aerial system (UAS) platform consisting of an embedded flight controller firmware and a ground control station (GCS) software for monitoring and commanding UAV operations.

![GCS UI](/images/gcs_ui_0.png)

## Features

### UAV Firmware
- **PID Controller**: Implements proportional-integral-derivative control for flight stability
- **IMU Integration**: Sensor fusion using MPU6050 for accurate orientation tracking
- **Motor Control**: PWM-based speed adjustment for quadcopter motors
- **Communication**: Serial interface for telemetry and command exchange

### Ground Control Station (GCS)
- **Real-time Monitoring**: Live telemetry display including altitude, orientation, and motor status
- **Command Interface**: User controls for UAV operations
- **Visualization**: Graphical UI for flight data and system status
- **Data Logging**: Recording of flight parameters for analysis

## Architecture

The system is divided into two main components:

### UAV Firmware (C++)
Located in `uav_firmware/`, this runs on the flight controller microcontroller:
- `application.cpp`: Main application logic
- `control.cpp`: PID control and motor management
- `imu.cpp`: IMU sensor handling and data fusion
- `motors.cpp`: PWM motor control
- `gcs.cpp`: Communication with ground station

### GCS Software (Python)
Located in `gcs_software/`, this provides the ground-based interface:
- `application.py`: Main GCS application
- `gcs_ui.py`: Graphical user interface
- `gcs_backend.py`: Backend logic and data processing
- `uav.py`: UAV communication and telemetry handling

## Getting Started

### Prerequisites
- **Hardware**:
  - Flight controller microcontroller (e.g., Arduino, ESP32)
  - MPU6050 IMU sensor
  - Quadcopter motors and ESCs
  - Serial communication module

- **Software**:
  - Arduino IDE or PlatformIO for firmware
  - Python 3.x for GCS
  - Required Python packages: `pyserial`, `tkinter`, `matplotlib` (install via pip)

### Installation

1. **UAV Firmware**:
   ```bash
   cd uav_firmware
   # Open in Arduino IDE/PlatformIO and upload to microcontroller
   ```

2. **GCS Software**:
   ```bash
   cd gcs_software
   pip install pyserial tkinter matplotlib
   python src/application.py
   ```

### Hardware Setup
1. Connect MPU6050 to the microcontroller I2C pins
2. Wire motors to PWM-capable pins through ESCs
3. Establish serial connection between UAV and GCS

## Usage

1. **Launch GCS**: Run `python gcs_software/src/application.py`
2. **Connect to UAV**: Select the appropriate serial port in the GCS interface
3. **Monitor Telemetry**: View real-time flight data on the dashboard
4. **Send Commands**: Use the UI controls to command the UAV
5. **Flight Operations**: Arm motors, adjust PID parameters, and monitor stability

## Project Structure

```
uas/
├── README.md
├── gcs_software/
│   └── src/
│       ├── application.py      # Main GCS application
│       ├── gcs_backend.py      # Backend processing
│       ├── gcs_ui.py          # GUI components
│       └── uav.py             # UAV communication
└── uav_firmware/
    ├── include/
    │   ├── application.hpp
    │   ├── control.hpp
    │   ├── gcs.hpp
    │   ├── imu.hpp
    │   ├── motors.hpp
    │   └── std_types.hpp
    └── src/
        ├── application.cpp
        ├── control.cpp
        ├── gcs.cpp
        ├── imu.cpp
        └── motors.cpp
```

## Technologies

- **Firmware**: C++ with embedded programming
- **GCS**: Python with Tkinter for GUI
- **Communication**: Serial protocol
- **Sensors**: MPU6050 IMU
- **Control**: PID algorithm implementation
- **Hardware**: PWM motor control

## Contributing

This is part of the [Embedded Systems Projects](https://github.com/Abdulrhman-Bahaa/embedded-systems-apps) repository. Contributions are welcome via pull requests.

## License

This project is licensed under the MIT License.
