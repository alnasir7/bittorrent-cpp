#include "Bencode_parser.h"
#include "tracker_response.h"
#include <optional>
std::optional<tracker_response> connect(Bencode_parser &torrent);