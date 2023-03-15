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

#include <libhidx/InterfaceHandle.hh>
#include <libhidx/Interface.hh>
#include <libhidx/LibHidx.hh>
#include <libhidx/Device.hh>
#include <libhidx/DeviceStrings.hh>

#include <iostream>

namespace libhidx {

    InterfaceHandle::InterfaceHandle(Interface& interface)
        : m_interface{interface}, m_ifaceNumber{interface.getDesc().interfacenumber()}, m_lib{m_interface.getDevice().getLib()} {

        buffer::Open::Request msgOpen;
        msgOpen.set_device(m_interface.getDevice().getPtr());
        auto resOpen = m_lib.sendMessage<buffer::Open>(MessageId::open, msgOpen);

        if(resOpen.retvalue()){
            throw ConnectionException{resOpen.retvalue(), "Opening the device failed: " + std::to_string(resOpen.retvalue())};
        }

        m_handle = resOpen.handle();
#ifndef _WIN32
        buffer::KernelDriverActive::Request msgKernelDriverActive;
        msgKernelDriverActive.set_handle(m_handle);
        msgKernelDriverActive.set_interfacenumber(m_ifaceNumber);
        auto resKernelDriverActive = m_lib.sendMessage<buffer::KernelDriverActive>(MessageId::kernelDriverActive, msgKernelDriverActive);

        if(resKernelDriverActive.retvalue()){
            buffer::DetachKernelDriver::Request msgDetachKernelDriver;
            msgDetachKernelDriver.set_handle(m_handle);
            msgDetachKernelDriver.set_interfacenumber(m_ifaceNumber);
            auto resDetachKernelDriver = m_lib.sendMessage<buffer::DetachKernelDriver>(MessageId::detachKernelDriver, msgDetachKernelDriver);
            if(resDetachKernelDriver.retvalue()){
                throw ConnectionException{resDetachKernelDriver.retvalue(), "Detaching the kernel driver from device failed: " + std::to_string(resDetachKernelDriver.retvalue())};
            }
        }
#endif

        buffer::ClaimInterface::Request msgClaimInterface;
        msgClaimInterface.set_handle(m_handle);
        msgClaimInterface.set_interfacenumber(m_ifaceNumber);
        auto resClaimInterface = m_lib.sendMessage<buffer::ClaimInterface>(MessageId::claimInterface, msgClaimInterface);

        if(resClaimInterface.retvalue()){
            throw ConnectionException{resClaimInterface.retvalue(), "Claiming the interface failed: " + std::to_string(resClaimInterface.retvalue())};
        }
    }

    InterfaceHandle::~InterfaceHandle() {

        buffer::ReleaseInterface::Request msgReleaseInterface;
        msgReleaseInterface.set_handle(m_handle);
        msgReleaseInterface.set_interfacenumber(m_ifaceNumber);
        auto resReleaseInterface = m_lib.sendMessage<buffer::ReleaseInterface>(MessageId::releaseInterface, msgReleaseInterface);

        if(resReleaseInterface.retvalue()){
            std::cerr << "Releasing the interface failed: " << std::to_string(resReleaseInterface.retvalue()) << std::endl;
        }
#ifndef _WIN32
        buffer::AttachKernelDriver::Request msgAttachKernelDriver;
        msgAttachKernelDriver.set_handle(m_handle);
        msgAttachKernelDriver.set_interfacenumber(m_ifaceNumber);
        m_lib.sendMessage<buffer::AttachKernelDriver>(MessageId::attachKernelDriver, msgAttachKernelDriver);
#endif
        buffer::Close::Request msgClose;
        msgClose.set_handle(m_handle);
        m_lib.sendMessage<buffer::Close>(MessageId::close, msgClose);
    }

    DeviceStrings InterfaceHandle::readStrings() {
        auto deviceStrings = DeviceStrings{};
        auto deviceDescriptor = m_interface.getDevice().getDesc();

        deviceStrings.manufacturer = extractString(
            deviceDescriptor->indexmanufacturer());
        deviceStrings.product = extractString(deviceDescriptor->indexproduct());
        deviceStrings.serialNumber = extractString(
            deviceDescriptor->indexserialnumber());

        return deviceStrings;
    }

    std::string InterfaceHandle::extractString(uint32_t index) const {
        buffer::GetStringDescriptorAscii::Request msgGetStringDescriptorAscii;
        msgGetStringDescriptorAscii.set_handle(m_handle);
        msgGetStringDescriptorAscii.set_descindex(index);
        auto response = m_lib.sendMessage<buffer::GetStringDescriptorAscii>(MessageId::getStringDescriptorAscii, msgGetStringDescriptorAscii);
        if(response.retvalue() < 0){
            // TODO better reporting
            return "";
        }
        return response.data();
    }

    int InterfaceHandle::controlOutTransfer(uint8_t requestType, uint8_t request, uint16_t value, uint16_t index, const void* data, size_t length,
                           unsigned int timeout){
        buffer::ControlOutTransfer::Request msgControlTransfer;
        msgControlTransfer.set_handle(m_handle);
        msgControlTransfer.set_requesttype(requestType);
        msgControlTransfer.set_request(request);
        msgControlTransfer.set_value(value);
        msgControlTransfer.set_index(index);
        msgControlTransfer.set_data(data, length);
        msgControlTransfer.set_timeout(timeout);
        auto response = m_lib.sendMessage<buffer::ControlOutTransfer>(MessageId::controlOutTransfer, msgControlTransfer);

        return response.retvalue();
    }

    std::pair<int, std::string> InterfaceHandle::controlInTransfer(uint8_t requestType, uint8_t request, uint16_t value, uint16_t index, uint16_t length,
                      unsigned int timeout){

        buffer::ControlInTransfer::Request msgControlTransfer;
        msgControlTransfer.set_handle(m_handle);
        msgControlTransfer.set_requesttype(requestType);
        msgControlTransfer.set_request(request);
        msgControlTransfer.set_value(value);
        msgControlTransfer.set_index(index);
        msgControlTransfer.set_length(length);
        msgControlTransfer.set_timeout(timeout);
        auto response = m_lib.sendMessage<buffer::ControlInTransfer>(MessageId::controlInTransfer, msgControlTransfer);

        return std::make_pair(response.retvalue(), response.data());
    }

    buffer::InterruptOutTransfer::Response
    InterfaceHandle::interruptOutTransfer(unsigned char endpoint, const void* data,
                                    size_t length,
                                    unsigned timeout) {
        buffer::InterruptOutTransfer::Request request;
        request.set_handle(m_handle);
        request.set_endpoint(endpoint);
        request.set_data(data, length);
        request.set_timeout(timeout);

        return m_lib.sendMessage<buffer::InterruptOutTransfer>(MessageId::interruptOutTransfer, request);
    }

    buffer::InterruptInTransfer::Response
    InterfaceHandle::interruptInTransfer(unsigned char endpoint, uint16_t length, unsigned timeout) {
        buffer::InterruptInTransfer::Request request;
        request.set_handle(m_handle);
        request.set_endpoint(endpoint);
        request.set_length(length);
        request.set_timeout(timeout);
        return m_lib.sendMessage<buffer::InterruptInTransfer>(MessageId::interruptInTransfer, request);
    }
}
