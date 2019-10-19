#include "decryptor.hpp"

void aes_decryptor::ShiftRows() {
    int temp = state[1][3];
    for (int i = 3; i >= 1; i--) {
        state[1][i] = state[1][i - 1];
    }
    state[1][0] = temp;

    temp = state[2][0];
    int temp2 = state[2][1];
    for (int i = 2; i < 4; i++) {
        state[2][i - 2] = state[2][i];
    }
    state[2][2] = temp;
    state[2][3] = temp2;
    ;

    //右移三位等于左移一位
    temp = state[3][0];
    for (int i = 0; i <= 2; i++) {
        state[3][i] = state[3][i + 1];
    }
    state[3][3] = temp;
}
string aes_decryptor::decryption(vector<vector<uint8>> cipher, string key,
                               string init_vector) {
    uint8 key_array[16];
    for (int i = 0; i < key.size(); i++) {
        key_array[i] = key[i];
    }
    generate_roundkey(key_array);
    reverse_round_key();

    uint8 pre_y[16];
    for(int i = 0;i < init_vector.size();i++){
        pre_y[i] = init_vector[i];
    }

    uint8 post_y[16];
    uint8 current_y[16];
    uint8* p_text_array = new uint8[16*cipher.size()];
    for(auto group: cipher){
        for(int i = 0;i < 16;i++){
            post_y[i] = group[i];
            current_y[i] = group[i];
        }
        decrypt_one_group(current_y);

        xor_group(current_y, pre_y);
        splice(p_text_array, current_y);

        for(int i = 0;i < 16;i++){
            pre_y[i] = post_y[i];
        }
    }

    truncate(p_text_array, cipher.size() * 4);

    string ret(p_text_array, p_text_array + cipher.size() * 16);

    delete p_text_array;

    return ret;
}

void aes_decryptor::truncate(uint8* p_text_array, int size){
    int truncate_size = p_text_array[size - 1];
    p_text_array[size - truncate_size] = '\0';
}

void aes_decryptor::splice(uint8* p_text_array, uint8 group[16]){
    static int cursor = 0;
    for(int i = 0;i < 16;i++){
        p_text_array[cursor] = group[i];
        cursor++;
    }
}

void aes_decryptor::reverse_round_key() {
    int i = 0, j = 10;
    uint32 temp;
    while (i < j) {
        for(int k = 0;k < 4;k++){
            swap(round_key[4*i + k], round_key[4*j + k]);
        }
        i++;
        j--;
    }
}

void swap(uint32& a, uint32& b){
    int temp = a;
    a = b;
    b = temp;
}
void aes_decryptor::decrypt_one_group(uint8 cipher[16]) {
    current_round = 0;
    for (int i = 0; i < 16; i++) {
        state[i % 4][i / 4] = cipher[i];
    }

    AddRoundKey();
    for (int i = 1; i < NROUND; i++) {
        ShiftRows();
        SubBytes();
        AddRoundKey();
        MixColumns();
    }

    ShiftRows();
    SubBytes();
    AddRoundKey();

    for (int i = 0; i < 16; i++) {
        cipher[i] = state[i % 4][i / 4];
    }
}
void aes_decryptor::SubBytes() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->state[i][j] = SubByte(state[i][j]);
        }
    }
}
uint8 aes_decryptor::SubByte(uint8 c){
    int row = (0xf0 & c) >> 4;
    int col = (0x0f & c);
    return this->s_box_inv[row][col];
}

void aes_decryptor::MixColumns() {
    uint8 new_state[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            uint8 n = 0;
            for (int k = 0; k < 4; k++) {
                n ^= FieldMul(mix_column_matrix_inv[i][k], state[k][j]);
            }
            new_state[i][j] = n;
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = new_state[i][j];
        }
    }
}