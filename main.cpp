#include <iostream>
#include <string>
#include <fstream>
#include "Bencode_parser.h"
#include "tracker_connect.h"

const std::string filepath = "./sintel.torrent";

int main(int argc, const char **argv)
{
    Bencode_parser parser{filepath};

    connect(parser);

    // std::cout << " ---- " << announce << "   -----\n";
    return 0;
}