#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "decryptflag.h"

std::string decryptFlag ( std::string encryptedFlag )
{
    char key[8] = {0x70, 0x52, 0x12, 0x66, 0x7c, 0x0a, 0x54, 0x3e};
    int size = encryptedFlag.size() / 2;
    if (size % 8 != 0)
        throw std::length_error("wrong length");
    char* Flag = new char[size+1];
    Flag[size]=0;
    for ( int i = 0; i < size; i++ )
        Flag[i] = std::stoi ( std::string ( encryptedFlag, i * 2, 2 ), 0, 16 ) ^ key[i%8];
    std::string decryptedFlag ( Flag );
    delete Flag;
    return decryptedFlag;
}
