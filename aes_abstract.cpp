#include "aes_abstract.hpp"

void aes_abstract::AddRoundKey() {
    //表示第current_round轮的密钥轮加，从0开始
    for (int k = 0; k < 4; k++) {
        uint32 current_key = round_key[4 * current_round + k];
        // printf("密钥：%x\n", current_key);
        // printf("密钥轮加前：%x %x %x %x\n", state[0][k], state[1][k],
        // state[2][k], state[3][k]);
        state[0][k] = state[0][k] ^ (uint8)(current_key >> 24);
        state[1][k] = state[1][k] ^ (uint8)((current_key & 0x00ff0000) >> 16);
        state[2][k] = state[2][k] ^ (uint8)((current_key & 0x0000ff00) >> 8);
        state[3][k] = state[3][k] ^ (uint8)(current_key);
        // printf("密钥轮加后：%x %x %x %x\n", state[0][k], state[1][k],
        // state[2][k], state[3][k]);
    }
    current_round++;
}

void aes_abstract::SubBytes() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->state[i][j] = SubByte(state[i][j]);
        }
    }
}

void aes_abstract::MixColumns() {
    uint8 new_state[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            uint8 n = 0;
            for (int k = 0; k < 4; k++) {
                n ^= FieldMul(mix_column_matrix[i][k], state[k][j]);
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

uint8 aes_abstract::FieldMul(uint8 a, uint8 b) {
    // 有限域上两者相乘，计算乘积，a为被乘数，b为乘数
    uint8 result = 0;
    uint8 most_significant_bit = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 0x01) {
            result = result ^ a;
        }
        most_significant_bit = 0x80 & a;
        a = a << 1;
        if (most_significant_bit != 0) {
            a = a ^ 0x1b;
        }
        b = b >> 1;
    }
    return result;
}
uint32 aes_abstract::bytes_to_word(uint8* bytes) {
    //将一个bytes数组的四个字节合成一个字
    uint32 number = 0;
    number = number | bytes[0];
    number = (number << 8) | bytes[1];
    number = (number << 8) | bytes[2];
    number = (number << 8) | bytes[3];
    return number;
}

void aes_abstract::KeyExpansion(uint32 initial_key[4]) {
    // 将初始的密钥分为四个字，密钥拓展为44个字
    int i;
    for (i = 0; i <= 3; i++) {
        this->round_key[i] = initial_key[i];
    }

    for (; i <= 43; i++) {
        uint32 temp = round_key[i - 1];
        if (i % 4 == 0) {
            temp = SubWord(RotWord(temp)) ^ r_cont[i / 4 - 1];
        }
        this->round_key[i] = this->round_key[i - 4] ^ temp;
    }
}

uint32 aes_abstract::RotWord(uint32 w) {
    // (B1, B2, B3, B4) --> (B2, B3, B4, B1)
    uint32 right = (w & 0xff000000) >> 24;
    // right为字的第一个byte，循环移位到最右边
    uint32 left = w << 8;
    return left | right;
}
uint32 aes_abstract::SubWord(uint32 w) {
    uint8* byte_array = (uint8*)&w;
    for (int i = 0; i < 4; i++) {
        uint8 one_char = byte_array[i];
        byte_array[i] = SubByte(byte_array[i]);
    }
    return w;
}

uint8 aes_abstract::SubByte(uint8 c) {
    int row = (0xf0 & c) >> 4;
    int col = (0x0f & c);
    return this->s_box[row][col];
}
void aes_abstract::general_roundkey(uint8 initial_key_byte[16]) {
    uint32 initial_key_word[4];
    for (int i = 0; i < 4; i++) {
        initial_key_word[i] = bytes_to_word(&initial_key_byte[4 * i]);
    }
    KeyExpansion(initial_key_word);
}


void aes_abstract::print_array(uint8 a[16]){
    for(int i = 0;i < 16;i++){
        printf("%x", a[i]);
    }
    printf("\n");
}

void aes_abstract::xor_group(uint8 a[16], uint8 b[16]){
    for(int i = 0;i < 16;i++){
        a[i] = a[i] ^ b[i];
    }
}


void aes_abstract::print_round_key(){
    for(int i = 0;i < 44;i++){
        printf("The RoundKey[%d]: 0x%x\n", i, round_key[i]);
    }
}
void aes_abstract::set_state(uint8 new_state[4][4]){
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            state[i][j] = new_state[i][j];
        }
    }
}
void aes_abstract::print_state(){
    printf("The current state:\n");
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            printf("%x ", state[i][j]);
        }
        printf("\n");
    }
}