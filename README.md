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

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author

Nathaniel Asia

## Warning

This is a DIY project for electric skateboard control. Use at your own risk and always wear appropriate safety gear. Ensure all connections are secure and properly insulated before use.
