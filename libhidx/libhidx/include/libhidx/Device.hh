//
// hidviz - Tool for in-depth analysis of USB HID devices communication
// Copyright (C) 2017 Ondrej Budai <ondrej@budai.cz>
//
// This file is part of hidviz.
//
// hidviz is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// hidviz is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with hidviz.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef LIBHIDX_HIDLIBDEVICE_HH
#define LIBHIDX_HIDLIBDEVICE_HH

#include <vector>
#include <memory>

namespace libhidx {
    namespace buffer {
        class DeviceDescriptor;
        class ConfigDescriptor;
    }
    class Interface;
    class LibHidx;
    class DeviceStrings;

    /// Representation of USB device.
    class Device {
    public:
        /**
         * Constructs new device.
         * @param handle libusb handle
         * @param lib Libhidx instance
         */
        Device(uint64_t handle, LibHidx& lib);
        Device(const Device&) = delete;

        /// Returns libusb device native handle.
        const auto& getPtr() const { return m_device; }

        /// Returns device descriptor.
        auto getDesc() const { return m_descriptor.get(); }

        /// Returns device strings.
        const DeviceStrings& getStrings();

        /// Returns vector of interfaces.
        const auto& getInterfaces() const {return m_interfaces;}

        /// Returns libhidx instance to which this device belongs.
        auto& getLib() {return m_lib;}

        /// Returns device configuration descriptor.
        auto getConfigDesc() const {return m_config_descriptor.get();}


    private:
        /// Libusb device handle
        uint64_t m_device = 0;

        /// Device descriptor
        std::unique_ptr<const buffer::DeviceDescriptor> m_descriptor;

        /// Config descriptor
        std::unique_ptr<const buffer::ConfigDescriptor> m_config_descriptor;

        /// Device strings (vendor, product).
        std::unique_ptr<DeviceStrings> m_strings;

        /// Vector of interfaces.
        std::vector<std::unique_ptr<Interface>> m_interfaces;

        /// Reference of library to which this device belongs.
        LibHidx& m_lib;

        /// Constructs all interfaces belonging to this device.
        void fillInterfaces();

    };
}


#endif //LIBHIDX_HIDLIBDEVICE_HH
