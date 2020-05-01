#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <chrono>

constexpr int iWidth = 384, iHeight = 216;
bool bBoard[iHeight][iWidth];
int iAntX = iWidth / 2, iAntY = iHeight / 2;
struct { unsigned int value : 2; } iAntDir;

class Example : public olc::PixelGameEngine {
public:
	Example() {
		sAppName = "Langton's ant";
	}

public:
	bool OnUserCreate() override {
		for(int i = 0; i < iHeight; i++) {
			for(int j = 0; j < iWidth; j++)
				bBoard[i][j] = false;
		}
		iAntDir.value = 0;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		if(GetKey(olc::ESCAPE).bHeld)
			return false;
		iAntDir.value += bBoard[iAntY][iAntX] ? 1 : -1;
		bBoard[iAntY][iAntX] ^= true;
		Draw(iAntX, iAntY, bBoard[iAntY][iAntX] ? olc::WHITE : olc::BLACK);
		switch(iAntDir.value) {
		case 0:	iAntX++;	break;
		case 1:	iAntY--;	break;
		case 2:	iAntX--;	break;
		case 3:	iAntY++;	break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		return true;
	}
};

int main() {
	Example demo;
	if(demo.Construct(iWidth, iHeight, 8, 8, true))
		demo.Start();

	return 0;
}