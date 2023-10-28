#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <cmath>
#include <ctime>

constexpr int iPaddleSize = 10;
constexpr float pi = 3.1415926535;
constexpr float fPaddleSpeed = 200;
constexpr int iBallRadius = 2;

class Example : public olc::PixelGameEngine {
    float fPlayer;
    float fComp;
    float fBallX;
    float fBallY;
    float fBallDir;
    float fBallSpeed = 200;
    int iPlayerScore = 0;
    int iCompScore = 0;
    bool bHitEdge = false;
    std::clock_t timer;

  public:
    Example() {
        sAppName = "Example";
    }

  public:
    void placeBall() {
        fBallX = ScreenWidth() / 2;
        fBallY = ScreenHeight() / 2;
        fBallDir = pi * (0.25 + rand() % 4);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        timer = std::clock();
    }

    bool OnUserCreate() override {
        srand(time(NULL));

        fPlayer = ScreenHeight() / 2;
        fComp = ScreenHeight() / 2;
        placeBall();
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (fElapsedTime > 1)
            fElapsedTime = 0;

        fBallSpeed = 200 + (std::clock() - timer) / 1000;
        if (GetKey(olc::ESCAPE).bHeld)
            return false;

        Clear(olc::BLACK);
        DrawLine(0, 16, ScreenWidth(), 16, olc::WHITE);
        DrawString(
            ScreenWidth() / 2 - 16, 5,
            std::to_string(iPlayerScore) + " - " + std::to_string(iCompScore),
            olc::WHITE
        );

        if (GetKey(olc::UP).bHeld) {
            fPlayer -= fPaddleSpeed * fElapsedTime;
            if (fPlayer < iPaddleSize + 16)
                fPlayer = iPaddleSize + 16;
        }
        if (GetKey(olc::DOWN).bHeld) {
            fPlayer += fPaddleSpeed * fElapsedTime;
            if (fPlayer > ScreenHeight() - iPaddleSize)
                fPlayer = ScreenHeight() - iPaddleSize;
        }

        fBallX += cosf(fBallDir) * fBallSpeed * fElapsedTime;
        fBallY += sinf(fBallDir) * fBallSpeed * fElapsedTime;
        if (fBallY < 16 + iBallRadius) {
            fBallY = 16 + iBallRadius;
            fBallDir = -fBallDir;
        } else if (fBallY > ScreenHeight() - iBallRadius) {
            fBallY = ScreenHeight() - iBallRadius;
            fBallDir = -fBallDir;
        }

        if (fBallX < 18 + iBallRadius && fBallX > 16 - iBallRadius &&
            std::abs(fBallY - fPlayer) < iPaddleSize) {
            fBallX = 18 + iBallRadius;
            fBallDir = pi - fBallDir;
        } else if (fBallX > ScreenWidth() - 18 - iBallRadius && fBallX < ScreenWidth() - 16 + iBallRadius && std::abs(fBallY - fComp) < iPaddleSize) {
            fBallX = ScreenWidth() - 18 - iBallRadius;
            fBallDir = pi - fBallDir;
        }

        if (fBallX > ScreenWidth() / 2 && fBallY - fComp < -8) {
            fComp -= fPaddleSpeed * fElapsedTime;
            if (fComp < iPaddleSize + 16)
                fComp = iPaddleSize + 16;
        }
        if (fBallX > ScreenWidth() / 2 && fBallY - fComp > 8) {
            fComp += fPaddleSpeed * fElapsedTime;
            if (fComp > ScreenHeight() - iPaddleSize)
                fComp = ScreenHeight() - iPaddleSize;
        }

        if (fBallX < 0) {
            iCompScore++;
            placeBall();
        } else if (fBallX > ScreenWidth()) {
            iPlayerScore++;
            placeBall();
        }

        FillRect(16, fPlayer - iPaddleSize, 2, iPaddleSize * 2, olc::WHITE);
        FillRect(
            ScreenWidth() - 18, fComp - iPaddleSize, 2, iPaddleSize * 2,
            olc::WHITE
        );
        FillCircle(fBallX, fBallY, iBallRadius, olc::WHITE);

        return true;
    }
};

int main() {
    Example demo;
    if (demo.Construct(320, 180, 4, 4, true))
        demo.Start();

    return 0;
}
