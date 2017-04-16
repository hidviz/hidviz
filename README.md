# Hidviz
[![Build Status](https://travis-ci.org/ondrejbudai/hidviz.svg?branch=master)](https://travis-ci.org/ondrejbudai/hidviz)

Hidviz is a GUI application for in-depth analysis of USB HID class devices. The 2 main usecases of this aplication are reverse-engineering existing devices and developing new USB HID devices.

*USB HID class consists of many possible devices, e.g. mice, keyboards,
joysticks and gamepads. But that's not all! There are more exotic HID devices, e.g. weather stations, medical equipment
(thermometers, blood pressure monitors) or even simulation devices (think of flight sticks!).*

![Hidviz screenshot](https://github.com/ondrejbudai/hidviz/blob/master/media/screenshot.png?raw=true)

## Building
### Prerequisities
- C++ compiler with C++14 support
- libusb 1.0 (can be called libusbx in you distro)
- protobuf (v2 is enough)
- Qt5 base
- Posix environment (see ondrejbudai/libhidx#2)
- CMake (>=3.2)

#### Installing prerequsities on Fedora
```
sudo dnf install gcc-c++ gcc qt5-qtbase-devel protobuf-devel libusbx-devel
```
*Tested on Fedora 25.*

#### Installing prequsities on Ubuntu
```
sudo apt-get install build-essential qtbase5-dev libprotobuf-dev protobuf-compiler libusb-1.0-0-dev
```
*Note that Ubuntu 14.04 LTS has old gcc unable to build hidviz, you need to install at least [gcc 5](https://launchpad.net/~ubuntu-toolchain-r).*
### Clone and prepare
```
git clone --recursive https://github.com/ondrejbudai/hidviz.git
mkdir hidviz/build
cd hidviz/build
cmake ..
```
*Please note you have to do recursive clone.*

### Build
```
make -j$(nproc)
```

## Running
**To run this project you need `build/hidviz` as you current directory for hidviz to work properly!**

After successful build you need to run

```
cd hidviz
./hidviz
```

## Installing
Not yet available

## License
Not yet available
