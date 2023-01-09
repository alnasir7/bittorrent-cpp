#include <iostream>
#include <string>
#include <fstream>
#include <optional>
#include "torrent_downloader.h"
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
    torrent::downloader::downloader downloader{file_path, 2};
    int torrent_status;
    downloader.download(torrent_status);

    // Peer p = response.value().peers[18];

    // handshake(p, response.value().info_hash, response.value().client_id);

    // connect to peers and establish what pieces they have

    // std::cout << " ---- " << announce << "   -----\n";
    return 0;
}