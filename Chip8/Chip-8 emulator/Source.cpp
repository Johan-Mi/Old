#define OLC_PGE_APPLICATION
#define int8 uint8_t
#define int16 uint16_t
#include "olcPixelGameEngine.h"
#include <array>
#include <fstream>
#include <iostream>
//#define log

const int
width = 64,
height = 32;
const std::array<olc::Key, 16> keyCodes{olc::NP0,olc::NP1,olc::NP2,olc::NP3,olc::NP4,olc::NP5,olc::NP6,olc::NP7,olc::NP8,olc::NP9,olc::A,olc::B,olc::C,olc::D,olc::E,olc::F};
std::array<int8, 0x1000> memory;
std::array<int16, 0x18> stack;
std::array<int8, 0x10> vRegs;
std::array<std::array<bool, width>, height> screen;
int16
I = 0x0,
PC = 0x200;
int8
SP = 0x0,
DT = 0x0,
ST = 0x0;

const std::array<int8, 96> font{
	0xF0,0x90,0x90,0x90,0xF0,0x00,
	0x20,0x60,0x20,0x20,0x70,0x00,
	0xF0,0x10,0xF0,0x80,0xF0,0x00,
	0xF0,0x10,0xF0,0x10,0xF0,0x00,
	0x90,0x90,0xF0,0x10,0x10,0x00,
	0xF0,0x80,0xF0,0x10,0xF0,0x00,
	0xF0,0x80,0xF0,0x90,0xF0,0x00,
	0xF0,0x10,0x20,0x40,0x40,0x00,
	0xF0,0x90,0xF0,0x90,0xF0,0x00,
	0xF0,0x90,0xF0,0x10,0xF0,0x00,
	0xF0,0x90,0xF0,0x90,0x90,0x00,
	0xE0,0x90,0xE0,0x90,0xE0,0x00,
	0xF0,0x80,0x80,0x80,0xF0,0x00,
	0xE0,0x90,0x90,0x90,0xE0,0x00,
	0xF0,0x80,0xF0,0x80,0xF0,0x00,
	0xF0,0x80,0xF0,0x80,0x80,0x00,
};

