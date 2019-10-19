#include "encryptor.hpp"
#include "decryptor.hpp"
#include "const_value.hpp"

int main(){
    aes_encryptor encrypter;
    aes_decryptor decryptor;
    
	vector<vector<uint8>> cipher;

    cipher = encrypter.encryption(plain_text, key, IV);

    printf("密文：\n");
    for(auto group : cipher){
        for(int i = 0;i < group.size();i++){
            printf("%x", group[i]);
        }
        printf("\n");
    }

    string text = decryptor.decryption(cipher, key, IV);
    printf("明文：\n");
    printf("%s\n", text.c_str());

    return 0;
}