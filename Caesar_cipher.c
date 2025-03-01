#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_LENGTH 100

void encrypt(char* text, int shift){
    for(int i = 0; i < strlen(text); i++){
        if(isalpha(text[i])){
            if(isupper(text[i])){
                text[i] = ((text[i] - 'A' + shift) % 26) + 'A';
            }
            else{
                text[i] = ((text[i] - 'a' + shift) % 26) + 'a';
            }
        }
    }
}

void decrypt(char* text, int shift){
    for(int i = 0; i < strlen(text); i++){
        if(isalpha(text[i])){
            if(isupper(text[i])){
                text[i] = ((text[i] - 'A' - shift + 26) % 26) + 'A';
            }
            else{
                text[i] = ((text[i] - 'a' - shift + 26) % 26) + 'a';
            }
        }
    }
}

int main()
{
    char message[MAX_LENGTH];
    int shift;

    printf("Enter your text:\n");
    fgets(message, MAX_LENGTH, stdin);
    printf("Enter the shift:\n");
    scanf("%d", &shift);

    encrypt(message, shift);
    printf("Encrypted text: %s\n", message);

    decrypt(message, shift);
    printf("Decrupted text: %s\n", message);

    return 0;
}