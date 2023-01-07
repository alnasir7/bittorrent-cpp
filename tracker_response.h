#ifndef TRACKER_RESPONSE_H
#define TRACKER_RESPONSE_H
#include "Bencode_parser.h"
#include <vector>
#include <string>

namespace torrent::tracker
{
    struct Peer
    {
        std::string peer_id;
        std::string ip;
        num_t port;
    };
    struct tracker_response
    {
        num_t interval;
        num_t tracker_id;
        num_t complete;
        num_t incomplete;
        std::vector<Peer> peers;
        std::string client_id;
        std::string info_hash;
    };
}

#endif // !TRACKER_RESPONSE_H