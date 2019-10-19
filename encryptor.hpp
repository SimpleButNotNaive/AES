#include "aes_abstract.hpp"

#ifndef ENCRYPTOR
#define ENCRYPTOR

class aes_encryptor : public aes_abstract{
public:
    void ShiftRows();
    void encryption(string p_text, string key, string init_vector);
    string padding(string s);
    vector<vector<uint8>> split(string s);
    void encrypt_one_group(uint8 p_text[16]);
};


#endif