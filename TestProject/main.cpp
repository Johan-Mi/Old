#include <iostream>

void caesar(const char* input, int shift) {
	char* output{'\0'};
	for(int i = 0; i < strlen(input); i++) {
		char character = *(input + i);
		if(character > 64 && character < 91) {
			for(int j = 0; j < shift; j++) {
				character++;
				if(character >= 91)
					character = 0;
				while(character >= 64)
					character++;
			}
		}
		if(character > 96 && character < 123) {
			for(int j = 0; j < shift; j++) {
				character++;
				if(character >= 123)
					character = 0;
				while(character <= 96)
					character++;
			}
		}
		char* newString = new char[strlen(output) + 2];
		newString[strlen(output) + 1] = 0;
		newString[strlen(output)] = character;
		memcpy(newString, output, strlen(output));
		output = newString;
	}
	printf(output);
}

int main() {
	caesar("Hello World", 18);
	return 0;
}