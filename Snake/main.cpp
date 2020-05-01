#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <deque>

int iWidth = 80;
int iHeight = 45;
int iDir = 0;
int iFoodX;
int iFoodY;
std::deque<std::pair<int, int>> pParts = {{iWidth / 2 - 2, iHeight / 2}, {iWidth / 2 - 1, iHeight / 2}, {iWidth / 2, iHeight / 2}};


class Example : public olc::PixelGameEngine {
public:
	Example() {
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override {
		srand(time(NULL));
		placeFood();
		return true;
	}

	void placeFood() {
		bool bPlaceable = false;
		while(!bPlaceable) {
			bPlaceable = true;
			iFoodX = rand() % iWidth;
			iFoodY = rand() % iHeight;
			for(auto a : pParts)
				bPlaceable &= !(a.first == iFoodX && a.second == iFoodY);
		}
		Draw(iFoodX, iFoodY, olc::RED);
	}

	bool OnUserUpdate(float fElapsedTime) override {
		int iHeadX = pParts.back().first;
		int iHeadY = pParts.back().second;
		
		for(int a = 0; a < std::size(pParts) - 1; a++) {
			if(iHeadX == pParts[a].first && iHeadY == pParts[a].second)
				return false;
		}

		auto Start = std::chrono::high_resolution_clock::now();
		while(true) {
			if(GetKey(olc::ESCAPE).bHeld)
				return false;

			int iDirNew;
			((GetKey(olc::RIGHT).bHeld || GetKey(olc::NP3).bHeld) && iDir != 2) && (iDirNew = 0);
			((GetKey(olc::UP).bHeld || GetKey(olc::NP5).bHeld) && iDir != 3) && (iDirNew = 1);
			((GetKey(olc::LEFT).bHeld || GetKey(olc::NP1).bHeld) && iDir != 0) && (iDirNew = 2);
			((GetKey(olc::DOWN).bHeld || GetKey(olc::NP2).bHeld) && iDir != 1) && (iDirNew = 3);
			iDir = iDirNew;

			auto End = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> Elapsed = End - Start;
			if(Elapsed.count() >= 100)
				break;
		}

		switch(iDir) {
		case 0:	iHeadX++;	break;
		case 1:	iHeadY--;	break;
		case 2:	iHeadX--;	break;
		case 3:	iHeadY++;	break;
		}

		iHeadX == -1 && (iHeadX = iWidth - 1);
		iHeadX == iWidth && (iHeadX = 0);
		iHeadY == -1 && (iHeadY = iHeight - 1);
		iHeadY == iHeight && (iHeadY = 0);
		
		Draw(pParts[0].first, pParts[0].second, olc::BLACK);
		pParts.pop_front();
		pParts.emplace_back(iHeadX, iHeadY);
		if(iHeadX == iFoodX && iHeadY == iFoodY) {
			placeFood();
			for(int i = 0; i < 3; i++)
				pParts.push_front(pParts[0]);
		}
		Draw(iHeadX, iHeadY, olc::GREEN);

		return true;
	}
};


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Example demo;
	if(demo.Construct(iWidth, iHeight, 8, 8, true))
		demo.Start();
	return 0;
}