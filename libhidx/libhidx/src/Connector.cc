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

#include "Connector.hh"

#include <libhidx/server/Server.hh>

#ifdef __unix__
#include "subprocess.hh"
#endif

#ifdef __FreeBSD__
#include <libgen.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

using generic = asio::generic::stream_protocol;

#ifdef __unix__
using local = asio::local::stream_protocol;
#endif

namespace libhidx {

    LocalConnector::LocalConnector() {

    }

    bool libhidx::LocalConnector::connect() {
        return true;
    }

    std::string libhidx::LocalConnector::sendMessage(const std::string& message) {
        return server::cmd(message);
    }

#ifdef __unix__

    constexpr size_t length(const char* str)
    {
        return *str ? 1 + length(str + 1) : 0;
    }

    UnixSocketConnector::UnixSocketConnector() {
        constexpr const char* t = "/tmp/libhidxXXXXXX";
        std::string ta{t};
        std::array<char, length(t) + 1> temp;

        std::copy(std::begin(ta), std::end(ta), std::begin(temp));
        temp.back() = 0;


        mkdtemp(temp.data());
        m_socketDir = temp.data();

        auto serverPath = getServerPath();

        m_process = std::make_unique<subprocess::Popen>(
            "pkexec " + serverPath + " -p -u " + temp.data(),
            subprocess::input{subprocess::PIPE},
            subprocess::output{subprocess::PIPE}
        );

        m_ioService.run();
    }

    std::string UnixSocketConnector::getServerPath() {
        const static std::vector<std::string> possiblePaths{
            getExecutablePath() + "/../libhidx/libhidx_server_daemon",
            getExecutablePath() + "/../libexec",
            "/usr/local/libexec",
            "/usr/libexec"
        };
        const static std::string executableName{"libhidx_server_daemon"};

        for(const auto& dir: possiblePaths){
            auto path = dir + '/' + executableName;
            if(access(path.c_str(), X_OK) != -1){
                return path;
            }
        }

        throw IOException{"Cannot find server binary!"};
    }

    UnixSocketConnector::~UnixSocketConnector() {
        m_process->kill();
    }

    bool UnixSocketConnector::connect() {

        std::string sockPath{m_socketDir};
        sockPath += "/";
        sockPath += SOCKET_FILENAME;
        local::endpoint ep{sockPath};

        local::socket socket(m_ioService);
        socket.connect(ep);

        m_socket = std::make_unique<asio::generic::stream_protocol::socket>(std::move(socket));
        return true;
    }

    std::string UnixSocketConnector::sendMessage(const std::string& message) {
        utils::writeMessage(*m_socket, message);
        return utils::readMessage(*m_socket);
    }

    std::string UnixSocketConnector::getExecutablePath() {
        char result[ PATH_MAX ];
#ifdef __FreeBSD__
        int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
        size_t count = sizeof( result );
        sysctl( mib, 4, result, &count, NULL, 0 );
        char *dir = dirname( result );
        count = strlen( dir );
#else
        ssize_t count = readlink( "/proc/self/cwd", result, PATH_MAX );
        char *dir = result;
#endif
        return std::string( dir, (count > 0) ? count : 0 );
    }
#endif
}
