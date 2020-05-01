#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <ctime>
#include <chrono>

const int iBoardWidth = 43;
const int iBoardHeight = 24;
int iMineCount = 90;
int iTilesLeft = iBoardWidth * iBoardHeight - iMineCount;
int iBoard[iBoardHeight][iBoardWidth];
int iBoardVisible[iBoardHeight][iBoardWidth];
bool bHasLost = false;
bool bHasWon = false;
bool bEndProgram = false;
olc::Sprite* sprTiles[13];

class Example : public olc::PixelGameEngine {
	void drawTile(int x, int y) {
		DrawSprite(x * 16, y * 16, sprTiles[iBoardVisible[y][x] == 0 ? 11 : (iBoardVisible[y][x] == 1 ? 10 : (iBoard[y][x] == -1 ? (bHasLost ? 12 : 9) : iBoard[y][x]))]);
	}
	void digTile(int x, int y) {
		if(iBoardVisible[y][x] == 0) {
			iBoardVisible[y][x] = 2;
			iTilesLeft--;
			if(iBoard[y][x] == -1) {
				bHasLost = true;
				for(int i = 0; i < iBoardHeight; i++) {
					for(int j = 0; j < iBoardWidth; j++)
						iBoardVisible[i][j] = 2;
				}
			} else if(iBoard[y][x] == 0) {
				if(x > 0) {
					if(iBoardVisible[y][x - 1] != 2)
						digTile(x - 1, y);
					if(y > 0) {
						if(iBoardVisible[y - 1][x - 1] != 2)
							digTile(x - 1, y - 1);
					}
					if(y < iBoardHeight - 1) {
						if(iBoardVisible[y + 1][x - 1] != 2)
							digTile(x - 1, y + 1);
					}
				}

				if(x < iBoardWidth - 1) {
					if(iBoardVisible[y][x + 1] != 2)
						digTile(x + 1, y);
					if(y > 0) {
						if(iBoardVisible[y - 1][x + 1] != 2)
							digTile(x + 1, y - 1);
					}
					if(y < iBoardHeight - 1) {
						if(iBoardVisible[y + 1][x + 1] != 2)
							digTile(x + 1, y + 1);
					}
				}

				if(y > 0) {
					if(iBoardVisible[y - 1][x] != 2)
						digTile(x, y - 1);
				}

				if(y < iBoardHeight - 1) {
					if(iBoardVisible[y + 1][x] != 2)
						digTile(x, y + 1);
				}
			}
		}

		if(iTilesLeft == 0) {
			bHasWon = true;
			for(int i = 0; i < iBoardHeight; i++) {
				for(int j = 0; j < iBoardWidth; j++)
					iBoardVisible[i][j] = 2;
			}
		}
		drawTile(x, y);
	}

public:
	Example() {
		sAppName = "Minesweeper";
	}

public:
	bool OnUserCreate() override {
		sprTiles[0] = new olc::Sprite("..\\0.png");
		sprTiles[1] = new olc::Sprite("..\\1.png");
		sprTiles[2] = new olc::Sprite("..\\2.png");
		sprTiles[3] = new olc::Sprite("..\\3.png");
		sprTiles[4] = new olc::Sprite("..\\4.png");
		sprTiles[5] = new olc::Sprite("..\\5.png");
		sprTiles[6] = new olc::Sprite("..\\6.png");
		sprTiles[7] = new olc::Sprite("..\\7.png");
		sprTiles[8] = new olc::Sprite("..\\8.png");
		sprTiles[9] = new olc::Sprite("..\\mine.png");
		sprTiles[10] = new olc::Sprite("..\\flag.png");
		sprTiles[11] = new olc::Sprite("..\\hidden.png");
		sprTiles[12] = new olc::Sprite("..\\explosion.png");

		srand(time(NULL));
		for(int i = 0; i < iBoardHeight; i++) {
			for(int j = 0; j < iBoardWidth; j++) {
				iBoard[i][j] = 0;
				iBoardVisible[i][j] = 0;
			}
		}

		while(iMineCount > 0) {
			int iMineX = 0;
			int iMineY = 0;
			while(iBoard[iMineY][iMineX] == -1) {
				iMineX = rand() % iBoardWidth;
				iMineY = rand() % iBoardHeight;
			}
			iBoard[iMineY][iMineX] = -1;

			if(iMineX > 0) {
				if(iBoard[iMineY][iMineX - 1] != -1)
					iBoard[iMineY][iMineX - 1]++;
				if(iMineY > 0) {
					if(iBoard[iMineY - 1][iMineX - 1] != -1)
						iBoard[iMineY - 1][iMineX - 1]++;
				}
				if(iMineY < iBoardHeight - 1) {
					if(iBoard[iMineY + 1][iMineX - 1] != -1)
						iBoard[iMineY + 1][iMineX - 1]++;
				}
			}

			if(iMineX < iBoardWidth - 1) {
				if(iBoard[iMineY][iMineX + 1] != -1)
					iBoard[iMineY][iMineX + 1]++;
				if(iMineY > 0) {
					if(iBoard[iMineY - 1][iMineX + 1] != -1)
						iBoard[iMineY - 1][iMineX + 1]++;
				}
				if(iMineY < iBoardHeight - 1) {
					if(iBoard[iMineY + 1][iMineX + 1] != -1)
						iBoard[iMineY + 1][iMineX + 1]++;
				}
			}

			if(iMineY > 0) {
				if(iBoard[iMineY - 1][iMineX] != -1)
					iBoard[iMineY - 1][iMineX]++;
			}

			if(iMineY < iBoardHeight - 1) {
				if(iBoard[iMineY + 1][iMineX] != -1)
					iBoard[iMineY + 1][iMineX]++;
			}

			iMineCount--;
		}

		for(int i = 0; i < iBoardHeight; i++) {
			for(int j = 0; j < iBoardWidth; j++) {
				DrawSprite(j * 16, i * 16, sprTiles[11]);
			}
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		if(IsFocused()) {
			if(GetKey(olc::ESCAPE).bPressed)
				return false;

			if(GetMouse(0).bPressed || GetMouse(1).bPressed) {
				int iClickX = floor(GetMouseX() / 16);
				int iClickY = floor(GetMouseY() / 16);
				if(GetMouse(0).bPressed) {
					digTile(iClickX, iClickY);
				} else {
					switch(iBoardVisible[iClickY][iClickX]) {
					case 0:
						iBoardVisible[iClickY][iClickX] = 1;
						break;
					case 1:
						iBoardVisible[iClickY][iClickX] = 0;
						break;
					}
					drawTile(iClickX, iClickY);
				}
			}
		}

		if(bHasLost || bHasWon) {
			if(bEndProgram) {
				std::this_thread::sleep_for(std::chrono::seconds(2));
				return false;
			}
			bEndProgram = true;
			for(int i = 0; i < iBoardHeight; i++) {
				for(int j = 0; j < iBoardWidth; j++)
					drawTile(j, i);
			}
		}

		return true;
	}
};


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Example demo;
	if(demo.Construct(iBoardWidth * 16, iBoardHeight * 16, 2, 2, true))
		demo.Start();
	return 0;
}