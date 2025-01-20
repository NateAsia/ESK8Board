# Electric Skateboard Controller Project

A DIY electric skateboard control system featuring a handheld remote and board-mounted controller, built using Arduino. This project implements a reliable wireless control system for electric skateboard applications with safety features and real-time monitoring.

## Hardware Components

### Board Controller
- Arduino Nano
- VESC (Motor Controller)
- 433MHz RF Receiver
- TM1637 7-segment Display
- Status LED
- Voltage Monitoring Circuit (voltage divider)

### Hand Remote
- Arduino Nano
- 433MHz RF Transmitter
- Analog Joystick (with integrated button)
- Status LED

## Features

- Wireless throttle control using 433MHz communication
- Real-time battery monitoring and display
- Safety cutoff system for signal loss
- Power-efficient remote with sleep mode
- Packet validation for reliable communication
- LED status indicators
- Configurable motor response

## Project Structure

```
.
├── BoardControllerFirmware/          # Board-mounted controller code
│   ├── BoardControllerFirmware.ino
│   ├── skateboard.h/cpp                # Main controller class
│   ├── button_switch.h                 # Power switch handling
│   ├── led.h                           # LED control
│   └── voltmeter.h                     # Battery monitoring
├── HandTransmitterFirmware/          # Remote control code
│   ├── HandTransmitterFirmware.ino
│   ├── transmitter.h/cpp               # Main remote class
│   ├── joystick.h                      # Input handling
│   └── led.h                           # Status indication
├── ESK8Comms/                        # Shared communication protocol
│   ├── ESK8Comms.h
│   └── ESK8Comms.cpp
└── Makefile                          # Build system
```

## Setup Instructions

1. **Hardware Assembly**
   - Wire the components according to the pin definitions in the firmware
   - Ensure proper power supply connections
   - Configure VESC parameters for your motor

2. **Software Requirements**
   - Arduino IDE
   - Required Libraries:
     - RCSwitch
     - TM1637Display
     - Servo
     - LowPower

3. **Building and Flashing**
   ```bash
   # Copy communication files to both projects
   make copy_comms

   # Flash using Arduino IDE:
   # 1. Open BoardControllerFirmware/BoardControllerFirmware.ino
   # 2. Open HandTransmitterFirmware/HandTransmitterFirmware.ino
   # 3. Upload to respective Arduino boards
   ```

## Safety Features

- Radio disconnect detection with motor cutoff
- Power switch override
- Battery voltage monitoring
- Throttle position validation
- Packet integrity checking

## Development Notes

- Communication protocol uses 2-byte packets with validation
- Remote enters sleep mode when inactive
- Board controller maintains safe state during signal loss
- Battery voltage is sampled and filtered for stability

## Warning

> [!CAUTION]
> This is a DIY electric skateboard control system that interfaces with high-power electrical components and motor controllers. Improper assembly or use could result in serious injury or death.
>
> - This project is provided "AS IS" without warranty of any kind
> - The author assumes NO responsibility or liability for any injuries, damages, or losses
> - You are solely responsible for the safe implementation and use of this system
> - Building and using this system indicates acceptance of all risks involved
>
> **REQUIRED SAFETY MEASURES:**
> - Always wear appropriate safety gear (helmet, pads, etc.)
> - Thoroughly test all systems before riding
> - Ensure all electrical connections are secure and properly insulated
> - Install and test emergency cutoff systems
> - Never exceed the rated specifications of your components
>
> By using any part of this project, you acknowledge that electric skateboarding is inherently dangerous and agree that the author (Nathaniel Asia) shall not be held liable for any direct, indirect, incidental, special, exemplary, or consequential damages arising from the use of this project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
