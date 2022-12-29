#include <string>
#include "Bencode_parser.h"
#include "sha1.h"
unsigned char hexval(unsigned char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    else if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    else
        return '\0';
}

std::string decode_hex(const std::string &in)
{
    std::string out;
    out.clear();
    out.reserve(in.length() / 2);
    for (std::string::const_iterator p = in.begin(); p != in.end(); p++)
    {
        unsigned char c = hexval(*p);
        p++;
        if (p == in.end())
            break;
        c = (c << 4) + hexval(*p);
        out.push_back(c);
    }

    return out;
}

std::string get_info_hash(map_t &info)
{
    std::string info_string = bencode::encode(info);
    std::string sha_info_string = decode_hex(sha1(info_string));
    return sha_info_string;
}