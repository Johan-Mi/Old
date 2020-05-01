#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
olc::Sprite* image;

class Example : public olc::PixelGameEngine {
public:
	Example() {
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override {
		image = new olc::Sprite("..\\smiley.png");
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		Clear(olc::WHITE);
		DrawSprite(GetMouseX(), GetMouseY(), image);
		return true;
	}
};


int main() {
	Example demo;
	if(demo.Construct(120, 100, 2, 2))
		demo.Start();

	return 0;
}