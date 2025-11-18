# sample_cmake_project

## Overview
# Raspberry Pi Compute Module 5 Hardware Peripheral Verification Tool

## Overview
**Raspberry Pi CM5 Peripheral Verification Tool** is a comprehensive CMake-based C++ testing framework designed to verify and monitor all hardware peripherals on the Raspberry Pi Compute Module 5. The tool provides both short verification tests and extended monitoring capabilities for each peripheral, ensuring complete hardware validation.

## Purpose & Motivation
The Raspberry Pi Compute Module 5 features a wide array of hardware peripherals including CPU, GPU, memory, storage, display, camera, USB, networking, GPIO, and power management systems. Comprehensive testing of these peripherals is crucial for:

- Hardware validation during development
- Quality assurance in production
- Debugging hardware-related issues
- Performance monitoring and diagnostics
- Ensuring system stability over time

This tool addresses the need for systematic, automated testing of all CM5 peripherals with professional-grade C++ implementation and extensive documentation.

## Key Features
- **Modular Architecture** – Each peripheral has its own library and test suite
- **Dual Testing Modes** – Short verification tests and extended monitoring
- **Comprehensive Coverage** – Tests all major CM5 hardware peripherals
- **Professional Code Quality** – Full Doxygen documentation, modern C++17, PIMPL pattern
- **CMake Build System** – Cross-platform build configuration with testing support
- **Extensible Framework** – Easy to add new peripherals and test cases
- **Command-Line Interface** – Flexible CLI for individual or batch testing

## Supported Peripherals

| Peripheral | Status | Short Test | Monitoring | Description |
|------------|--------|------------|------------|-------------|
| **CPU** | ✅ Implemented | Core validation, benchmarking, temperature | Temperature stability | CPU performance and thermal monitoring |
| **GPIO** | ✅ Implemented | Digital I/O, PWM, I2C, SPI, UART | Signal stability | General-purpose I/O and communication interfaces |
| **GPU** | 🚧 Planned | OpenGL/Vulkan tests | Performance monitoring | Graphics processing unit validation |
| **Memory** | 🚧 Planned | RAM testing, ECC validation | Memory integrity | System memory verification |
| **Storage** | 🚧 Planned | eMMC/SD card tests | Wear monitoring | Storage device validation |
| **Display** | 🚧 Planned | HDMI output tests | Display stability | Video output verification |
| **Camera** | 🚧 Planned | CSI-2 interface tests | Image capture validation | Camera module testing |
| **USB** | 🚧 Planned | Device enumeration, data transfer | Connection stability | USB host/device testing |
| **Networking** | 🚧 Planned | Ethernet/WiFi tests | Bandwidth monitoring | Network interface validation |
| **Power** | 🚧 Planned | Voltage monitoring, fan control | Power stability | Power management testing |

## Architecture

### Core Components
- **PeripheralTester** – Abstract base class defining the testing interface
- **TestReport** – Structured test results with timing and diagnostic information
- **Modular Libraries** – Each peripheral implemented as a separate static library
- **Main Application** – CLI orchestrator for running tests across all peripherals

### Testing Framework
- **Short Tests** – Quick verification (seconds) of peripheral functionality
- **Monitoring Tests** – Extended testing (minutes/hours) for stability validation
- **Automated Reporting** – Detailed results with pass/fail status and diagnostics

## Getting Started

### Prerequisites
- Linux system (Raspberry Pi OS recommended for full hardware access)
- CMake ≥ 3.20
- GCC ≥ 9.0 with C++17 support
- Doxygen (for documentation generation)
- Google Test (automatically downloaded via FetchContent)

### Build & Install
```bash
# Clone the repository
git clone https://github.com/soccentric/cm5-peripheral-test.git
cd cm5-peripheral-test

# Create build directory
mkdir build && cd build

# Configure the project
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build all components
make

# Run tests
make test

# Generate documentation
make docs
```

### Usage Examples

#### List Available Peripherals
```bash
./cm5_peripheral_test_app --list
```

