#include <iostream>
#include <string>
#include <fstream>
#include <optional>
#include "Bencode_parser.h"
#include "tracker_connect.h"
#include "tracker_response.h"
#include "peers_connect.h"
#include <sys/socket.h>
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/internet.hpp>
#include <asio/ts/buffer.hpp>

const std::string file_prefix = "./test_torrents/";
const std::string file_suffix = ".torrent";

int main(int argc, const char **argv)
{
    std::string file{argv[1]};
    std::string file_path = file_prefix + file + file_suffix;
    Bencode_parser parser{file_path};
    std::optional<tracker_response> response = connect(parser);

    if (!response.has_value())
    {
        std::cout << "failed to connect \n";
        exit(0);
    }

    Peer p = response.value().peers[6];

    handshake(p, response.value().info_hash, response.value().client_id);

    // connect to peers and establish what pieces they have

    // std::cout << " ---- " << announce << "   -----\n";
    return 0;
}