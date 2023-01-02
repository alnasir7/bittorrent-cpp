#include <string>
#include "Bencode_parser.h"
#include "bencode.hpp"
#include <chrono>
#include <optional>
#include <ctime>
#include "sha1.h"
#include "info_hash.h"
#include "tracker_response.h"
#include "bencode.hpp"
#include <cpr/cpr.h>

// std::string hexDecode(const std::string &value)
// {
//     int hashLength = value.length();
//     std::string decodedHexString;
//     for (int i = 0; i < hashLength; i += 2)
//     {
//         std::string byte = value.substr(i, 2);
//         char c = (char)(int)strtol(byte.c_str(), nullptr, 16);
//         decodedHexString.push_back(c);
//     }
//     return decodedHexString;
// }

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
    std::string info_hash = get_info_hash(info);

    // std::cout << "info hash: " << info_hash << std::endl;
    // std::cout << "url: " << url << std::endl;

    auto files = torrent.get_files();
    num_t total_size = 0;

    for (auto file : files)
    {
        total_size += file.length;
    }

    return tracker_request{info_hash, total_size, url};
}

std::vector<Peer> parse_peers(std::string &peers_binary)
{
    std::vector<Peer> result{};
    int p = 0;
    while (p < peers_binary.size() - 5)
    {
        uint8_t ip_1 = peers_binary[p++];
        uint8_t ip_2 = peers_binary[p++];
        uint8_t ip_3 = peers_binary[p++];
        uint8_t ip_4 = peers_binary[p++];

        std::string ip_address = std::to_string(ip_1) + "." + std::to_string(ip_2) + "." + std::to_string(ip_3) + "." + std::to_string(ip_4);

        // std::cout << "ip: " << ip_address << std::endl;

        uint8_t port_1 = peers_binary[p++];
        uint8_t port_2 = peers_binary[p++];

        int port = port_1 * 100 + port_2;

        Peer peer{"", ip_address, port};

        result.push_back(peer);
    }

    return result;
}

tracker_response parse_tracker_response(std::string r)
{
    // std::cout << peers_response;
    tracker_response response;
    auto res_variant = bencode::decode(r);
    map_t res = std::get<map_t>(res_variant);

    // parse the response data one by one

    auto interval_variant = res.find("interval");
    auto interval = std::get<num_t>(interval_variant->second);
    response.interval = interval;

    // auto tracker_id_variant = res.find("tracker id");
    // auto tracker_id = std::get<num_t>(tracker_id_variant->second);
    // response.tracker_id = tracker_id;

    auto complete_variant = res.find("complete");
    auto complete = std::get<num_t>(complete_variant->second);
    response.complete = complete;

    auto incomplete_variant = res.find("incomplete");
    auto incomplete = std::get<num_t>(incomplete_variant->second);
    response.incomplete = incomplete;

    auto peers_variant = res.find("peers");
    auto peers_list = std::get<std::string>(peers_variant->second);

    auto peers = parse_peers(peers_list);

    response.peers = peers;

    return response;
}

std::optional<tracker_response> get_response(tracker_request &request_info)
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
                               params);
    if (r.status_code == 200)
        return std::optional<tracker_response>{parse_tracker_response(r.text)};
    else
        return std::nullopt;
}

std::optional<tracker_response> connect(Bencode_parser &torrent)
{
    tracker_request request = generate_request(torrent);
    std::optional<tracker_response> response = get_response(request);
    return response;
}