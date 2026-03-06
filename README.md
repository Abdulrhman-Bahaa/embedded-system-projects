# Embedded Systems Projects

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A collection of embedded systems projects demonstrating firmware development, control systems, authentication mechanisms, and unmanned aerial system (UAS) implementations. These projects showcase practical applications of microcontroller programming, sensor integration, and real-time control algorithms.

## Table of Contents

- [Projects](#projects)
  - [UAS (QuadVista)](#uas-quadvista)
  - [OLED Based Password Authenticator](#oled-based-password-authenticator)
  - [ShadowAuth](#shadowauth)
- [Getting Started](#getting-started)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## Projects

### UAS (QuadVista)

An embedded flight controller and ground station visualization platform for unmanned aerial systems.

**Key Features:**
- PID controller implementation for flight stability
- Sensor fusion using IMU (MPU6050) for orientation tracking
- PWM motor control for precise speed adjustment
- Ground Control Station (GCS) with UI for monitoring and commanding
- Real-time telemetry and visualization

**Technologies:**
- C++ for UAV firmware
- Python for GCS software
- IMU sensor integration
- PWM motor control

### OLED Based Password Authenticator

A secure embedded authentication system with OLED display interface and keypad input.

**Key Features:**
- Password authentication with verification and re-entry capabilities
- Menu-driven interface for password management and system settings
- OLED display feedback (128x64 resolution)
- RGB LED and buzzer status indicators
- Access attempt handling with feedback
- Interactive features including a simple game

**Technologies:**
- C++ firmware
- OLED display drivers
- Keypad input handling
- LED and buzzer control

### ShadowAuth

A modular password authentication framework with hidden internal security logic.

**Key Features:**
- Callback-based interface for custom authentication flows
- Secure framework design where sensitive logic is abstracted
- Ideal for embedded systems and security SDKs
- User-implementable interfaces for password handling

**Technologies:**
- C++ modular design
- Interface-based architecture

## Getting Started

### Prerequisites

- **Hardware Requirements:**
  - Microcontroller (e.g., Arduino, ESP32, or similar)
  - IMU sensor (MPU6050) for UAS project
  - OLED display (128x64) for authenticator
  - Keypad for input
  - Motors and ESCs for UAS

- **Software Requirements:**
  - Arduino IDE or PlatformIO for firmware development
  - Python 3.x for GCS software
  - Git for version control

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/Abdulrhman-Bahaa/embedded-systems-apps.git
   cd embedded-systems-apps
   ```

2. For UAS GCS software:
   ```bash
   cd uas/gcs_software
   pip install -r requirements.txt  # If requirements.txt exists
   ```

3. For firmware projects, open the respective directories in your preferred IDE and upload to your microcontroller.

## Project Structure

```
embedded-system-projects/
├── README.md
├── images/
├── uas/
│   ├── README.md
│   ├── gcs_software/
│   │   └── src/
│   │       ├── application.py
│   │       ├── gcs_backend.py
│   │       ├── gcs_ui.py
│   │       └── uav.py
│   └── uav_firmware/
│       ├── include/
│       │   ├── application.hpp
│       │   ├── control.hpp
│       │   ├── gcs.hpp
│       │   ├── imu.hpp
│       │   ├── motors.hpp
│       │   └── std_types.hpp
│       └── src/
│           ├── application.cpp
│           ├── control.cpp
│           ├── gcs.cpp
│           ├── imu.cpp
│           └── motors.cpp
├── oled_based_password_authenticator/
│   ├── include/
│   │   ├── oled_menu.h
│   │   ├── pass_auth_system_config.h
│   │   └── pass_auth_system_main.h
│   └── src/
│       ├── oled_menu.cpp
│       ├── pass_auth_system_interfaces.cpp
│       └── pass_auth_system_main.cpp
└── shadow_auth/
    ├── include/
    │   └── pass_auth_system_main.h
    └── src/
        ├── pass_auth_system_interfaces.cpp
        └── pass_auth_system_main.cpp
```

## Usage

### UAS (QuadVista)
1. Upload UAV firmware to your flight controller
2. Run the GCS software: `python gcs_software/src/application.py`
3. Connect to the UAV via serial communication
4. Monitor telemetry and send commands through the UI

### OLED Based Password Authenticator
1. Upload the firmware to your microcontroller
2. Connect OLED display, keypad, LEDs, and buzzer
3. Power on the system and follow on-screen prompts
4. Use keypad to enter passwords and navigate menus

### ShadowAuth
1. Include the ShadowAuth headers in your project
2. Implement the required callback interfaces
3. Integrate with your authentication logic

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contact

Abdulrhman Bahaa - [GitHub Profile](https://github.com/Abdulrhman-Bahaa)

Project Link: [https://github.com/Abdulrhman-Bahaa/embedded-systems-apps](https://github.com/Abdulrhman-Bahaa/embedded-systems-apps)
