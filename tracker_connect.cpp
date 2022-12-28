#include <string>
#include "Bencode_parser.h"
#include "bencode.hpp"
#include <chrono>
#include <ctime>
#include "sha1.h"

struct tracker_request
{
    std::string info_hash;
    std::string peer_id;
    std::string port;
    std::string uploaded;
    std::string left;
    std::string compact;

    tracker_request(std::string info_hash, num_t file_sizes) : info_hash{info_hash}, left{std::to_string(file_sizes)}
    {
        peer_id = "";
        // get current time
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto value = now_ms.time_since_epoch();
        long duration = value.count();

        peer_id += "-BX2022";

        for (int j = 0; j < 13; j++)
        {
            char digit = '0';
            if (duration > 0)
            {
                int rem = duration % 10;
                duration = duration / 10;
                digit = '0' + rem;
            }

            peer_id += digit;
        }

        // std::cout << peer_id << "\n";

        port = "6881";
        uploaded = "0";
        compact = "1";
    }
};

void connect(Bencode_parser &torrent)
{
    std::string url = torrent.get_announce();
    map_t info = torrent.get_info();
    std::string info_string = bencode::encode(info);
    std::string sha_info_string = sha1(info_string);

    auto files = torrent.get_files();
    num_t total_size = 0;

    for (auto file : files)
    {
        total_size++;
    }

    tracker_request request{sha_info_string, total_size};
}