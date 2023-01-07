#ifndef TORRENT_DOWNLOADER_H
#define TORRENT_DOWNLOADER_H

#include <string>
#include <queue>

using num_t = long long;

namespace torrent::downloader
{
    struct peer
    {
        std::string address;
        int id;
        int port;
        int thread_in_charge; //-1 => available
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
        num_t interval;
        num_t tracker_id;
        num_t complete;
        num_t incomplete;
        std::string client_id;
        std::string info_hash;
        std::vector<piece> pieces;

    public:
        downloader(std::string path);
        void download(int status);
    };
}
#endif // !TORRENT_DOWNLOADER_H