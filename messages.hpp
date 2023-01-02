#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "utils.h"
#include <string>
#include <sstream>

const std::string PROTOCOL = "BitTorrent protocol";
const uint8_t PSTRLEN = 19;

enum message_id
{
    CHOKE,
    UNCHOKE,
    INTERESTED,
    NOT_INTERESTED,
    HAVE,
    BITFIELD,
    REQUEST,
    PIECE,
    CANCEL,
    PORT
};

struct message
{
    int len;
    message_id id;
    std::string payload;
    std::stringstream message_buffer;
    message(int len, message_id id, std::string payload) : len{len}, id{id}, payload{payload}, message_buffer{}
    {
        message_buffer << static_cast<char>(len);
        message_buffer << static_cast<char>(id);
    }

    std::string get()
    {
        return message_buffer.str();
    }
};

std::string generate_handshake(std::string info_hash, std::string peer_id)
{
    std::stringstream message_buffer;
    message_buffer << (char)PSTRLEN;
    message_buffer << PROTOCOL;
    std::string reserved;
    for (int i = 0; i < 8; i++)
        reserved.push_back('\0');
    message_buffer << reserved;
    message_buffer << (info_hash);
    message_buffer << peer_id;
    return message_buffer.str();
}

#endif // !MESSAGES_HPP
