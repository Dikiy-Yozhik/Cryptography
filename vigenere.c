#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 100

void encrypt(char *text, const char *key) 
{
    int key_len = strlen(key);
    int key_index = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            int shift = tolower(key[key_index % key_len]) - 'a';

            if (islower(text[i])) {
                text[i] = 'a' + (text[i] - 'a' + shift) % 26;
            } else {
                text[i] = 'A' + (text[i] - 'A' + shift) % 26;
            }

            key_index++;
        }
    }
}

void decrypt(char *text, const char *key) 
{
    int key_len = strlen(key);
    int key_index = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            int shift = tolower(key[key_index % key_len]) - 'a';

            if (islower(text[i])) {
                text[i] = 'a' + (text[i] - 'a' - shift + 26) % 26;
            } else {
                text[i] = 'A' + (text[i] - 'A' - shift + 26) % 26;
            }

            key_index++;
        }
    }
}

int main() 
{
    char text[MAX_LENGTH];
    char key[256];

    printf("Enter your text: ");
    fgets(text, sizeof(text), stdin);
    printf("Enter the key: ");
    fgets(key, sizeof(key), stdin);

    key[strcspn(key, "\n")] = '\0';

    encrypt(text, key);
    printf("Encrypted text: %s\n", text);

    decrypt(text, key);
    printf("Decrypted text: %s\n", text);

    return 0;
}