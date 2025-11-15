# sample_cmake_project

## Overview
**sample_cmake_project** is a CMake‑based C++ project that implements a custom Linux kernel driver for the Raspberry Pi, enabling software‑based PWM (Pulse‑Width Modulation) generation through GPIO bit‑banging. By leveraging precise timing loops and kernel timers, the driver can toggle any GPIO pin at user‑defined frequencies and duty cycles, providing a flexible alternative to the limited hardware PWM channels available on the Pi. The driver exposes a clean user‑space interface via standard Linux mechanisms (ioctl and sysfs), allowing applications to configure PWM parameters in real time without recompiling or reloading the module.

## Purpose & Motivation
Many embedded projects quickly run into the bottleneck of having only a handful of hardware PWM outputs, while the need for additional PWM signals grows—for example, driving multiple LEDs, servos, or motor controllers simultaneously. **sample_cmake_project** addresses this limitation by moving PWM generation into software, making *any* GPIO pin a potential PWM source. Although this approach is more CPU‑intensive and slightly less precise than hardware PWM, it offers unparalleled flexibility: you can create as many PWM streams as you need, change frequencies on the fly, and experiment with unconventional duty‑cycle patterns that hardware modules may not support. This makes the project ideal for prototyping, educational purposes, and niche applications where hardware resources are scarce.

## Key Features
- **Full‑GPIO PWM support** – Turn any Raspberry Pi GPIO pin into a PWM output.
- **Dynamic configuration** – Adjust frequency (1 Hz – 10 kHz) and duty cycle (0 % – 100 %) at runtime via ioctl or sysfs.
- **Multi‑channel capability** – Run multiple independent PWM streams concurrently, limited only by CPU capacity.
- **Kernel‑timer precision** – Utilizes high‑resolution kernel timers to achieve sub‑microsecond timing accuracy.
- **CMake build system** – Simple, cross‑platform build configuration with automatic kernel module compilation and installation targets.
- **Comprehensive documentation** – Detailed README, API reference, and example user‑space programs.

## Benefits
- **Scalability** – No need to redesign hardware when you need more PWM signals; just allocate another GPIO.
- **Rapid development** – Change PWM parameters on the fly without rebooting or re‑loading the driver.
- **Educational value** – Demonstrates low‑level kernel programming, timer management, and GPIO manipulation on Linux.
- **Open source flexibility** – Extend or modify the driver to suit custom timing requirements, integrate with other kernel subsystems, or add advanced features like PWM chaining.

## Getting Started

### Prerequisites
- Raspberry Pi running a Debian‑based Linux distribution (Raspberry Pi OS recommended)
- Kernel headers matching your running kernel (`sudo apt-get install raspberrypi-kernel-headers`)
- CMake ≥ 3.15
- GNU Make, GCC, and standard build tools
- Root privileges for installing the kernel module (`sudo`)

### Build & Install
```bash
# Clone the repository
git clone https://github.com/yourusername/sample_cmake_project.git
cd sample_cmake_project

# Create a build directory
mkdir build && cd build

# Configure the project (detects kernel headers automatically)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Compile the kernel module and user‑space helper library
make

# Install the kernel module (requires sudo)
sudo make install

# Load the module
sudo modprobe sample_pwm
```

### Usage Example
The repository includes a small example program `pwm_ctl` that demonstrates how to set PWM parameters via ioctl.

```bash
# Set GPIO 18 to 1 kHz, 75 % duty cycle
sudo ./pwm_ctl --gpio 18 --freq 1000 --duty 75

# Verify via sysfs (optional)
cat /sys/class/pwm/pwmchip0/pwm18/period
cat /sys/class/pwm/pwmchip0/pwm18/duty_cycle
```

## User‑Space Interface
| Interface | Path | Description |
|-----------|------|-------------|
| **ioctl** | `/dev/sample_pwm` | Allows atomic configuration of frequency and duty cycle using a custom ioctl command structure. |
| **sysfs** | `/sys/class/pwm/pwmchip0/pwmX/` | Exposes `period`, `duty_cycle`, `enable`, and `polarity` attributes for each active PWM channel. |

Both methods can be used interchangeably; ioctl is preferred for high‑performance, low‑latency updates, while sysfs offers simple scripting access.

## Contributing
Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a feature branch (`git checkout -b feature/your-feature`).
3. Ensure the code builds and passes `make test` (if applicable).
4. Submit a pull request with a clear description of changes.

Please adhere to the existing coding style and include documentation updates for any new functionality.

## License
This project is licensed under the **MIT License** – see the `LICENSE` file for details.

--- 

*Happy hacking! Let the GPIOs sing with software PWM.*