#include "Bencode_parser.h"
#include <iostream>
#include <string>
#include <fstream>

Bencode_parser::Bencode_parser(std::string file)
{
    std::ifstream torrent(file, std::ifstream::binary);
    auto bencode_data = bencode::decode(torrent);
    this->data = std::get<map_t>(bencode_data);
}

std::string Bencode_parser::get_announce()
{
    auto variant = data.find("announce");
    auto announce = std::get<std::string>(variant->second);
    return announce;
}

map_t Bencode_parser::get_info()
{
    auto variant = data.find("info");
    auto info = std::get<map_t>(variant->second);
    return info;
}