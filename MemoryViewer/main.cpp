#include <iostream>
#include <iomanip>

const char* exampleString = "Hello, world!";
constexpr int charsPerLine = 16;
const char* ascii = "................................ !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~.ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜø£Ø×ƒáíóúñÑªº¿®¬½¼¡«»░▒▓│┤ÁÂÀ©╣║╗╝¢¥┐└┴┬├─┼ãÃ╚╔╩╦╠═╬¤ðÐÊËÈıÍÎÏ┘┌█▄¦Ì▀ÓßÔÒõÕµþÞÚÛÙýÝ¯´­±‗¾¶§÷¸°¨·¹³²■.";

int main() {
	int startAdress = (int)exampleString;
	int endAdress = (int)exampleString + 64;
	//std::cout << "Starting adress: ";
	//std::cin >> startAdress;
	//std::cout << "Ending adress:   ";
	//std::cin >> endAdress;

	for(int offset = 0; offset < endAdress - startAdress; offset += charsPerLine) {
		char* currentAdress = (char*)startAdress + offset;
		std::cout << "\n0x" << std::hex << (int)currentAdress << "    ";
		for(int i = 0; i < charsPerLine; i++) {
			std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)* currentAdress << ' ';
			currentAdress++;
		}
		std::cout << "   ";
		for(int i = 0; i < charsPerLine; i++) {
			std::cout << ascii[*currentAdress];
			currentAdress++;
		}
	}
}