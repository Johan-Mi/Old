#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <cmath>

constexpr unsigned int iMaxIterations = 128;

class Mandelbrot : public olc::PixelGameEngine {
    long double dCenterY = 0;
    long double dSize = 2;
    long double dCenterX = 0;

  public:
    Mandelbrot() {
        sAppName = "Mandelbrot";
    }

  public:
    bool OnUserCreate() override {
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (GetKey(olc::ESCAPE).bPressed)
            return false;
        if (GetKey(olc::A).bHeld)
            dCenterX -= 0.5 * dSize * fElapsedTime;
        if (GetKey(olc::D).bHeld)
            dCenterX += 0.5 * dSize * fElapsedTime;
        if (GetKey(olc::W).bHeld)
            dCenterY -= 0.5 * dSize * fElapsedTime;
        if (GetKey(olc::S).bHeld)
            dCenterY += 0.5 * dSize * fElapsedTime;
        if (GetKey(olc::Key::Q).bHeld)
            dSize *= 1 + 0.5 * fElapsedTime;
        if (GetKey(olc::Key::E).bHeld)
            dSize /= 1 + 0.5 * fElapsedTime;

        for (int x = 0; x < ScreenWidth(); x++) {
            for (int y = 0; y < ScreenHeight(); y++) {
                long double a =
                    (2 * (long double)x / ScreenWidth() - 1) * dSize + dCenterX;
                long double b =
                    (2 * (long double)y / ScreenHeight() - 1) * dSize +
                    dCenterY;
                unsigned int iIterations = 0;
                long double a2 = a;
                long double b2 = b;

                while (++iIterations < iMaxIterations && a * a + b * b < 4)
                    b = 2 * std::exchange(a, a * a - b * b + a2) * b + b2;
                uint8_t l =
                    iIterations == iMaxIterations
                        ? 0
                        : 255 * sqrt((float)iIterations / iMaxIterations);
                Draw(x, y, olc::Pixel(l, l, l));
            }
        }
        return true;
    }
};

int main() {
    Mandelbrot mandelbrot;
    if (mandelbrot.Construct(512, 512, 2, 2, true))
        mandelbrot.Start();

    return 0;
}
