# Embedded Systems Applications  

This repository contains various **embedded systems projects** that I am working on to improve my skills in **firmware development, control systems, etc**.  

## Projects Included  
### [ShadowAuth (Modular password authentication system)](shadow_auth/)
- Purpose: A secure authentication framework where the internal security logic is hidden from the user.
- User Interaction: The user is required to implement specific interfaces (callbacks) to handle authentication flow.
- Target Use Case: Ideal for embedded systems, security frameworks, or SDKs where users need to integrate with a secure system but not directly interact with sensitive logic.

### [Unmanned Aerial System (UAS)](uas/)
- Implementing a **PID controller** for stability  
- Sensor fusion using an **IMU (MPU6050)**  
- PWM control for **motor speed adjustment**   
- UI interface for **monitoring and command (GCS)**

### [OLED Based Password Authenticator](oled_based_password_authenticator/)
- Purpose: A secure, embedded authentication system that allows users to implement their own logic for password handling, access control, and system management.
- User Interaction: The user interacts with the system via keypad input and responds to authentication prompts displayed on an OLED screen.
- Key Features:
    - Implements password authentication with the ability to verify and re-enter passwords.
    - Provides a menu-driven interface for functionality like changing the password, adjusting display contrast, and logging out.
    - Displays feedback on a 128x64 OLED display, with control over RGB LEDs and buzzer for status indication.
    - Supports access attempts, handling both correct and incorrect password entries with feedback.
    - Includes additional features like a simple game (in the menu), making it interactive and engaging.



## Why I Made This Repo  
I am learning **embedded systems engineering**, and I use these projects to:  
- Gain hands-on experience with real-world applications  
- Improve **microcontroller programming**  
-  Experiment with different **communication protocols and control algorithms**  

## How to Use  
1. Clone the repository:  
```bash
git clone https://github.com/abdulrhman-Bahaa/embedded-systems-apps.git
cd embedded-systems-apps
