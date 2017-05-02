# Hidviz
[![Build Status](https://travis-ci.org/ondrejbudai/hidviz.svg?branch=master)](https://travis-ci.org/ondrejbudai/hidviz)
[![Build status](https://ci.appveyor.com/api/projects/status/8v04qku72xc7sh4t/branch/master?svg=true)](https://ci.appveyor.com/project/ondrejbudai/hidviz/branch/master)

Hidviz is a GUI application for in-depth analysis of USB HID class devices. The 2 main usecases of this aplication are reverse-engineering existing devices and developing new USB HID devices.

*USB HID class consists of many possible devices, e.g. mice, keyboards,
joysticks and gamepads. But that's not all! There are more exotic HID devices, e.g. weather stations, medical equipment
(thermometers, blood pressure monitors) or even simulation devices (think of flight sticks!).*

![Hidviz screenshot](https://github.com/ondrejbudai/hidviz/blob/master/media/screenshot.png?raw=true)

## 1) Building

Hidviz can be built on various platforms where following prerequisities can be obtained. Currently only Fedora, Ubuntu and MSYS2/Windows are supported and build guide is available for them.

### 1.1) Prerequisities
- C++ compiler with C++14 support
- libusb 1.0 (can be called libusbx in you distro)
- protobuf (v2 is enough)
- Qt5 base
- CMake (>=3.2)

#### 1.1.1) Installing prerequisities on Fedora
```
sudo dnf install gcc-c++ gcc qt5-qtbase-devel protobuf-devel libusbx-devel
```

#### 1.1.2) Installing prerequisities on Ubuntu
```
sudo apt-get install build-essential qtbase5-dev libprotobuf-dev protobuf-compiler libusb-1.0-0-dev
```
*Note that Ubuntu 14.04 LTS has old gcc unable to build hidviz, you need to install at least [gcc 5](https://launchpad.net/~ubuntu-toolchain-r).*

#### 1.1.3) Installing prerequisities on MSYS2/Windows
*Please note hidviz is primarily developed on Linux and we currently don't have Windows CI therefore Windows build can be broken at any time. If you find so, please create an issue.*

If you do not have MSYS2 installed, firstly follow this [guide](https://github.com/msys2/msys2/wiki/MSYS2-installation) to install MSYS2.

```
pacman -S git mingw-w64-x86_64-cmake mingw-w64-x86_64-qt5 mingw-w64-x86_64-libusb \
          mingw-w64-x86_64-protobuf mingw-w64-x86_64-protobuf-c mingw-w64-x86_64-toolchain \
          make
```

### 1.2) Clone and prepare out of source build
Firstly you need to obtain sources from git and prepare directory for out of source build:
```
git clone --recursive https://github.com/ondrejbudai/hidviz.git
mkdir hidviz/build
cd hidviz/build
```
**Please note you have to do recursive clone.**
### 1.3) Configuring
#### 1.2.1) Configuring on Fedora/Ubuntu (Linux)
```
cmake ..
```
#### 1.2.2) Configuring on MSYS2/Windows
```
cmake -G "Unix Makefiles" ..
```
### 1.4) Build
```
make -j$(nproc)
```

*If you are doing MSYS2 build, check before build you are using MinGW32/64 shell, otherwise the build process won't work. More information can be found [here](https://github.com/msys2/msys2/wiki/MSYS2-introduction).*

## 2) Running
**To run this project you need `build/hidviz` as you current directory for hidviz to work properly!**

After successful build you need to run

```
cd hidviz
./hidviz
```

### 2) Running on Windows

## 3) Installing
Not yet available

## 4) License
Not yet available
