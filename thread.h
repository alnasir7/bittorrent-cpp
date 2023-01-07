#ifndef DOWNLOAD_THREAD_H
#define DOWNLOAD_THREAD_H

#include "torrent_downloader.h"
#include "tracker_response.h"
#include <string>
// bool handshake(torrent::tracker::Peer peer, std::string info_hash, std::string client_id);
// void receiver(std::vector<char> received_message);

namespace torrent
{
    class thread
    {
    private:
        int id;
        int peer_id;
        downloader::peer peer;
        metadata metadata;

    public:
        thread(int id, int peer_id, downloader::peer peer, torrent::metadata metadata);
        bool start();
        bool handle_handshake(std::vector<char> received_message);
        bool handshake();
    };

}

#endif // !DOWNLOAD_THREAD_H