#include "aes.hpp"

uint8 aes_encryption::s_box[16][16] = {
	{0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
	{0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
	{0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
	{0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
	{0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
	{0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
	{0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
	{0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
	{0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
	{0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
	{0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
	{0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
	{0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
	{0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
	{0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
	{0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
};

uint32 aes_encryption::r_cont[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 
				 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};

uint8 aes_encryption::mix_column_matrix[4][4] = {
	{0x02, 0x03, 0x01, 0x01},
	{0x01, 0x02, 0x03, 0x01},
	{0x01, 0x01, 0x02, 0x03},
	{0x03, 0x01, 0x01, 0x02},
};

void aes_encryption::AddRoundKey() {
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

void aes_encryption::SubBytes() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->state[i][j] = SubByte(state[i][j]);
        }
    }
}

void aes_encryption::ShiftRows() {
    int temp = state[1][0];
    for (int i = 1; i < 4; i++) {
        state[1][i - 1] = state[1][i];
        // printf("%x %x %x %x\n", state[1][0], state[1][1],
        // state[1][2],state[1][3]);
    }
    state[1][3] = temp;
    // printf("%x %x %x %x\n", state[1][0], state[1][1],
    // state[1][2],state[1][3]);

    temp = state[2][0];
    int temp2 = state[2][1];
    for (int i = 2; i < 4; i++) {
        state[2][i - 2] = state[2][i];
    }
    state[2][2] = temp;
    state[2][3] = temp2;
    ;
    //左移三位等加于右移一位
    temp = state[3][3];
    for (int i = 2; i >= 0; i--) {
        state[3][i + 1] = state[3][i];
    }
    state[3][0] = temp;
}

void aes_encryption::MixColumns() {
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

uint8 aes_encryption::FieldMul(uint8 a, uint8 b) {
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
uint32 aes_encryption::bytes_to_word(uint8* bytes) {
    //将一个bytes数组的四个字节合成一个字
    uint32 number = 0;
    number = number | bytes[0];
    number = (number << 8) | bytes[1];
    number = (number << 8) | bytes[2];
    number = (number << 8) | bytes[3];
    return number;
}

void aes_encryption::KeyExpansion(uint32 initial_key[4]) {
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

uint32 aes_encryption::RotWord(uint32 w) {
    // (B1, B2, B3, B4) --> (B2, B3, B4, B1)
    uint32 right = (w & 0xff000000) >> 24;
    // right为字的第一个byte，循环移位到最右边
    uint32 left = w << 8;
    return left | right;
}
uint32 aes_encryption::SubWord(uint32 w) {
    uint8* byte_array = (uint8*)&w;
    for (int i = 0; i < 4; i++) {
        uint8 one_char = byte_array[i];
        byte_array[i] = SubByte(byte_array[i]);
    }
    return w;
}

uint8 aes_encryption::SubByte(uint8 c) {
    int row = (0xf0 & c) >> 4;
    int col = (0x0f & c);
    return this->s_box[row][col];
}
void aes_encryption::general_roundkey(uint8 initial_key_byte[16]) {
    uint32 initial_key_word[4];
    for (int i = 0; i < 4; i++) {
        initial_key_word[i] = bytes_to_word(&initial_key_byte[4 * i]);
    }
    KeyExpansion(initial_key_word);
}

void aes_encryption::encryption(string p_text, string key, string init_vector) {
    string total_plain_text = padding(p_text);
    vector<vector<uint8>> text_vector = split(total_plain_text);

    uint8 key_array[16];
    for(int i = 0;i < key.size();i++){
        key_array[i] = key[i];
    }
    general_roundkey(key_array);

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
        print_array(y);
    }
}

void aes_encryption::print_array(uint8 a[16]){
    for(int i = 0;i < 16;i++){
        printf("%x", a[i]);
    }
    printf("\n");
}

void aes_encryption::xor_group(uint8 a[16], uint8 b[16]){
    for(int i = 0;i < 16;i++){
        a[i] = a[i] ^ b[i];
    }
}
vector<vector<uint8>> aes_encryption::split(string s){
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
string aes_encryption::padding(string s) {
    char padding[16];
    int pad_size;
    if (s.size() % 16 == 0) {
        pad_size = 16;
    }
    else{
        pad_size = 16 - (s.size() - (s.size() / 16) * 16);
        padding[pad_size] = '\0';
    }

    for(int i = 0;i < pad_size;i++){
        padding[i] = pad_size;
    }

    return s + padding;
}

void aes_encryption::encrypt_one_group(uint8 p_text[16]) {
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
