#include "tracker_response.h"
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include "messages.hpp"
#include <asio.hpp>
#include <asio/ts/internet.hpp>
#include <asio/ts/buffer.hpp>

bool handshake(Peer peer, std::string info_hash, std::string client_id)
{

    std::cout << peer.ip << ":" << peer.port << std::endl;
    asio::error_code ec{};

    asio::ip::tcp::endpoint endpoint(asio::ip::make_address(peer.ip, ec), peer.port);

    if (ec)
    {
        std::cout << ec.message() << std::endl;
    }
    else
    {
        std::cout << "created" << std::endl;
    }

    std::cout << "begin" << std::endl;

    asio::io_context context;

    asio::ip::tcp::socket socket{context};

    std::cout << "begin" << std::endl;

    socket.connect(endpoint, ec);

    std::cout << "about to connect" << std::endl;

    if (ec)
    {
        std::cout << ec.message() << std::endl;
    }
    else
    {
        std::cout << "connected\n";
    }

    std::string handshake_string = generate_handshake(info_hash, client_id);
    std::cout << handshake_string << std::endl;
    socket.write_some(asio::buffer(handshake_string.data(), handshake_string.length()), ec);

    if (ec)
    {
        std::cout << ec.message() << std::endl;
    }

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(900ms);

    socket.wait(socket.wait_read);

    size_t num_bytes = socket.available();

    std::cout << "bytes" << num_bytes << std::endl;

    std::vector<char> read_buffer(num_bytes);

    if (num_bytes > 0)
    {
        socket.read_some(asio::buffer(read_buffer.data(), read_buffer.size()), ec);
        for (auto c : read_buffer)
            std::cout << c;
    }

    return true;
}