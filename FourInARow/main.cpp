#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

const int iBoardWidth = 7;
const int iBoardHeight = 6;
byte yBoard[iBoardHeight][iBoardWidth];
byte yTurn = 1;
byte yWinner = 0;
olc::Sprite* sprites[3];
bool bGameEnded = false;

class Example : public olc::PixelGameEngine {
public:
	Example() {
		sAppName = "Four in a row";
	}

public:
	bool OnUserCreate() override {
		sprites[0] = new olc::Sprite("..\\0.png");
		sprites[1] = new olc::Sprite("..\\1.png");
		sprites[2] = new olc::Sprite("..\\2.png");
		for(int i = 0; i < iBoardHeight; i++) {
			for(int j = 0; j < iBoardWidth; j++) {
				yBoard[i][j] = 0;
				DrawSprite(j * 16, i * 16, sprites[0]);
			}
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		if(bGameEnded) {
			std::this_thread::sleep_for(std::chrono::seconds(2));
			return false;
		} else if(yWinner != 0) {
			bGameEnded = true;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			Clear(olc::WHITE);
			if(yWinner == 1)
				DrawString(20, 40, "Red won!", olc::BLACK);
			else if(yWinner == 2)
				DrawString(20, 40, "Blue won!", olc::BLACK);
			else
				DrawString(25, 40, "Tie!", olc::BLACK);
		}
		else if(GetMouse(0).bPressed) {
			int iSelectedCol = floor(GetMouseX() / 16);
			if(yBoard[0][iSelectedCol] == 0) {
				int iTestRow = 0;
				while(iTestRow < iBoardHeight - 1 && yBoard[iTestRow + 1][iSelectedCol] == 0)
					iTestRow++;
				yBoard[iTestRow][iSelectedCol] = yTurn;
				DrawSprite(iSelectedCol * 16, iTestRow * 16, sprites[yTurn]);

				for(int i = 0; i < iBoardHeight; i++) {
					for(int j = 0; j < iBoardWidth - 3; j++) {
						bool bWin = true;
						for(int k = 0; k < 4; k++) {
							bWin &= yBoard[i][j + k] == yTurn;
						}
						if(bWin)
							yWinner = yTurn;
					}
				}

				for(int i = 0; i < iBoardHeight - 3; i++) {
					for(int j = 0; j < iBoardWidth; j++) {
						bool bWin = true;
						for(int k = 0; k < 4; k++) {
							bWin &= yBoard[i + k][j] == yTurn;
						}
						if(bWin)
							yWinner = yTurn;
					}
				}

				for(int i = 0; i < iBoardHeight - 3; i++) {
					for(int j = 0; j < iBoardWidth - 3; j++) {
						bool bWin = true;
						for(int k = 0; k < 4; k++) {
							bWin &= yBoard[i + k][j + k] == yTurn;
						}
						if(bWin)
							yWinner = yTurn;
					}
				}

				for(int i = 3; i < iBoardHeight; i++) {
					for(int j = 0; j < iBoardWidth - 3; j++) {
						bool bWin = true;
						for(int k = 0; k < 4; k++) {
							bWin &= yBoard[i - k][j + k] == yTurn;
						}
						if(bWin)
							yWinner = yTurn;
					}
				}

				bool bWin = true;
				for(int i = 0; i < iBoardHeight; i++) {
					for(int j = 0; j < iBoardWidth; j++)
						bWin &= yBoard[i][j] != 0;
				}
				if(bWin)
					yWinner = 3;

				yTurn = 3 - yTurn;
			}
		}
		return true;
	}
};

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
	Example demo;
	if(demo.Construct(112, 96, 4, 4))
		demo.Start();

	return 0;
}