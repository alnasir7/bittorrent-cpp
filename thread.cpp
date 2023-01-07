#include "thread.h"
#include "tracker_response.h"
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <sstream>
#include "messages.hpp"
#include <asio.hpp>
#include <asio/ts/internet.hpp>
#include <asio/ts/buffer.hpp>
namespace torrent
{
    torrent::thread::thread(int id, int peer_id, downloader::peer peer) : id{id}, peer_id{peer_id}, peer{peer}
    {
    }

    bool torrent::thread::start()
    {
    }

    bool torrent::thread::handshake()
    {

        std::cout << peer.ip << ":" << peer.port << std::endl;
        asio::error_code ec{};

        asio::ip::tcp::endpoint endpoint(asio::ip::make_address(peer.ip, ec), peer.port);

        if (ec)
        {
            std::cout << ec.message() << std::endl;
            return false;
        }

        asio::io_context context;

        asio::ip::tcp::socket socket{context};

        socket.connect(endpoint, ec);

        if (ec)
        {
            std::cout << ec.message() << std::endl;
        }
        else
        {
            std::cout << "connected\n";
        }

        std::string handshake_string = generate_handshake(info_hash, client_id);
        socket.write_some(asio::buffer(handshake_string.data(), handshake_string.length()), ec);

        if (ec)
        {
            std::cout << ec.message() << std::endl;
        }

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(100ms);

        socket.wait(socket.wait_read);

        size_t num_bytes = socket.available();

        std::vector<char> read_buffer(num_bytes);

        if (num_bytes > 0)
        {
            socket.read_some(asio::buffer(read_buffer.data(), read_buffer.size()), ec);
            receiver(read_buffer, info_hash);
        }

        return true;
    }
}

/*
#include "tracker_response.h"
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <sstream>
#include "messages.hpp"
#include <asio.hpp>
#include <asio/ts/internet.hpp>
#include <asio/ts/buffer.hpp>

void handle_handshake(std::vector<char> received_message, std::string info_sha)
{
    std::stringstream rec_info_sha_binary;
    for (int i = 28, j = 0; j < 20; i++, j++)
    {
        rec_info_sha_binary << received_message[i];
    }

    std::string rec_info_sha = rec_info_sha_binary.str();

    if (rec_info_sha == info_sha)
    {
        std::cout << "matching" << std::endl;
    }
    else
    {
        std::cout << "no match" << std::endl;
    }

    if (received_message[received_message.size() - 1] == '\0')
    {
        std::cout << "confirm" << std::endl;
    }
}

void receiver(std::vector<char> received_message, std::string info_hash)
{
    // idea: write a function that reades some bytes from some sort
    // of char array / stream that has iterators. Make it a template
    // function using concepts
    uint8_t first_bit = received_message[0];
    if (first_bit == 19)
    {
        handle_handshake(received_message, info_hash);
        return;
    }
    int len = 0;
    for (int i = 0; i < 32; i++)
    {
        uint8_t byte = received_message[i];
        std::cout << byte << std::endl;
        len = (len << 8) | byte;
    }
    std::cout << len << std::endl;
}

bool handshake(torrent::tracker::Peer peer, std::string info_hash, std::string client_id)
{

    std::cout << peer.ip << ":" << peer.port << std::endl;
    asio::error_code ec{};

    asio::ip::tcp::endpoint endpoint(asio::ip::make_address(peer.ip, ec), peer.port);

    if (ec)
    {
        std::cout << ec.message() << std::endl;
        return false;
    }

    asio::io_context context;

    asio::ip::tcp::socket socket{context};

    socket.connect(endpoint, ec);

    if (ec)
    {
        std::cout << ec.message() << std::endl;
    }
    else
    {
        std::cout << "connected\n";
    }

    std::string handshake_string = generate_handshake(info_hash, client_id);
    socket.write_some(asio::buffer(handshake_string.data(), handshake_string.length()), ec);

    if (ec)
    {
        std::cout << ec.message() << std::endl;
    }

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);

    socket.wait(socket.wait_read);

    size_t num_bytes = socket.available();

    std::vector<char> read_buffer(num_bytes);

    if (num_bytes > 0)
    {
        socket.read_some(asio::buffer(read_buffer.data(), read_buffer.size()), ec);
        receiver(read_buffer, info_hash);
    }

    return true;
}

*/