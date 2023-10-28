#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <deque>

constexpr size_t width = 80;
constexpr size_t height = 45;
int iDir = 0;
int iFoodX;
int iFoodY;
std::deque<std::pair<int, int>> pParts = {
    {width / 2 - 2, height / 2},
    {width / 2 - 1, height / 2},
    {width / 2, height / 2}};

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
        while (!bPlaceable) {
            bPlaceable = true;
            iFoodX = rand() % width;
            iFoodY = rand() % height;
            for (auto a : pParts) {
                bPlaceable &= !(a.first == iFoodX && a.second == iFoodY);
            }
        }
        Draw(iFoodX, iFoodY, olc::RED);
    }

    bool OnUserUpdate(float fElapsedTime) override {
        int iHeadX = pParts.back().first;
        int iHeadY = pParts.back().second;

        for (auto &part : pParts) {
            if (iHeadX == part.first && iHeadY == part.second &&
                &part != &pParts.back()) {
                return false;
            }
        }

        auto Start = std::chrono::high_resolution_clock::now();
        while (true) {
            if (GetKey(olc::ESCAPE).bHeld) {
                return false;
            }

            int iDirNew = iDir;
            if ((GetKey(olc::RIGHT).bHeld || GetKey(olc::D).bHeld) &&
                iDir != 2) {
                iDirNew = 0;
            }
            if ((GetKey(olc::UP).bHeld || GetKey(olc::W).bHeld) && iDir != 3) {
                iDirNew = 1;
            }
            if ((GetKey(olc::LEFT).bHeld || GetKey(olc::A).bHeld) &&
                iDir != 0) {
                iDirNew = 2;
            }
            if ((GetKey(olc::DOWN).bHeld || GetKey(olc::S).bHeld) &&
                iDir != 1) {
                iDirNew = 3;
            }
            iDir = iDirNew;

            auto End = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> Elapsed = End - Start;
            if (Elapsed.count() >= 100) {
                break;
            }
        }

        switch (iDir) {
        case 0:
            iHeadX++;
            break;
        case 1:
            iHeadY--;
            break;
        case 2:
            iHeadX--;
            break;
        case 3:
            iHeadY++;
            break;
        }

        if (iHeadX == -1) {
            iHeadX = width - 1;
        } else if (iHeadX == width) {
            iHeadX = 0;
        }
        if (iHeadY == -1) {
            iHeadY = height - 1;
        } else if (iHeadY == height) {
            iHeadY = 0;
        }

        Draw(pParts[0].first, pParts[0].second, olc::BLACK);
        pParts.pop_front();
        pParts.emplace_back(iHeadX, iHeadY);
        if (iHeadX == iFoodX && iHeadY == iFoodY) {
            placeFood();
            for (int i = 0; i < 3; i++) {
                pParts.push_front(pParts[0]);
            }
        }
        Draw(iHeadX, iHeadY, olc::GREEN);

        return true;
    }
};

int main() {
    Example demo;
    if (demo.Construct(width, height, 8, 8, true)) {
        demo.Start();
    }
    return 0;
}
