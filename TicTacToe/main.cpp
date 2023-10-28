#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <chrono>
#include <memory>

uint8_t iTurn = 1;
uint8_t iBoard[3][3]{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
uint8_t iWinner = 0;
bool bEndGame = false;
std::unique_ptr<olc::Sprite> spr1;
std::unique_ptr<olc::Sprite> spr2;

constexpr uint8_t winChecks[8][3]{
	{0,1,2},
	{4,5,6},
	{8,9,10},
	{0,4,8},
	{1,5,9},
	{2,6,10},
	{0,5,10},
	{2,5,8}
};

void checkWin(uint8_t iSide) {
	for(uint8_t i = 0; i < 8; i++) {
		for(uint8_t j = 0; j < 3; j++)
			if(iBoard[winChecks[i][j] >> 2][winChecks[i][j] & 3] != iSide)
				goto no_win;
		iWinner = iSide;
		return;
no_win:;
	}

	for(uint8_t i = 0; i < 3; i++)
		for(uint8_t j = 0; j < 3; j++)
			if(!iBoard[i][j])
				return;
	iWinner = 4;
}

class Example : public olc::PixelGameEngine {
public:
	Example() {
		sAppName = "Tic tac toe";
	}

public:
	bool OnUserCreate() override {
		spr1 = std::make_unique<olc::Sprite>("1.png");
		spr2 = std::make_unique<olc::Sprite>("2.png");
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
			uint8_t iMouseX = GetMouseX() / 33;
			uint8_t iMouseY = GetMouseY() / 33;
			if(iBoard[iMouseY][iMouseX] == 0) {
				iBoard[iMouseY][iMouseX] = iTurn;
				DrawSprite(iMouseX * 33, iMouseY * 33,
						iTurn & 1 ? spr1.get() : spr2.get());
				checkWin(iTurn);
				iTurn ^= 3;
			}
		}
		return true;
	}
};


int main() {
	Example demo;
	if(demo.Construct(98, 98, 4, 4))
		demo.Start();
	return 0;
}
