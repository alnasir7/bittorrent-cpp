#include "torrent_downloader.h"
#include "Bencode_parser.h"
#include "tracker_connect.h"
#include "tracker_response.h"
#include "torrent_downloader.h"
#include "peers_connect.h"
#include <iostream>
#include <thread>

namespace torrent::downloader
{
    downloader::downloader(std::string file, int num_threads) : num_threads{num_threads}
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

    void downloader::download(int status)
    {
        std::thread threads[num_threads];
        for (int i = 0; i < num_threads; i++)
        {
            // std::thread thread ()
        }
    }
}