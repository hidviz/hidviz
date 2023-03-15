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

#ifndef LIBHIDX_HIDLIB_HH
#define LIBHIDX_HIDLIB_HH

#include <libhidx/server/Utils.hh>

#include <vector>
#include <memory>
#include <mutex>
#include <functional>

namespace google {
namespace protobuf {
    class Message;
}
}

namespace libhidx {

    /// Exception thrown then Libhidx cannot initialize itself.
    /// @todo Make better exception system
    class LibHidxError : public std::runtime_error {
    public:
        explicit LibHidxError(const std::string& str) : std::runtime_error{str}{}
    };

    class Device;
    class Connector;

    /**
     * Main class of libhidx library.
     *
     * You van have more instances of this library, but it's mostly pointless.
     */
    class LibHidx {
    public:
        LibHidx();
        LibHidx(const LibHidx&) = delete;
        ~LibHidx();

        /**
         * Prepares connection to server using local connection
         *
         * Local connection will cause all usb requests will be run
         * from same thread as the frontend of library. This is the
         * most efficent way, but it may not work if user which is running
         * the library, doesn't have sufficient privileges.
         */
        void connectLocal();

#ifdef __unix__
        /**
         * Prepares connection to server using unix domain socket.
         *
         * AVAILABLE ONLY ON LINUX/BSD
         *
         * Unix socket is preferred way for connecting on linux.
         */
        void connectUnixSocket();
#endif

        /**
         * Does connect to backend.
         *
         * This function can be called after some of the connect methods.
         * It does the actual connecting. It's completely OK for this
         * function to return false. In that case, try to connect to backend
         * later. There's a chance backend is not ready yet for accepting
         * messages.
         * @return True if connection was sucessful.
         */
        bool doConnect();

        /// Inits the library.
        void init();

        /**
         * Reloads device list.
         *
         * This function will force reload the library.
         * Therefore if some devices were loaded prior to call
         * of this function. They will be destroyed.
         */
        void reloadDevices();

        /**
         * Loads device list
         *
         * If the device list is already loaded, this function will do
         * nothing.
         */
        void loadDevices();

        /// Free device list.
        void freeDevices();

        /// Returns device list.
        const auto& getDevices(){return m_devices;}

        /**
         * Sends and receives message to or from backend.
         *
         * This function is blocking in many ways. Firstly only one caller
         * may send/receive message in one time. The others must wait.
         * This is forced internally by mutex, therefore this function
         * is thread safe. But it means if you have long request on
         * one thread, the other thread's request will be processed after
         * the first one is done. This can produce quite large delays.
         *
         * @todo Do not block server
         * @todo Remove MessageId.
         * @tparam Msg Message class to be sent
         * @param messageId Message Id of message to be sent.
         * @param request Message request
         * @return Message response
         */
        template<typename Msg>
        typename Msg::Response sendMessage(MessageId messageId, const typename Msg::Request& request){
            std::lock_guard<std::mutex> lock{m_commMutex};

            auto messageOut = utils::packMessage(messageId, request.SerializeAsString());

            auto messageIn = sendMessage(messageOut);

            auto payloadIn = utils::unpackMessage(messageIn);

            typename Msg::Response response;

            response.ParseFromString(payloadIn.second);

            return response;
        }

    private:
        /// Connector used to communicate with backend.
        std::unique_ptr<Connector> m_connector;

        /// List of devices.
        std::vector<std::unique_ptr<Device>> m_devices;

        /// Mutex for communication.
        std::mutex m_commMutex;

        /// libusb device list handle.
        uint64_t m_listHandle = 0;

        /// libusb context.
        uint64_t m_ctx = 0;

        /// Determines if this instance is initialized.
        bool m_initialized = false;

        /**
         * Sends and receives buffer to/from server.
         * @param message Request message
         * @return Response message
         */
        std::string sendMessage(const std::string& message);
    };
}

#endif //LIBHIDX_HIDLIB_HH
