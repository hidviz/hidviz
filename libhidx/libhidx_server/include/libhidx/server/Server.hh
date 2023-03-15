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

#ifndef PROJECT_SERVER_HH_HH
#define PROJECT_SERVER_HH_HH

#include <string>

namespace libhidx {
namespace server {
    /**
     * Runs server to process requests given via unix domain socket.
     * @param sockDir Directory where socket should be created.
     * @param watchParent Specifies if server should close when its parent process dies.
     */
    void run(std::string sockDir, bool watchParent);

    /**
     * Processes one request and returns response.
     * @param request Request to be processed.
     * @return Response to given request
     */
    std::string cmd(const std::string& request);

}
}

#endif //PROJECT_SERVER_HH_HH
