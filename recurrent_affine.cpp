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

// Обратный элемент
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

// шифрование (рекуррентное)
string encrypt(const string& text, int a1, int a2, int b1, int b2) {
    string cipher = "";

    int a_prev2 = a1, a_prev1 = a2;
    int b_prev2 = b1, b_prev1 = b2;

    for (size_t i = 0; i < text.size(); i++) {
        int idx = charToIndex(text[i]);

        if (idx == -1) {
            cipher += text[i];
            continue;
        }

        int a_curr, b_curr;

        if (i == 0) {
            a_curr = a_prev2;
            b_curr = b_prev2;
        } else if (i == 1) {
            a_curr = a_prev1;
            b_curr = b_prev1;
        } else {
            a_curr = (a_prev1 * a_prev2) % N;
            b_curr = (b_prev1 + b_prev2) % N;

            a_prev2 = a_prev1;
            a_prev1 = a_curr;
            b_prev2 = b_prev1;
            b_prev1 = b_curr;
        }

        int var = (a_curr * idx + b_curr) % N;
        cipher += indexToChar(var);
    }

    return cipher;
}

// дешифрование (рекуррентное)
string decrypt(const string& cipher, int a1, int a2, int b1, int b2) {
    string text = "";

    int a_prev2 = a1, a_prev1 = a2;
    int b_prev2 = b1, b_prev1 = b2;

    for (size_t i = 0; i < cipher.size(); i++) {
        int idx = charToIndex(cipher[i]);

        if (idx == -1) {
            text += cipher[i];
            continue;
        }

        int a_curr, b_curr;

        if (i == 0) {
            a_curr = a_prev2;
            b_curr = b_prev2;
        } else if (i == 1) {
            a_curr = a_prev1;
            b_curr = b_prev1;
        } else {
            a_curr = (a_prev1 * a_prev2) % N;
            b_curr = (b_prev1 + b_prev2) % N;

            a_prev2 = a_prev1;
            a_prev1 = a_curr;
            b_prev2 = b_prev1;
            b_prev1 = b_curr;
        }

        int invA = mod_inverse(a_curr, N);
        if (invA == -1) {
            cout << "Error: inverse does not exist\n";
            return "";
        }

        int var = (invA * (idx - b_curr)) % N;
        if (var < 0) var += N;

        text += indexToChar(var);
    }

    return text;
}

// ввод ключей
void inputKeys(int& a1, int& a2, int& b1, int& b2, const vector<int>& A) {
    cout << "Choose a1 and a2:\n";
    for (int x : A) cout << x << " ";
    cout << endl;

    cin >> a1 >> a2;

    bool valid1 = false, valid2 = false;
    for (int x : A) {
        if (x == a1) valid1 = true;
        if (x == a2) valid2 = true;
    }

    if (!(valid1 && valid2)) {
        cout << "Invalid a values!\n";
        exit(0);
    }

    cout << "Choose b1 and b2 (0 to " << N - 1 << "): ";
    cin >> b1 >> b2;

    cin.ignore();
}

int main() {
    vector<int> A = findA();
    int a1, a2, b1, b2;

    cout << "Alphabet size: " << N << endl;

    inputKeys(a1, a2, b1, b2, A);

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

            cout << "Encrypted: " << encrypt(text, a1, a2, b1, b2) << endl;
        }
        else if (choice == 2) {
            string text;
            cout << "Enter cipher: ";
            getline(cin, text);

            cout << "Decrypted: " << decrypt(text, a1, a2, b1, b2) << endl;
        }
        else if (choice == 3) {
            inputKeys(a1, a2, b1, b2, A);
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