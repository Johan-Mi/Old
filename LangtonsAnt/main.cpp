#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <chrono>
#include <memory.h>

constexpr size_t width = 384;
constexpr size_t height = 216;

class Example : public olc::PixelGameEngine {
    bool bBoard[height][width];
    size_t iAntX = width / 2;
    size_t iAntY = height / 2;
    struct {
        uint8_t value : 2;
    } iAntDir;

  public:
    Example() {
        sAppName = "Langton's ant";
    }

  public:
    bool OnUserCreate() override {
        memset(bBoard, 0, width * height);
        iAntDir.value = 0;
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (GetKey(olc::ESCAPE).bHeld)
            return false;
        iAntDir.value += bBoard[iAntY][iAntX] ? 1 : -1;
        bBoard[iAntY][iAntX] ^= true;
        Draw(iAntX, iAntY, bBoard[iAntY][iAntX] ? olc::WHITE : olc::BLACK);
        switch (iAntDir.value) {
        case 0:
            iAntX++;
            break;
        case 1:
            iAntY--;
            break;
        case 2:
            iAntX--;
            break;
        case 3:
            iAntY++;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return true;
    }
};

int main() {
    Example demo;
    if (demo.Construct(width, height, 8, 8, true))
        demo.Start();

    return 0;
}
