#include "Bencode_parser.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

File::File(num_t length, std::vector<std::string> path, std::string name) : length{length}, path{path}, name{name}
{
}

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

std::vector<File> Bencode_parser::get_files()
{
    auto variant = data.find("info");
    auto info = std::get<map_t>(variant->second);
    auto files_variant = info.find("files");
    std::vector<File> result;

    try
    {
        auto files = std::get<list_t>(files_variant->second);
        for (auto file_variant : files)
        {
            auto file = std::get<map_t>(file_variant);
            auto length_variant = file.find("length");
            auto length = std::get<num_t>(length_variant->second);
            auto path_variant = file.find("path");
            auto path = std::get<list_t>(path_variant->second);
            std::vector<std::string> path_vector{};

            for (auto p_variant : path)
            {
                auto p = std::get<std::string>(p_variant);
                path_vector.push_back(p);
            }

            File f{length, path_vector, "NA"};
            result.push_back(f);
        }
    }
    catch (std::bad_variant_access ex)
    {
        auto length_variant = info.find("length");
        auto length = std::get<num_t>(length_variant->second);
        auto name_variant = info.find("name");
        auto name = std::get<std::string>(name_variant->second);
        std::vector<std::string> path_vector{};
        File f{length, path_vector, name};
        result.push_back(f);
    }
    return result;
}
