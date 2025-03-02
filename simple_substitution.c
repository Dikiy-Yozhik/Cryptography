#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_LENGTH 100

char Alphabet[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char alphabet[27] = "abcdefghijklmnopqrstuvwxyz";

void cipher(char* key){
    char out[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char res[26];
    int k = 0;
    for(int i = 0; i < strlen(key); i++){
        if(isalpha(key[i]) && strchr(out, toupper(key[i]))!=NULL){
            res[k++] = toupper(key[i]);
            *strchr(out, toupper(key[i])) = '0';
        }
    }
    for(int i = 0; i < strlen(out); i++){
        if(out[i] != '0') res[k++] = out[i];
    }

    for(int i = 0; i < 26; i++) key[26 - i] = res[i];
    key[26] = '\0'; 
}

void encrypt(char* text, char* key){
    for(int i = 0; i < strlen(text); i++){
        if(isalpha(text[i])){
            if(isupper(text[i])){
                int k = strchr(Alphabet, text[i]) - Alphabet;
                text[i] = key[k];
            }
            else{
                int k = strchr(alphabet, text[i]) - alphabet;
                text[i] = tolower(key[k]);
            }
        }
    }
}

void decrypt(char* text, char* key){
    for(int i = 0; i < strlen(text); i++){
        if(isalpha(text[i])){
            if(isupper(text[i])){
                char* b = strchr(key, text[i]);
                int k = b - key;
                text[i] = Alphabet[k];
            }
            else{
                char* b = strchr(key, toupper(text[i]));
                int k = b - key;
                text[i] = alphabet[k];
            }
        }
    }
}

int main(){
    char message[MAX_LENGTH];
    char key[27];

    printf("Enter your text:\n");
    fgets(message, MAX_LENGTH, stdin);
    message[strcspn(message, "\n")] = 0; 

    printf("Enter the key:\n");
    fgets(key, 27, stdin);
    key[strcspn(key, "\n")] = 0; 

    cipher(key);
    puts(key);

    encrypt(message, key);
    printf("Encrypted text: %s\n", message);

    decrypt(message, key);
    printf("Decrypted text: %s\n", message);

    return 0;
}
