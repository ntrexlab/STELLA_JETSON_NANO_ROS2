#include "MwIEEE754.hpp"

void Binary2Hex( std::string Binary, int8_t hex_[4])
{
    std::bitset<32> set(Binary);      
    int hex = set.to_ulong();

    hex_[0] = (hex);
    hex_[1] = (hex >> 8);
    hex_[2] = (hex >> 16);
    hex_[3] = (hex >> 24);
}

std::string GetBinary32( float value )
{
    union
    {
         float input; 
         int   output;
    }    data;
 
    data.input = value;
 
    std::bitset<sizeof(float) * CHAR_BIT>   bits(data.output);
 
    std::string mystring = bits.to_string<char, std::char_traits<char>, std::allocator<char> >();
 
    return mystring;
}
 