#include "torrent_downloader.h"
#include "Bencode_parser.h"
#include "tracker_connect.h"
#include "tracker_response.h"
#include "torrent_downloader.h"
#include "peers_connect.h"
#include <iostream>
#include <thread>
#include <optional>
#include <mutex>
#include "thread.h"

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
            torrent::tracker::tracker_response response_value = response.value();
            metadata.client_id = response_value.client_id;
            metadata.complete = response_value.complete;
            metadata.incomplete = response_value.incomplete;
            metadata.info_hash = response_value.info_hash;
            metadata.interval = response_value.interval;

            peers = std::vector<peer>(response_value.peers.size());
            for (int i = 0; i < peers.size(); i++)
            {
                peers[i].address = response_value.peers[i].ip;
                peers[i].port = response_value.peers[i].port;
                peers[i].peer_id = response_value.peers[i].peer_id;
                peers[i].id = i;
                peers[i].thread_in_charge = -1;
            }
        }
    }

    void downloader::download(int status)
    {
        std::thread threads[num_threads];
        for (int i = 0; i < num_threads; i++)
        {
            torrent::thread torrent_thread{i, metadata, this};
            std::thread thread(&torrent::thread::start, torrent_thread);
            threads[i] = std::move(thread);
        }

        for (int i = 0; i < num_threads; i++)
        {
            threads[i].join();
        }
    }

    std::optional<peer> downloader::request_peer(int id)
    {
        std::scoped_lock lock{peers_mutex};
        for (int i = 0; i < peers.size(); i++)
        {
            if (peers[i].thread_in_charge == -1)
            {
                peers[i].thread_in_charge = id;
                return std::optional<peer>{peers[i]};
            }
        }
        return std::nullopt;
    }

    void downloader::release_peer(int peer_id)
    {
        std::scoped_lock lock{peers_mutex};
        peers[peer_id].thread_in_charge = -2; // can not be reused. Once a thread lets go of a peer
        // it is either because it is done downloading all of its pieces or because it couldn't connect
        // to it, therefore the peer should not be used again
    }
}