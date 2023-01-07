#include "torrent_downloader.h"
#include "Bencode_parser.h"
#include "tracker_connect.h"
#include "tracker_response.h"
#include "torrent_downloader.h"
#include "peers_connect.h"
#include <iostream>

namespace torrent::downloader
{
    downloader::downloader(std::string file)
    {
        Bencode_parser parser{file};
        std::optional<torrent::tracker::tracker_response> response = torrent::tracker::connect(parser);

        if (!response.has_value())
        {
            std::cout << "failed to connect \n";
            exit(0);
        }
        else
        {
            std::cout << "successfully connected\n";
        }
    }
}