#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string ALPHABET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.!?- ";
const int N = ALPHABET.size();

// НОД
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Обратный элемент по модулю
int mod_inverse(int a, int m) {
    int m0 = m;
    int x = 1, x1 = 0;
    int q, temp;

    while (m != 0) {
        q = a / m;

        temp = m;
        m = a % m;
        a = temp;

        temp = x1;
        x1 = x - q * x1;
        x = temp;
    }

    if (a != 1) return -1;

    return (x % m0 + m0) % m0;
}

// допустимые a
vector<int> findA() {
    vector<int> res;
    for (int i = 1; i < N; i++) {
        if (gcd(i, N) == 1) {
            res.push_back(i);
        }
    }
    return res;
}

// маппинг
int charToIndex(char c) {
    size_t pos = ALPHABET.find(c);
    if (pos == string::npos) return -1;
    return pos;
}

char indexToChar(int i) {
    return ALPHABET[i];
}

// шифрование
string encrypt(const string& text, int a, int b) {
    string cipher = "";

    for (size_t i = 0; i < text.size(); i++) {
        int idx = charToIndex(text[i]);

        if (idx != -1) {
            int var = (a * idx + b) % N;
            cipher += indexToChar(var);
        } else {
            cipher += text[i];
        }
    }

    return cipher;
}

// дешифрование
string decrypt(const string& cipher, int a, int b) {
    int invA = mod_inverse(a, N);

    if (invA == -1) {
        cout << "Invalid key a (no inverse exists)\n";
        return "";
    }

    string text = "";

    for (size_t i = 0; i < cipher.size(); i++) {
        int idx = charToIndex(cipher[i]);

        if (idx != -1) {
            int var = (invA * (idx - b)) % N;
            if (var < 0) var += N;
            text += indexToChar(var);
        } else {
            text += cipher[i];
        }
    }

    return text;
}

int main() {
    int a, b;
    vector<int> A = findA();

    cout << "Alphabet size: " << N << endl;

    // выбор a
    cout << "Choose the key a from the list:\n";
    for (size_t i = 0; i < A.size(); i++) {
        cout << A[i] << " ";
    }
    cout << endl;

    cin >> a;

    // проверка a
    bool valid = false;
    for (int x : A) {
        if (x == a) valid = true;
    }

    if (!valid) {
        cout << "Invalid a!\n";
        return 0;
    }

    cout << "Choose the key b (0 to " << N - 1 << "): ";
    cin >> b;

    cin.ignore(); // важно для getline

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Encrypt\n";
        cout << "2. Decrypt\n";
        cout << "3. Change key\n";
        cout << "4. Exit\n";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string text;
            cout << "Enter text: ";
            getline(cin, text);

            cout << "Encrypted: " << encrypt(text, a, b) << endl;
        }
        else if (choice == 2) {
            string text;
            cout << "Enter cipher: ";
            getline(cin, text);

            cout << "Decrypted: " << decrypt(text, a, b) << endl;
        }
        else if (choice == 3) {
            cout << "Choose new a:\n";
            for (int x : A) cout << x << " ";
            cout << endl;

            cin >> a;

            valid = false;
            for (int x : A) {
                if (x == a) valid = true;
            }

            if (!valid) {
                cout << "Invalid a!\n";
                continue;
            }

            cout << "Choose new b (0 to " << N - 1 << "): ";
            cin >> b;

            cin.ignore();
        }
        else if (choice == 4) {
            break;
        }
        else {
            cout << "Invalid option\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}