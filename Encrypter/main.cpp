#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main() {
	std::string sKey, sModeInput;
	bool bMode;

	std::cout << "Select a mode\nEncrypt: 1\nDecrypt: 2\n";
	std::cin >> sModeInput;
	bMode = sModeInput != "2";

	std::cout << (bMode ? "Encryption key: " : "Decryption key: ");
	std::cin >> sKey;

	std::ifstream infile(bMode ? "decrypted.txt" : "encrypted.txt");
	std::vector<char> buffer;

	infile.seekg(0, infile.end);
	size_t length = infile.tellg();
	infile.seekg(0, infile.beg);

	if(length > 0) {
		buffer.resize(length);
		infile.read(&buffer[0], length);
	}

	for(int i = 0; i < buffer.size(); i++) {
		if(bMode)
			buffer[i] += sKey[i % sKey.length()];
		else
			buffer[i] -= sKey[i % sKey.length()];
	}

	std::ofstream outfile(bMode ? "encrypted.txt" : "decrypted.txt", std::ios::trunc);
	outfile.write(&buffer[0], buffer.size());
}