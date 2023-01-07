
#ifndef TRACKER_CONNECT_H
#define TRACKER_CONNECT_H
#include "Bencode_parser.h"
#include "tracker_response.h"
#include <optional>
namespace torrent::tracker
{
    std::optional<torrent::tracker::tracker_response> connect(Bencode_parser &torrent);
}
#endif // !TRACKER_CONNECT_H