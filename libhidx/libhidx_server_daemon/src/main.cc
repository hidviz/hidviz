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

#include <libhidx/server/Server.hh>
#include <iostream>

#include "args.hh"

int main(int argc, char* argv[]){

    args::ArgumentParser parser{"Daemon"};
    args::Group modes{parser, "", args::Group::Validators::Xor};
    args::Group unixSocketGroup{modes, "", args::Group::Validators::All};
    args::Group unixSocketRequiredGroup{unixSocketGroup, "", args::Group::Validators::All};
    args::Flag unixSocket{unixSocketRequiredGroup, "u", "", {'u'}};
    args::Positional<std::string> socketDirectory{unixSocketRequiredGroup, "", ""};

    args::Group unixSocketOptionalGroup{unixSocketGroup, ""};
    args::Flag watchParent{unixSocketOptionalGroup, "p", "", {'p'}};


    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    } catch (const args::ValidationError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    libhidx::server::run(socketDirectory.Get(), watchParent);
    return 0;
}
