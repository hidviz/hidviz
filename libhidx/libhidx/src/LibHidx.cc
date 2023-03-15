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

// TODO: Workaround for this bug: https://sourceforge.net/p/mingw-w64/bugs/611/
#include <thread>

#include <libhidx/LibHidx.hh>

#include <libhidx/server/Buffer.hh>
#include <libhidx/Device.hh>
#include <libhidx/Interface.hh>
#include <libhidx/DeviceStrings.hh>
#include <Connector.hh>

namespace libhidx {

#ifndef _WIN32

#endif

    LibHidx::LibHidx() {}

    void LibHidx::connectLocal(){
        m_connector = std::make_unique<LocalConnector>();
    }

#ifdef __unix__
    void LibHidx::connectUnixSocket(){
        m_connector = std::make_unique<UnixSocketConnector>();
    }
#endif

    bool LibHidx::doConnect() {
        try {
            m_connector->connect();
        } catch(asio::system_error&){
            return false;
        }

        return true;
    }

    void LibHidx::init(){
        buffer::Init::Request initReq;

        auto response = sendMessage<buffer::Init>(MessageId::init, initReq);

        if(response.retvalue() != 0){
            throw LibHidxError{"Cannot initialize libhidx."};
        }

        m_ctx = response.ctx();
        m_initialized = true;
    }

    LibHidx::~LibHidx() {
        freeDevices();
        if(!m_initialized){
            return;
        }
        buffer::Exit::Request exitReq;
        exitReq.set_ctx(m_ctx);
        sendMessage<buffer::Exit>(MessageId::exit, exitReq);
    }

    void LibHidx::reloadDevices() {
        freeDevices();

        buffer::GetDeviceList::Request getDeviceListRequest;
        getDeviceListRequest.set_ctx(m_ctx);

        auto response = sendMessage<buffer::GetDeviceList>(MessageId::getDeviceList, getDeviceListRequest);

        for(const auto& deviceHandle: response.devicelist()){
            m_devices.emplace_back(std::make_unique<Device>(deviceHandle, *this));
        }

        m_listHandle = response.listhandle();
    }

    void LibHidx::loadDevices() {
        if(!m_listHandle){
            reloadDevices();
        }
    }

    void LibHidx::freeDevices()  {
        m_devices.clear();
        if (m_listHandle) {
            buffer::FreeDeviceList::Request freeDeviceListReq;
            freeDeviceListReq.set_listhandle(m_listHandle);
            freeDeviceListReq.set_unrefdevices(1);
            sendMessage<buffer::FreeDeviceList>(MessageId::freeDeviceList, freeDeviceListReq);
            m_listHandle = 0;
        }
    }

    std::string LibHidx::sendMessage(const std::string& message) {
        return m_connector->sendMessage(message);
    }

}
