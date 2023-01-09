#ifndef TORRENT_DOWNLOADER_H
#define TORRENT_DOWNLOADER_H

#include <string>
#include <queue>
#include <optional>
#include <mutex>

using num_t = long long;

namespace torrent
{
    struct metadata
    {
        num_t interval;
        num_t tracker_id;
        num_t complete;
        num_t incomplete;
        std::string client_id;
        std::string info_hash;
    };
};

namespace torrent::downloader
{
    struct peer
    {
        std::string address;
        int id;              // internal
        std::string peer_id; // external
        int port;
        int thread_in_charge; // -1 => available
    };

    struct piece
    {
        int id;
        bool downloaded;
        bool written;
    };

    class downloader
    {
    private:
        std::vector<peer> peers;
        int num_threads;
        torrent::metadata metadata;
        std::vector<piece> pieces;
        std::mutex peers_mutex;

    public:
        downloader(std::string path, int num_threads);
        void download(int status);
        std::optional<peer> request_peer(int id);
        void release_peer(int peer_id);
    };
}
#endif // !TORRENT_DOWNLOADER_H