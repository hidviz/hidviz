# Hidviz

Hidviz is a GUI application for in-depth analysis of USB HID class devices. The 2 main usecases of this application are reverse-engineering existing devices and developing new USB HID devices.

*USB HID class consists of many possible devices, e.g. mice, keyboards,
joysticks and gamepads. But that's not all! There are more exotic HID devices, e.g. weather stations, medical equipment
(thermometers, blood pressure monitors) or even simulation devices (think of flight sticks!).*

![Hidviz screenshot](https://github.com/ondrejbudai/hidviz/blob/main/media/screenshot.png?raw=true)

## 1) Building

Hidviz can be built on various platforms where following prerequisites can be obtained. Currently only Fedora, Ubuntu and MSYS2/Windows are supported and build guide is available for them.  There is also experimental support for MacOS which has currently been tested on an M1 based Mac.

### 1.1) Prerequisities
- C++ compiler with C++14 support
- libusb 1.0 (can be called libusbx in you distro)
- protobuf (v2 is enough)
- Qt6 base
- CMake (>=3.2)

#### 1.1.1) Installing prerequisities on Fedora
```
sudo dnf install cmake gcc-c++ qt6-qtbase-devel protobuf-devel libusbx-devel asio-devel
```

#### 1.1.2) Installing prerequisities on Ubuntu
```
sudo apt-get install build-essential qtbase6-dev libprotobuf-dev protobuf-compiler libusb-1.0-0-dev
```

#### 1.1.3) Installing prerequisities on MSYS2/Windows
*Please note hidviz is primarily developed on Linux and we currently don't have Windows CI therefore Windows build can be broken at any time. If you find so, please create an issue.*

If you do not have MSYS2 installed, firstly follow this [guide](https://github.com/msys2/msys2/wiki/MSYS2-installation) to install MSYS2.

```
pacman -S git mingw-w64-x86_64-cmake mingw-w64-x86_64-qt6 mingw-w64-x86_64-libusb \
          mingw-w64-x86_64-protobuf mingw-w64-x86_64-protobuf-c mingw-w64-x86_64-toolchain \
          make
```

#### 1.1.4) Install prerequsites on MacOS
_Please not hidviz on macos is not an officially supported platform.  This has been known to work, but your mileage may vary._

You need to install cmake, qt6 and protobuf and libusb via brew.  Please also ensure that if you are using an M1 based mac that your build environment matches the host architecture.  I was stumped for a while with a failing build because I was using an x86 version of vscode which caused problems when querying ```arch``` as a part of the build.  Switching to a native terminal fixed the issue for me.

```bash
brew install cmake qt@5 protobuf libusb
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
#### 1.2.3) Configuring on MacOS
```bash
cmake .. -DQt6_DIR=$(brew --prefix qt6)/lib/cmake/Qt6 -DCMAKE_PREFIX_PATH=/opt/homebrew
````
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

### 2.1) Running on Windows
Unfortunately you cannot use hidviz on Windows straight out of the box. Firstly you need to install custom drivers to devices you want to analyze.

I recommend you using following approach: Download [Zadig](http://zadig.akeo.ie/), run it and install libusbK to device you want to analyze. You can find more information about the need for custom driver on Windows on [libusb wiki](https://github.com/libusb/libusb/wiki/Windows#how-to-use-libusb-on-windows).

## 3) Installing (Linux only)

```
sudo make install
```

Alternatively, you can install [`hidviz`](https://aur.archlinux.org/packages/hidviz/) from Arch Linux's AUR repository:

```
pacaur -S hidviz
```

## 4) License
Hidviz is license under GPLv3+. For more information see LICENSE file.
