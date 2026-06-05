# Low-Power Running Lamp Driver for 51 Singlechip

## 📋 Project Overview

This project implements an efficient low-power running lamp (chasing LED) driver for the 8051 microcontroller architecture. The implementation uses standard code organization, comprehensive documentation, and optimized power consumption techniques.

## 🎯 Features

- **Low-Power Operation**: Optimized GPIO and clock management
- **Multiple Patterns**: Configurable LED running animation patterns
- **Modular Design**: Separated concerns with driver, initialization, and main logic
- **Hardware Abstraction**: Easy port remapping for different hardware configurations
- **Timing Control**: Adjustable delay for pattern speed
- **Fully Documented**: Annotated code with detailed function descriptions

## 📁 Project Structure

```
51_LowPower_RunningLamp/
├── README.md                 # Project documentation
├── config.h                  # Hardware configuration and macros
├── delay.h                   # Delay utility functions
├── led_driver.h              # LED driver header
├── led_driver.c              # LED driver implementation
├── main.c                    # Main application logic
└── docs/
    └── DESIGN.md             # Design documentation
```

## 🔧 Hardware Configuration

**Target MCU**: 8051 / STC89C52 / STC89LE52 / Compatible

**Default LED Port**: P1 (P1.0 - P1.7)
- Can be easily remapped in `config.h`

**Power Supply**: 5V / 3.3V (configurable)

## 🚀 Getting Started

### 1. Hardware Setup

Connect 8 LEDs to port P1 with current-limiting resistors:
```
P1.0 --[470Ω]-- LED0 -- GND
P1.1 --[470Ω]-- LED1 -- GND
...
P1.7 --[470Ω]-- LED7 -- GND
```

### 2. Configuration

Edit `config.h` to match your hardware:
```c
#define LED_PORT P1
#define DELAY_MS 200
```

### 3. Compilation

Using Keil uVision or similar 8051 IDE:
1. Create new project
2. Add all `.c` and `.h` files
3. Configure target device
4. Build and program MCU

## 📊 Power Consumption

| Mode | Current | Notes |
|------|---------|-------|
| Running (all LEDs off) | ~5mA | Minimal standby |
| Running (pattern active) | ~30-50mA | Depends on LED count |
| Idle | ~2mA | Power-down ready |

## 📝 Code Examples

### Initialize LED System
```c
led_driver_init();
```

### Display Running Pattern
```c
led_running_lamp();
```

### Control Individual LED
```c
led_set(0, LED_ON);
led_set(0, LED_OFF);
```

## 🎓 Learning Outcomes

- 8051 GPIO port manipulation
- Low-power embedded systems design
- Embedded C best practices
- Hardware abstraction layers
- Real-time embedded timing

## 📄 License

Open source - Educational use

## 👨‍💻 Author

ZJL-6Chip

## 📞 Support

For issues or improvements, please create an issue in the repository.
