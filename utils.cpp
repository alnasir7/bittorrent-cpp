#include <string>
#include <bitset>

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

int bytesToInt(std::string bytes)
{
    // FIXME: Use bitwise operation to convert
    std::string binStr;
    long byteCount = bytes.size();
    for (int i = 0; i < byteCount; i++)
        binStr += std::bitset<8>(bytes[i]).to_string();
    return stoi(binStr, 0, 2);
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