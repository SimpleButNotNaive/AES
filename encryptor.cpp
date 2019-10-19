#include "encryptor.hpp"

void aes_encryptor::ShiftRows() {
    int temp = state[1][0];
    for (int i = 1; i < 4; i++) {
        state[1][i - 1] = state[1][i];
    }
    state[1][3] = temp;

    temp = state[2][0];
    int temp2 = state[2][1];
    for (int i = 2; i < 4; i++) {
        state[2][i - 2] = state[2][i];
    }
    state[2][2] = temp;
    state[2][3] = temp2;

    //左移三位等加于右移一位
    temp = state[3][3];
    for (int i = 2; i >= 0; i--) {
        state[3][i + 1] = state[3][i];
    }
    state[3][0] = temp;
}

vector<vector<uint8>> aes_encryptor::encryption(string p_text, string key, string init_vector) {
    string total_plain_text = padding(p_text);
    vector<vector<uint8>> text_vector = split(total_plain_text);

    vector<vector<uint8>> cipher;

    uint8 key_array[16];
    for(int i = 0;i < key.size();i++){
        key_array[i] = key[i];
    }

    generate_roundkey(key_array);

    uint8 y[16];
    for(int i = 0;i < 16;i++){
        y[i] = init_vector[i];
    }

    uint8 x[16];
    for(auto group : text_vector){
        for(int i = 0;i < group.size();i++){
            x[i] = group[i];
        }
        xor_group(x, y);
        encrypt_one_group(x);

        for(int i = 0; i < 16;i++){
            y[i] = x[i];
        }
        
        vector<uint8> one_group;
        for(int i = 0;i < 16;i++){
            one_group.push_back(y[i]);
        }
        cipher.push_back(one_group);
    }
    return cipher;
}

vector<vector<uint8>> aes_encryptor::split(string s){
    int group_number = s.size() / 16;
    vector<vector<uint8>> ret;
    for(int i = 0;i < group_number;i++){
        vector<uint8> one_group;
        for(int j = 0;j < 16;j++){
            one_group.push_back(s[i * 16 + j]);
        }
        ret.push_back(one_group);
        one_group.clear();
    }
    return ret;
}
string aes_encryptor::padding(string s) {
    char padding[17];
    int pad_size;
    if (s.size() % 16 == 0) {
        pad_size = 16;
    }
    else{
        pad_size = 16 - (s.size() - (s.size() / 16) * 16);
    }

    for(int i = 0;i < pad_size;i++){
        padding[i] = pad_size;
    }
    padding[pad_size] = '\0';

    return s + padding;
}


void aes_encryptor::encrypt_one_group(uint8 p_text[16]) {
    current_round = 0;
    for (int i = 0; i < 16; i++) {
        state[i % 4][i / 4] = p_text[i];
    }
    AddRoundKey();

    for (int round = 1; round < NROUND; round++) {
        SubBytes();
        ShiftRows();
        MixColumns();
        AddRoundKey();
    }

    SubBytes();
    ShiftRows();
    AddRoundKey();

    for(int i = 0;i < 16;i++){
        p_text[i] = state[i % 4][i / 4];
    }
}