#ifndef ABSTRACT
#define ABSTRACT
#include <stdio.h>
#include <string>
#include <vector>

typedef unsigned int uint32;
typedef unsigned char uint8;

#define NROUND 10
using namespace std;

class aes_abstract{
public:
    static uint8 s_box[16][16];    
    static uint32 r_cont[10];
    static uint8 mix_column_matrix[4][4];
    uint8 state[4][4];
    uint32 round_key[44];
    int current_round = 0;

    void AddRoundKey();
    void SubBytes();

    void MixColumns();
    uint8 FieldMul(uint8, uint8);

    uint32 bytes_to_word(uint8*);

    void KeyExpansion(uint32[4]);
    uint32 RotWord(uint32);
    uint32 SubWord(uint32);
    uint8 SubByte(uint8);

    void general_roundkey(uint8 [16]);

    void print_array(uint8 a[16]);
    void xor_group(uint8 a[16], uint8 b[16]);


    void print_round_key();
    void set_state(uint8 new_state[4][4]);
    void print_state();
};
#endif