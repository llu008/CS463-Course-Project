#include <iostream>
#include <numeric>
#include <cctype>
#include <cmath>

using namespace std;

// Function to get the user input (plaintext and affine cipher key)
void getUserInput(string& plaintext, int& a, int& b) {
    cout << "Enter the plaintext: ";
    getline(cin, plaintext);
    cout << "Enter the value of a: ";
    cin >> a;
    while (a < 1 || a > 25 || gcd(a, 26) != 1) {
        cout << "Invalid input (it must be between 1 and 25 and coprime with 26)." << endl;
        cout << "Enter the value of a: ";
        cin >> a;
    }
    cout << "Enter the value of b: ";
    cin >> b;
    while (b < 0 || b > 25) {
        cout << "Invalid input (it must be between 0 and 25)." << endl;
        cout << "Enter the value of b: ";
        cin >> b;
    }
}

// Function to check if a number is prime
bool isPrime(int number) {
    if (number <= 1) {
        return false;
    }
    for (int i = 2; i <= sqrt(number); i++) {
        if (number % i == 0) {
            return false;
        }
    }
    return true;
}

// Function to calculate the modular inverse
int modularInverse(int phi, int e) {
    int r0 = phi;
    int r1 = e;
    int s0 = 1;
    int s1 = 0;
    int t0 = 0;
    int t1 = 1;
    while (r1 != 0) {
        int r2 = r0 % r1;
        int q2 = (r0 - r2) / r1;
        int s2 = s0 - (q2 * s1);
        int t2 = t0 - (q2 * t1);
        r0 = r1;
        r1 = r2;
        s0 = s1;
        s1 = s2;
        t0 = t1;
        t1 = t2;
    }
    if (t0 < 0) {
        t0 += phi;
    }
    return t0;
}

// Function to generate the RSA public and private keys
void rsaKeyGeneration(int& p, int& q, int& n, int& e, int& d) {
    cout << "Enter the value of p: ";
    cin >> p;
    while (!isPrime(p)) {
        cout << "Invalid input (it must be prime)." << endl;
        cout << "Enter the value of p: ";
        cin >> p;
    }
    cout << "Enter the value of q: ";
    cin >> q;
    while (!isPrime(q) || p == q) {
        cout << "Invalid input (it must be prime and not equal to p)." << endl;
        cout << "Enter the value of q: ";
        cin >> q;
    }
    n = p * q;
    if (n < 128) {
        cout << "Error. The product of p and q must be at least 128 to handle ASCII encryption." << endl;
        cout << "Please choose larger primes for p and q." << endl;
        rsaKeyGeneration(p, q, n, e, d);
        return;
    }
    int phi = (p - 1) * (q - 1);
    cout << "Enter the value of e: ";
    cin >> e;
    while (e < 1 || e > phi - 1 || gcd(e, phi) != 1) {
        cout << "Invalid input (it must be between 1 and " << phi - 1 << " and coprime with " << phi << ")." << endl;
        cout << "Enter the value of e: ";
        cin >> e;
    }
    d = modularInverse(phi, e);
}

// Function to encrypt the plaintext
string affineEncryption(string plaintext, int a, int b) {
    string affineEncrypted = "";
    for (char character : plaintext) {
        if (isalpha(character)) {
            char base = isupper(character) ? 'A' : 'a';
            int x = character - base;
            int encryption = (a * x + b) % 26; // Affine cipher key = (a, b)
            affineEncrypted += char(encryption + base);
        } else {
            affineEncrypted += character;
        }
    }
    return affineEncrypted;
}

/**
 * Function to calculate the modular exponentiation
 * For example: 5^13 mod 23
 * Initialization: result = 1, x = 5 % 23 = 5, e = 13
 * Iteration 1: result = 5 % 23 = 5, x = 25 % 23 = 2, e = 13 / 2 = 6
 * Iteration 2: x = 4 % 23 = 4, e = 6 / 2 = 3
 * Iteration 3: result = 20 % 23 = 20, x = 16 % 23 = 16, e = 3 / 2 = 1
 * Iteration 4: result = 320 % 23 = 21, x = 256 % 23 = 3, e = 1 / 2 = 0
 */
int modularExponentiation(int base, int n, int exponent) {
    int result = 1;
    base = base % n; // In case the base is large
    while (exponent > 0) {
        if (exponent % 2 == 1) { // If the exponent is odd
            result = (result * base) % n;
        }
        base = (base * base) % n; // Square the base
        exponent /= 2; // Divide the exponent by 2 (integer division)
    }
    return result;
}

// Function to encrypt the Affine ciphertext
vector<int> rsaEncryption(string affineEncrypted, int n, int e) {
    vector<int> rsaEncrypted;
    for (char character : affineEncrypted) {
        int x = int(character); // Get the ASCII value
        int encryption = modularExponentiation(x, n, e); // RSA public key = (n, e)
        rsaEncrypted.push_back(encryption);
    }
    return rsaEncrypted;
}

// Function to decrypt the RSA ciphertext
string rsaDecryption(vector<int> rsaEncrypted, int n, int d) {
    string rsaDecrypted = "";
    for (int y : rsaEncrypted) {
        int decryption = modularExponentiation(y, n, d); // RSA private key = (n, d)
        rsaDecrypted += char(decryption);
    }
    return rsaDecrypted;
}

// Function to decrypt the Affine ciphertext
string affineDecryption(string rsaDecrypted, int a, int b) {
    string affineDecrypted = "";
    for (char character : rsaDecrypted) {
        if (isalpha(character)) {
            char base = isupper(character) ? 'A' : 'a';
            int y = character - base;
            int decryption = (modularInverse(26, a) * (y - b + 26)) % 26; // Affine cipher key = (a, b)
            affineDecrypted += char(decryption + base);
        } else {
            affineDecrypted += character;
        }
    }
    return affineDecrypted;
}

int main() {
    string plaintext = "";
    int a = 0;
    int b = 0;
    int p = 0;
    int q = 0;
    int e = 0;
    int n = 0;
    int d = 0;

    getUserInput(plaintext, a, b);
    rsaKeyGeneration(p, q, n, e, d);

    cout << endl;
    string affineEncrypted = affineEncryption(plaintext, a, b);
    vector<int> rsaEncrypted = rsaEncryption(affineEncrypted, n, e);
    cout << "RSA encrypted: ";
    for (int number : rsaEncrypted) {
        cout << number << " ";
    }
    cout << endl;
    string rsaDecrypted = rsaDecryption(rsaEncrypted, n, d);
    string affineDecrypted = affineDecryption(rsaDecrypted, a, b);
    cout << "Affine decrypted: " << affineDecrypted << endl;

    return 0;
}

/*
    cout << endl;
    cout << "Affine cipher key = (a, b) = (" << a << ", " << b << ")" << endl;
    cout << "RSA public key = (n, e) = (" << n << ", " << e << ")" << endl;
    cout << "RSA private key = (n, d) = (" << n << ", " << d << ")" << endl;
    cout << endl;

    string affineEncrypted = affineEncryption(plaintext, a, b);
    cout << "Affine encrypted: " << affineEncrypted << endl;
    vector<int> rsaEncrypted = rsaEncryption(affineEncrypted, n, e);
    cout << "RSA encrypted: ";
    for (int number : rsaEncrypted) {
        cout << number << " ";
    }
    cout << endl;
    string rsaDecrypted = rsaDecryption(rsaEncrypted, n, d);
    cout << "RSA decrypted: " << rsaDecrypted << endl;
    string affineDecrypted = affineDecryption(rsaDecrypted, a, b);
    cout << "Affine decrypted: " << affineDecrypted << endl;
*/
