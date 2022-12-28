#include <string>
#include "Bencode_parser.h"
#include "bencode.hpp"
#include <chrono>
#include <ctime>
#include "sha1.h"
#include <cpr/cpr.h>

struct tracker_request
{
    std::string url;
    std::string info_hash;
    std::string peer_id;
    std::string port;
    std::string uploaded;
    std::string downloaded;
    std::string left;
    std::string compact;

    tracker_request(std::string info_hash, num_t file_sizes, std::string url) : info_hash{info_hash}, left{std::to_string(file_sizes)}, downloaded{"0"}, uploaded{"0"}, compact{"1"}, port{"6881"}, url{url}
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
    }
};

tracker_request generate_request(Bencode_parser &torrent)
{
    std::string url = torrent.get_announce();
    map_t info = torrent.get_info();
    std::string info_string = bencode::encode(info);
    std::string sha_info_string = sha1(info_string);

    std::cout << "info hash: " << sha_info_string << std::endl;

    auto files = torrent.get_files();
    num_t total_size = 0;

    for (auto file : files)
    {
        total_size += file.length;
    }

    return tracker_request{sha_info_string, total_size, url};
}

void get_peers(tracker_request &request_info)
{

    cpr::Parameters params{
        {"info_hash", request_info.info_hash},
        {"peer_id", request_info.peer_id},
        {"port", request_info.port},
        {"uploaded", request_info.uploaded},
        {"downloaded", request_info.downloaded},
        {"left", request_info.left},
        {"compact", request_info.compact}};

    cpr::Response r = cpr::Get(cpr::Url{request_info.url},
                               cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
                               params);

    std::cout << r.text;
}

void connect(Bencode_parser &torrent)
{
    tracker_request request = generate_request(torrent);
    get_peers(request);
}