#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <cmath>

constexpr int iMaxIterations = 128;
long double dCenterY = 0;
long double dSize = 2;
long double dCenterX = 0;

class Example : public olc::PixelGameEngine {
public:
	Example() {
		sAppName = "Mandelbrot";
	}

public:
	bool OnUserCreate() override {
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		if(GetKey(olc::ESCAPE).bPressed)
			return false;
		if(GetKey(olc::LEFT).bHeld)
			dCenterX -= 0.5 * dSize * fElapsedTime;
		if(GetKey(olc::RIGHT).bHeld)
			dCenterX += 0.5 * dSize * fElapsedTime;
		if(GetKey(olc::UP).bHeld)
			dCenterY -= 0.5 * dSize * fElapsedTime;
		if(GetKey(olc::DOWN).bHeld)
			dCenterY += 0.5 * dSize * fElapsedTime;
		if(GetKey(olc::Key::Q).bHeld)
			dSize += 0.5 * dSize * fElapsedTime;
		if(GetKey(olc::Key::W).bHeld)
			dSize -= 0.5 * dSize * fElapsedTime;

		for(int x = 0; x < ScreenWidth(); x++) {
			for(int y = 0; y < ScreenHeight(); y++) {
				long double a = (2 * (long double)x / ScreenWidth() - 1) * dSize + dCenterX;
				long double b = (2 * (long double)y / ScreenHeight() - 1) * dSize + dCenterY;
				unsigned int iIterations = 0;
				long double a2 = a;
				long double b2 = b;

				while(++iIterations < iMaxIterations && a * a + b * b < 4)
					b = 2 * std::exchange(a, a * a - b * b + a2) * b + b2;
				uint8_t l = iIterations == iMaxIterations ? 0 : 255 * sqrt((float)iIterations / iMaxIterations);
				Draw(x, y, olc::Pixel(l, l, l));
			}
		}
		return true;
	}
};


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Example demo;
	if(demo.Construct(512, 512, 2, 2, true))
		demo.Start();

	return 0;
}