# CS463-Course-Project

The following programming project uses C++17 for double encryption/decryption and key generation. It uses symmetric (Affine) and asymmetric (RSA) encryption. It also includes RSA key generation.

The outline of the program is as follows:
1. Let the user input the plaintext.
2. Let the user input the a and b for the Affine cipher key.
3. Let the user input the p, q, and e for the RSA key generation.
4. Encrypt the plaintext with the Affine cipher key.
5. Encrypt the Affine ciphertext with the RSA public key.
6. Print the RSA ciphertext.
7. Decrypt the RSA ciphertext with the RSA private key.
8. Decrypt the Affine ciphertext with the Affine cipher key.
9. Print the plaintext (it should match the original input).

Please use the following commands to run this program:
1. g++ -std=c++17 -o project project.cpp
2. ./project
