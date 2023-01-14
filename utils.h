#ifndef UTILS_H
#define UTILS_H

#include <string>

unsigned char hexval(unsigned char c);
std::string decode_hex(const std::string &in);
int bytesToInt(std::string bytes);

#endif // !UTILS_H
