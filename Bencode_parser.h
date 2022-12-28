#ifndef BENCODE_PARSER_H
#define BENCODE_PARSER_H


#include <string>
#include "bencode.hpp"

using map_t = bencode::map_proxy<std::string, bencode::data>;

class Bencode_parser
{
private:
    map_t data;

public:
    Bencode_parser(std::string file);
    std::string get_announce();
    map_t get_info();
};
#endif // !BENCODE_PARSER_H