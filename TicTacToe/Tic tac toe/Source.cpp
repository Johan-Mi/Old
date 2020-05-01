#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <chrono>

byte iTurn = 1;
byte iBoard[3][3]{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
byte iWinner = 0;
bool bEndGame = false;
olc::Sprite* spr1;
olc::Sprite* spr2;

constexpr byte winChecks[8][3]{
	{0,1,2},
	{4,5,6},
	{8,9,10},
	{0,4,8},
	{1,5,9},
	{2,6,10},
	{0,5,10},
	{2,5,8}
};

void checkWin(byte iSide) {
	for(byte i = 0; i < 8; i++) {
		bool bWin = true;
		for(byte j = 0; j < 3; j++) {
			bWin &= iBoard[winChecks[i][j] >> 2][winChecks[i][j] & 3] == iSide;
		}
		if(bWin) {
			iWinner = iSide;
			return;
		}
	}

	bool bBoardFull = true;
	for(byte i = 0; i < 3; i++)
		for(byte j = 0; j < 3; j++)
			bBoardFull &= iBoard[i][j] != 0;
	if(bBoardFull) {
		iWinner = 4;
		return;
	}
}

class Example : public olc::PixelGameEngine {
public:
	Example() {
		sAppName = "Tic tac toe";
	}

public:
	bool OnUserCreate() override {
		spr1 = new olc::Sprite("..\\1.png");
		spr2 = new olc::Sprite("..\\2.png");
		Clear(olc::WHITE);
		DrawLine(0, 32, 97, 32, olc::BLACK);
		DrawLine(0, 65, 97, 65, olc::BLACK);
		DrawLine(32, 0, 32, 97, olc::BLACK);
		DrawLine(65, 0, 65, 97, olc::BLACK);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		if(bEndGame) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			return false;
		}
		if(iWinner != 0) {
			bEndGame = true;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			Clear(olc::WHITE);
			if(iWinner & 1) {
				DrawString(24, 38, "X won!", olc::BLACK);
				return true;
			}
			if(iWinner & 2) {
				DrawString(24, 38, "O won!", olc::BLACK);
				return true;
			}
			DrawString(31, 38, "Tie!", olc::BLACK);
			return true;
		}

		if(GetKey(olc::ESCAPE).bHeld)
			return false;
		if(GetMouse(0).bPressed) {
			byte iMouseX = GetMouseX() / 33;
			byte iMouseY = GetMouseY() / 33;
			if(iBoard[iMouseY][iMouseX] == 0) {
				iBoard[iMouseY][iMouseX] = iTurn;
				DrawSprite(iMouseX * 33, iMouseY * 33, iTurn & 1 ? spr1 : spr2);
				checkWin(iTurn);
				iTurn ^= 3;
			}
		}
		return true;
	}
};


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Example demo;
	if(demo.Construct(98, 98, 4, 4))
		demo.Start();
	delete spr1, spr2;
	return 0;
}