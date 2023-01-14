#include "thread.h"
#include "messages.hpp"
#include "tracker_response.h"
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <chrono>
#include <sstream>
#include <string>
#include <iostream>
#include <thread>
#include <optional>
#include <vector>
#include <future> // std::async, std::future

namespace torrent
{
    torrent::thread::thread(int id, torrent::metadata metadata, downloader::downloader *downloader)
        : id{id}, metadata{metadata}, downloader{downloader}, has_peer{false}, state{UNCONNECTED}, pending_download{false}
    {
    }

    bool torrent::thread::set_peer()
    {
        std::optional<downloader::peer> new_peer_option = downloader->request_peer(id);
        if (new_peer_option.has_value())
        {
            peer = new_peer_option.value();
            return true;
        }
        else
        {
            return false;
        }
    }

    void torrent::thread::release_peer()
    {
        if (has_peer)
        {
            has_peer = false;
            downloader->release_peer(peer.id);
        }
    }

    bool torrent::thread::establish_peer_connection()
    {
        bool handshake_success = false;
        while (!handshake_success)
        {
            release_peer();
            if (!set_peer())
            {
                return false;
            }
            handshake_success = handshake();
        }

        std::cout << "successful handshake" << std::endl;
        // unchocke

        // interested
        return true;
    }

    void torrent::thread::receive_handler(std::vector<char> received_message)
    {
        // idea: write a function that reades some bytes from some sort
        // of char array / stream that has iterators. Make it a template
        // function using concepts
        uint8_t first_bit = received_message[0];
        if (first_bit == 19)
        {
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

    void torrent::thread::receive_message()
    {
        std::cout << "calling receive_message" << std::endl;
        char buffer[5];
        asio::read(*socket, asio::buffer(buffer, 5));
        // make bytesToInts template
        std::stringstream buffer_stream;
        for (int i = 0; i < 4; i++)
        {
            buffer_stream << buffer[i];
        }
        int len = bytesToInt(buffer_stream.str());
        std::stringstream id_buffer_stream;
        id_buffer_stream << buffer[4];
        int message_id = bytesToInt(id_buffer_stream.str());

        std::cout << "len: " << len << ", message_id=" << message_id << std::endl;
    }

    bool torrent::thread::start()
    {
        if (!establish_peer_connection())
        {
            return false;
        }

        while (state != DONE)
        {
            if (!pending_download)
            {
                // TODO: Make piece request
                // pending_download = true
            }
            receive_message();
            break;
        }

        return true;
    }

    bool torrent::thread::handle_handshake(std::vector<char> received_message)
    {
        std::stringstream rec_info_sha_binary;
        for (int i = 28, j = 0; j < 20; i++, j++)
        {
            rec_info_sha_binary << received_message[i];
        }

        std::string rec_info_sha = rec_info_sha_binary.str();

        if (rec_info_sha == metadata.info_hash)
        {
            std::cout << "info sha matched" << std::endl;
            return true;
        }
        else
        {
            std::cout << "info sha not matching" << std::endl;
            return false;
        }
    }

    bool torrent::thread::handshake()
    {
        try
        {
            socket.reset();
            context.reset();
            endpoint.reset();

            endpoint = std::make_shared<asio::ip::tcp::endpoint>(asio::ip::make_address(peer.address), peer.port);

            context = std::make_shared<asio::io_context>();

            socket = std::make_shared<asio::ip::tcp::socket>(*context);

            std::chrono::milliseconds span(100);
            std::chrono::milliseconds zero_s(0);

            std::future<void> connect_status = socket->async_connect(*endpoint, asio::use_future);
            context->run_for(span);

            try
            {
                if (connect_status.wait_for(zero_s) == std::future_status::timeout)
                    return false;

                connect_status.get();
            }
            catch (const std::exception &e)
            {
                std::cout << "caught" << std::endl;
            }

            std::string handshake_string = generate_handshake(metadata.info_hash, metadata.client_id);
            socket->write_some(asio::buffer(handshake_string.data(), handshake_string.length()));

            std::vector<char> read_buffer(68);
            asio::read(*socket, asio::buffer(read_buffer.data(), read_buffer.size()));
            handle_handshake(read_buffer);

            return true;
        }
        catch (const std::exception &e)
        {
            std::cout << "-------- caught ------" << std::endl;
            return false;
        }
    }
} // namespace torrent

/*
#include "messages.hpp"
#include "tracker_response.h"
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

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