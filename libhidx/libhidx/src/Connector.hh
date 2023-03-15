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

#ifndef PROJECT_CONNECTOR_HH
#define PROJECT_CONNECTOR_HH

#include <libhidx/server/Utils.hh>

#include <asio.hpp>

#include <memory>

namespace subprocess {
    class Popen;
}

namespace libhidx {

    /**
     * Connector class for connection frontend with backend.
     *
     * This class contains only abstract interface.
     */
    class Connector {
    public:
        /// Connects to backend.
        virtual bool connect() = 0;

        /**
         * Communicates with backend.
         *
         * By communicate it's meant send request and receive response.
         * @param message Request
         * @return Response
         */
        virtual std::string sendMessage(const std::string& message) = 0;
    };

    /**
     * LocalConnector connects to backend directly via API.
     *
     * This means backend is running in the same process as frontend.
     * If frontend doesn't have sufficient privileges to communicate
     * with usb devices.
     */
    class LocalConnector : public Connector {
    public:
        LocalConnector();
        bool connect() override;
        std::string sendMessage(const std::string& message) override;
    };

#ifdef __unix__
    /**
     * UnixSocketConnector uses unix socket to commucicate with backend.
     *
     * This connector spawns a helper process, which then communicates
     * with usb devices. The helper is started with polkit, therefore
     * it has root privileges needed for communcation with usb devices
     * on most linux systems.
     */
    class UnixSocketConnector : public Connector {
    public:
        UnixSocketConnector();
        ~UnixSocketConnector();

        bool connect() override;
        std::string sendMessage(const std::string& message) override;

    private:
        /// Pointer to helper process.
        std::unique_ptr<subprocess::Popen> m_process;

        /// Directory where the socket is located.
        std::string m_socketDir;

        /// Asio IO service.
        asio::io_service m_ioService;

        /// Socket used to communicate with helper process.
        std::unique_ptr<asio::generic::stream_protocol::socket> m_socket;

        /// Returns current working directory for frontend process.
        std::string getExecutablePath();

        /**
         * Returns the path to helper binary.
         *
         * This function searches for binary in several directories.
         * Therefore it should work in development environment and also
         * in production environment.
         *
         * @todo Return meaningful value if the binary wasn't found
         */
        std::string getServerPath();
    };
#endif

}


#endif //PROJECT_CONNECTOR_HH
