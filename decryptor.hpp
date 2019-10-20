#ifndef DECRYPTOR
#define DECRYPTOR

#include "aes_abstract.hpp"

class aes_decryptor : public aes_abstract{
public:

    void SubBytes();
    void MixColumns();

    uint8 SubByte(uint8);

    void ShiftRows();
    void reverse_round_key();
    string decryption(vector<vector<uint8>> cipher, string key, string init_vector);
    void splice(uint8*, uint8[16]);
    int truncate(uint8* p_text_array, int size);
    void decrypt_one_group(uint8 cipher[16]);
};

#endif
