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

#include <libhidx/Device.hh>

#include <libhidx/Interface.hh>
#include <libhidx/InterfaceHandle.hh>
#include <libhidx/LibHidx.hh>
#include <libhidx/DeviceStrings.hh>

namespace libhidx {

    Device::Device(uint64_t device, LibHidx& lib) : m_lib{lib} {
        m_device = device;

        buffer::GetDeviceDescriptor::Request msg;
        msg.set_device(m_device);
        auto response = m_lib.sendMessage<buffer::GetDeviceDescriptor>(MessageId::getDeviceDescriptor, msg);

        if(response.retvalue() != 0){
            throw IOException{"Cannot retrieve device descriptor."};
        }

        m_descriptor = std::make_unique<const buffer::DeviceDescriptor>(response.devicedescriptor());

        buffer::GetActiveConfigDescriptor::Request msg2;
        msg2.set_device(m_device);
        auto response2 = m_lib.sendMessage<buffer::GetActiveConfigDescriptor>(MessageId::getActiveConfigDescriptor, msg2);
        if(response.retvalue() != 0){
            throw IOException{"Cannot retrieve config descriptor."};
        }

        m_config_descriptor = std::make_unique<const buffer::ConfigDescriptor>(response2.configdescriptor());
        fillInterfaces();
    }

    const DeviceStrings& Device::getStrings() {
        if(!m_strings) {
            try {
              auto deviceHandle = m_interfaces.front()->getHandle();
              m_strings = std::make_unique<DeviceStrings>(deviceHandle->readStrings());
            } catch(ConnectionException &ex){
              m_strings = std::make_unique<DeviceStrings>();
            }
        }

        return *m_strings;
    }

    void Device::fillInterfaces() {
        for(const auto& interfaceDesc: m_config_descriptor->interface()){
            m_interfaces.emplace_back(std::make_unique<Interface>(interfaceDesc, *this));
        }
    }
}
