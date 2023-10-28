#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <chrono>
#include <ctime>
#include <memory.h>

constexpr size_t width = 192;
constexpr size_t height = 108;
bool bBoard[height + 2][width + 2];
bool bBuffer[height + 2][width + 2];

class GameOfLife : public olc::PixelGameEngine {
  public:
    GameOfLife() {
        sAppName = "Conway's game of life";
    }

  public:
    bool OnUserCreate() override {
        srand(time(NULL));
        for (size_t i = 1; i <= height; i++) {
            for (size_t j = 1; j <= width; j++)
                bBoard[i][j] = rand() % 2;
        }
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (GetKey(olc::ESCAPE).bHeld)
            return false;
        for (size_t i = 1; i <= height; i++) {
            for (size_t j = 1; j <= width; j++) {
                Draw(j, i, bBoard[i][j] ? olc::WHITE : olc::BLACK);
                int iNeighbours = 0;
                for (int m = -1; m < 2; m++)
                    for (int n = -1; n < 2; n++)
                        iNeighbours += bBoard[i + m][j + n];
                bBuffer[i][j] = bBoard[i][j]
                                    ? (iNeighbours == 3 || iNeighbours == 4)
                                    : (iNeighbours == 3);
            }
        }
        memcpy(bBoard, bBuffer, sizeof(bBuffer));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return true;
    }
};

int main() {
    GameOfLife game;
    if (game.Construct(width, height, 8, 8, true))
        game.Start();

    return 0;
}
