#include <stdio.h>
#include <string>
#include <vector>

#ifndef MAIN_HPP
#define MAIN_HPP

#define NROUND 10

using namespace std;
typedef unsigned int uint32;
typedef unsigned char uint8;

class aes_encryption{
public:
    static uint8 s_box[16][16];    
    static uint32 r_cont[10];
    static uint8 mix_column_matrix[4][4];
    uint8 state[4][4];
    uint32 round_key[44];
    int current_round = 0;

    void AddRoundKey();
    void SubBytes();
    void ShiftRows();

    void MixColumns();
    uint8 FieldMul(uint8, uint8);

    uint32 bytes_to_word(uint8*);

    void KeyExpansion(uint32[4]);
    uint32 RotWord(uint32);
    uint32 SubWord(uint32);
    uint8 SubByte(uint8);

    void general_roundkey(uint8 [16]);

    void encryption(string p_text, string key, string init_vector);
    string padding(string s);
    vector<vector<uint8>> split(string s);
    void xor_group(uint8 a[16], uint8 b[16]);
    void print_array(uint8 a[16]);

    void encrypt_one_group(uint8 p_text[16]);
    void print_round_key(){
        for(int i = 0;i < 44;i++){
            printf("The RoundKey[%d]: 0x%x\n", i, round_key[i]);
        }
    }
    void set_state(uint8 new_state[4][4]){
        for(int i = 0;i < 4;i++){
            for(int j = 0;j < 4;j++){
                state[i][j] = new_state[i][j];
            }
        }
    }
    void print_state(){
        printf("The current state:\n");
        for(int i = 0;i < 4;i++){
            for(int j = 0;j < 4;j++){
                printf("%x ", state[i][j]);
            }
            printf("\n");
        }
    }
};


#endif