#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum struct Mode { Encrypting, Decrypting };

int main() {
    std::string sKey, sModeInput;
    Mode mode;

    std::cout << "Select a mode\nEncrypt: 1\nDecrypt: 2\n";
    std::cin >> sModeInput;
    if (sModeInput == "1") {
        mode = Mode::Encrypting;
        std::cout << "Encryption key: ";
    } else {
        mode = Mode::Decrypting;
        std::cout << "Decryption key: ";
    }

    std::cin >> sKey;

    std::ifstream infile(
        mode == Mode::Decrypting ? "decrypted.txt" : "encrypted.txt"
    );
    std::vector<char> buffer;

    infile.seekg(0, infile.end);
    size_t length = infile.tellg();
    infile.seekg(0, infile.beg);

    if (length > 0) {
        buffer.resize(length);
        infile.read(&buffer[0], length);
    }

    for (size_t i = 0; i < buffer.size(); i++) {
        if (mode == Mode::Encrypting) {
            buffer[i] += sKey[i % sKey.length()];
        } else {
            buffer[i] -= sKey[i % sKey.length()];
        }
    }

    std::ofstream outfile(
        mode == Mode::Encrypting ? "encrypted.txt" : "decrypted.txt",
        std::ios::trunc
    );
    outfile.write(&buffer[0], buffer.size());
}
