#ifndef BENCODE_PARSER_H
#define BENCODE_PARSER_H

#include <string>
#include "bencode.hpp"

using map_t = bencode::map_proxy<std::string, bencode::data>;
using list_t = std::vector<bencode::data>;
using num_t = long long;

struct File
{
    num_t length;
    std::vector<std::string> path;
    std::string name;

    File(num_t length, std::vector<std::string> path, std::string name);
};

class Bencode_parser
{
private:
    map_t data;

public:
    Bencode_parser(std::string file);
    std::string get_announce();
    map_t get_info();
    std::vector<File> get_files();
};
#endif // !BENCODE_PARSER_H