#### Run All Short Tests
```bash
./cm5_peripheral_test_app --all-short
```

#### Run Individual Peripheral Tests
```bash
# CPU tests
./cm5_peripheral_test_app --cpu-short
./cm5_peripheral_test_app --cpu-monitor 300

# GPIO tests
./cm5_peripheral_test_app --gpio-short
./cm5_peripheral_test_app --gpio-monitor 60
```

#### Run Monitoring Tests
```bash
# Monitor all peripherals for 10 minutes
./cm5_peripheral_test_app --all-monitor 600
```

## Project Structure
```
cm5-peripheral-test/
├── CMakeLists.txt              # Main build configuration
├── README.md                   # This file
├── LICENSE                     # MIT License
├── include/                    # Public headers
│   ├── peripheral_tester.h     # Base testing interface
│   ├── cpu_tester.h           # CPU tester interface
│   └── gpio_tester.h          # GPIO tester interface
├── src/                        # Legacy sample code (to be removed)
├── libs/                       # Peripheral libraries
│   ├── cpu/                   # CPU testing library
│   └── gpio/                  # GPIO testing library
├── apps/                       # Peripheral-specific applications
│   ├── cpu/                   # CPU test application
│   └── gpio/                  # GPIO test application
├── tests/                      # Unit tests
│   ├── cpu/                   # CPU unit tests
│   └── gpio/                  # GPIO unit tests
├── app/                        # Main test orchestrator
└── docs/                       # Documentation
```

## Development Status

### Completed (v1.0)
- ✅ Project architecture and build system
- ✅ Base testing framework (PeripheralTester interface)
- ✅ CPU peripheral testing (benchmarking, temperature, multi-core)
- ✅ GPIO peripheral testing (digital I/O, PWM, I2C, SPI, UART)
- ✅ Main application with CLI interface
- ✅ Comprehensive Doxygen documentation
- ✅ Unit testing with Google Test
- ✅ CMake packaging and installation

### In Development
- 🚧 GPU testing (OpenGL/Vulkan validation)
- 🚧 Memory testing (RAM integrity, ECC)
- 🚧 Storage testing (eMMC/SD card validation)
- 🚧 Display testing (HDMI output verification)
- 🚧 Camera testing (CSI-2 interface validation)
- 🚧 USB testing (host/device functionality)
- 🚧 Networking testing (Ethernet/WiFi validation)
- 🚧 Power management testing (voltage, fan, RTC)

## Contributing
Contributions are welcome! The project follows these development practices:

1. **Code Style** – Modern C++17 with RAII, PIMPL pattern, and comprehensive error handling
2. **Documentation** – All code must have complete Doxygen comments
3. **Testing** – Unit tests required for all new functionality
4. **Modularity** – Each peripheral is a separate library with its own tests

### Adding a New Peripheral
1. Create peripheral header in `include/`
2. Implement tester class inheriting from `PeripheralTester`
3. Add library in `libs/` with CMake configuration
4. Create unit tests in `tests/`
5. Add CLI support in main application
6. Update documentation

## API Reference

### PeripheralTester Interface
```cpp
class PeripheralTester {
public:
    virtual ~PeripheralTester() = default;
    virtual TestReport short_test() = 0;
    virtual TestReport monitor_test(std::chrono::seconds duration) = 0;
    virtual std::string get_peripheral_name() const = 0;
    virtual bool is_available() const = 0;
};
```

### Test Results
```cpp
enum class TestResult { SUCCESS, FAILURE, NOT_SUPPORTED, TIMEOUT, SKIPPED };

struct TestReport {
    TestResult result;
    std::string peripheral_name;
    std::chrono::milliseconds duration;
    std::string details;
    std::chrono::system_clock::time_point timestamp;
};
```

## License
This project is licensed under the **MIT License** – see the `LICENSE` file for details.

## Contact
- **Author**: Sandesh Ghimire
- **Organization**: Soccentric LLC
- **Email**: sandesh@soccentric.com

---

*Built for the Raspberry Pi Compute Module 5 – ensuring hardware reliability through comprehensive testing.*

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