class Example : public olc::PixelGameEngine {
public:
	Example() {
		sAppName = "Chip-8 emulator";
	}

public:
	bool tick() {
		union {
			int16 value;
			struct { int8 byte2, byte1; };
			struct { unsigned int nybble4 : 4, nybble3 : 4, nybble2 : 4, nybble1 : 4; };
		} n;
		n.value = memory[PC] << 8 | memory[PC + 1];
		int8&
			Vx = vRegs[n.nybble2],
			Vy = vRegs[n.nybble3];
		bool pressed = false;



		switch(n.nybble1) {
		case 0x0:
			switch(n.byte2) {
			case 0xE0:	Clear(olc::BLACK);	for(int i = 0; i < height; i++)	screen[i].fill(false);				PC += 2;	break;
			case 0xEE:	PC = stack[SP];	SP--;																				break;
			case 0x00:	return false;																						break;
			}																												break;
		case 0x1:	PC = n.value & 0xFFF;																					break;
		case 0x2:	SP++;	stack[SP] = PC;	PC = n.value & 0xFFF;															break;
		case 0x3:	PC += Vx == n.byte2 ? 4 : 2;																			break;
		case 0x4:	PC += Vx == n.byte2 ? 2 : 4;																			break;
		case 0x5:	PC += Vx == Vy ? 4 : 2;																					break;
		case 0x6:	Vx = n.byte2;																				PC += 2;	break;
		case 0x7:	Vx += n.byte2;																				PC += 2;	break;
		case 0x8:
			switch(n.nybble4) {
			case 0x0:	Vx = Vy;																							break;
			case 0x1:	Vx |= Vy;																							break;
			case 0x2:	Vx &= Vy;																							break;
			case 0x3:	Vx ^= Vy;																							break;
			case 0x4:	vRegs[0xF] = ((int16)Vx + (int16)Vy) >> 8;	Vx += Vy;												break;
			case 0x5:	vRegs[0xF] = Vx > Vy;						Vx -= Vy;												break;
			case 0x6:	vRegs[0xF] = Vx & 0x1;						Vx = Vx >> 1;											break;
			case 0x7:	vRegs[0xF] = Vx < Vy;						Vx = Vy - Vx;											break;
			case 0xE:	vRegs[0xF] = Vx >> 7;						Vx = Vx << 1;											break;
			}																									PC += 2;	break;
		case 0x9:	PC += Vx == Vy ? 2 : 4;																					break;
		case 0xA:	I = n.value & 0x0FFF;																					break;
		case 0xB:	PC = n.value & 0x0FFF + vRegs[0x0];																		break;
		case 0xC:	Vx = (rand() % 255) & n.byte2;																			break;
		case 0xD:
			vRegs[0xF] = 0x0;
			for(int8 i = 0; i < n.nybble4; i++) {
				for(int8 j = 0; j < 8; j++) {
					if((memory[I + i] & (1 << j)) >> j) {
						int drawX = (Vx + 8 - j) % width;
						int drawY = (Vy + i) % height;
						screen[drawY][drawX] ^= true;
						if(screen[drawY][drawX]) {
							Draw(drawX, drawY, olc::WHITE);
						} else {
							vRegs[0xF] = 0x1;
							Draw(drawX, drawY, olc::BLACK);
						}
					}
				}
			}																									PC += 2;	break;
		case 0xE:
			switch(n.byte2) {
			case 0x9E:	PC += GetKey(keyCodes[Vx]).bHeld ? 4 : 2;															break;
			case 0xA1:	PC += GetKey(keyCodes[Vx]).bHeld ? 2 : 4;															break;
			}																												break;
		case 0xF:
			switch(n.byte2) {
			case 0x07:	Vx = DT;																							break;
			case 0x0A:
				while(!pressed) {
					for(int8 keyNum = 0; keyNum < 16 && !pressed; keyNum++) {
						if(GetKey(keyCodes[keyNum]).bHeld) {
							Vx = keyNum;
							pressed = true;
						}
					}
				}																											break;
			case 0x15:	DT = Vx;																							break;
			case 0x18:	ST = Vx;																							break;
			case 0x1E:	I += Vx;																							break;
			case 0x29:	I = Vx * 6;																							break;
			case 0x33:	memory[I] = Vx - (Vx % 100);	memory[I + 1] = (Vx - (Vx % 10)) % 100;	memory[I + 2] = Vx % 10;	break;
			case 0x55:	for(unsigned int i = 0; i <= n.nybble2; i++)	memory[I + i] = vRegs[i];							break;
			case 0x65:	for(unsigned int i = 0; i <= n.nybble2; i++)	vRegs[i] = memory[I + i];							break;
			}																									PC += 2;	break;
		}
		return true;
	}

	bool OnUserCreate() override {
		std::ifstream fileStream("..\\program.txt", std::ios::in);
		std::vector<std::pair<int16, int16>> program;
		fileStream >> std::hex >> PC;
		while(!fileStream.eof()) {
			std::pair<int16, int16> f;
			fileStream >> std::hex >> f.first >> f.second;
			memory[f.first] = f.second >> 8;
			memory[f.first + 1] = f.second & 0xFF;
		}

		std::cout << std::hex;
		srand((unsigned int)time(NULL));
		for(int i = 0; i < 96; i++)
			memory[i] = font[i];
		std::cout << '\n';
		Clear(olc::BLACK);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		if(GetKey(olc::ESCAPE).bHeld) return false;
		return tick();
	}
};

int main() {
	std::thread timerThread{[]() {
	if(DT > 0) DT--;
	if(ST > 0) ST--;
	std::this_thread::sleep_for(std::chrono::seconds(1 / 60));
	}};

#ifdef log
	std::thread loggingThread{[&]() {
		while(true) {
			system("CLS");
			std::cout << "PC: " << (int)PC << "\nSP: " << (int)SP << "\nI : " << (int)I << "\nDT: " << (int)DT << "\nST: " << (int)ST << '\n';
			for(int i = 0; i < 16; i++) std::cout << "\nV" << i << ": " << (int)vRegs[i];
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}};
#endif

	Example demo;
	if(demo.Construct(width, height, 16, 16)) demo.Start();
	timerThread.detach();

#ifdef log
	loggingThread.detach();
#endif
	return 0;
		}