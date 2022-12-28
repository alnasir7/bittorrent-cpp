#include <iostream>
#include <string>
#include <fstream>
#include "Bencode_parser.h"
#include "tracker_connect.h"

const std::string file_prefix = "./test_torrents/";
const std::string file_suffix = ".torrent";

int main(int argc, const char **argv)
{
    std::string file{argv[1]};
    std::string file_path = file_prefix + file + file_suffix;

    std::cout << file_path << "\n";
    Bencode_parser parser{file_path};

    connect(parser);

    // std::cout << " ---- " << announce << "   -----\n";
    return 0;
}