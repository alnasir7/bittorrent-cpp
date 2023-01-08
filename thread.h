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
        bool has_peer;
        downloader::peer peer;
        metadata metadata;
        downloader::downloader *downloader;

    public:
        thread(int id, torrent::metadata metadata, downloader::downloader *downloader);
        bool start();
        bool handle_handshake(std::vector<char> received_message);
        bool handshake();
        bool connect();
        bool establish_peer_connection();
        bool set_peer();
        void release_peer();
    };

}

#endif // !DOWNLOAD_THREAD_H