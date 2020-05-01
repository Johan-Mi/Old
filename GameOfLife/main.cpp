#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <ctime>
#include <chrono>

constexpr int iWidth = 192, iHeight = 108;
bool bBoard[iHeight + 2][iWidth + 2], bBuffer[iHeight + 2][iWidth + 2];

class Example : public olc::PixelGameEngine {
public:
	Example() {
		sAppName = "Conway's game of life";
	}

public:
	bool OnUserCreate() override {
		srand(time(NULL));
		for(int i = 1; i <= iHeight; i++) {
			for(int j = 1; j <= iWidth; j++)
				bBoard[i][j] = rand() % 2;
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		if(GetKey(olc::ESCAPE).bHeld)
			return false;
		for(int i = 1; i <= iHeight; i++) {
			for(int j = 1; j <= iWidth; j++) {
				Draw(j, i, bBoard[i][j] ? olc::WHITE : olc::BLACK);
				int iNeighbours = 0;
				for(int m = -1; m < 2; m++) {
					for(int n = -1; n < 2; n++)
						iNeighbours += bBoard[i + m][j + n];
				}
				bBuffer[i][j] = bBoard[i][j] ? (iNeighbours == 3 || iNeighbours == 4) : (iNeighbours == 3);
			}
		}
		memcpy(bBoard, bBuffer, sizeof(bBuffer));
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		return true;
	}
};

int main() {
	Example demo;
	if(demo.Construct(iWidth, iHeight, 8, 8, true))
		demo.Start();

	return 0;
}