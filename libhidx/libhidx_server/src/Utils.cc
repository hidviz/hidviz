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

#include <libhidx/server/Utils.hh>

#include <google/protobuf/message.h>

using generic = asio::generic::stream_protocol;

constexpr size_t LENGTH_LENGTH = 8;
constexpr size_t LENGTH_ID = 3;

namespace libhidx {
namespace utils {

    std::string readFromSocket(generic::socket& socket, const size_t length) {
        asio::streambuf buffer;
        auto received = asio::read(socket, buffer.prepare(length));
        if (received != length) {
            //TODO
            throw;
        }
        buffer.commit(length);
        return {asio::buffers_begin(buffer.data()), asio::buffers_end(buffer.data())};
    }

    std::string readMessage(generic::socket& stream) {
        auto lengthStr = readFromSocket(stream, LENGTH_LENGTH);
        auto length = std::stoul(lengthStr);

        auto message = readFromSocket(stream, length);

        return message;

    }
    void writeMessage(generic::socket& socket, const std::string& message){
        auto length = std::to_string(message.length());

        length.insert(0, LENGTH_LENGTH - length.length(), '0');

        asio::write(socket, asio::buffer(length));
        asio::write(socket, asio::buffer(message));
    }

    std::string packMessage(MessageId messageId, const std::string& payload){
        auto id = static_cast<unsigned>(messageId);
        auto idStr = std::to_string(id);

        idStr.insert(0, LENGTH_ID - idStr.length(), '0');

        return idStr + payload;
    }

    std::pair<MessageId, std::string> unpackMessage(const std::string& message){
        std::string idStr{std::begin(message), std::begin(message) + LENGTH_ID};

        auto id = std::stoul(idStr);
        auto messageId = static_cast<MessageId>(id);

        return {messageId, {std::begin(message) + LENGTH_ID, std::end(message)}};
    };
}}
