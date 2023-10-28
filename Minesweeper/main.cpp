#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <chrono>
#include <ctime>
#include <memory.h>
#include <memory>

constexpr size_t boardWidth = 43;
constexpr size_t boardHeight = 24;

class Example : public olc::PixelGameEngine {
    int iMineCount = 90;
    int iTilesLeft = boardWidth * boardHeight - iMineCount;
    int iBoard[boardHeight][boardWidth];
    int iBoardVisible[boardHeight][boardWidth];
    bool bHasLost = false;
    bool bHasWon = false;
    bool bEndProgram = false;
    std::unique_ptr<olc::Sprite> sprTiles[13];

    void drawTile(size_t x, size_t y) {
        DrawSprite(
            x * 16, y * 16,
            sprTiles
                [iBoardVisible[y][x] == 0
                     ? 11
                     : (iBoardVisible[y][x] == 1
                            ? 10
                            : (iBoard[y][x] == -1 ? (bHasLost ? 12 : 9)
                                                  : iBoard[y][x]))]
                    .get()
        );
    }
    void digTile(size_t x, size_t y) {
        if (iBoardVisible[y][x] == 0) {
            iBoardVisible[y][x] = 2;
            iTilesLeft--;
            if (iBoard[y][x] == -1) {
                bHasLost = true;
                for (size_t i = 0; i < boardHeight; i++)
                    for (size_t j = 0; j < boardWidth; j++)
                        iBoardVisible[i][j] = 2;
            } else if (iBoard[y][x] == 0) {
                if (x > 0) {
                    if (iBoardVisible[y][x - 1] != 2)
                        digTile(x - 1, y);
                    if (y > 0)
                        if (iBoardVisible[y - 1][x - 1] != 2)
                            digTile(x - 1, y - 1);
                    if (y < boardHeight - 1)
                        if (iBoardVisible[y + 1][x - 1] != 2)
                            digTile(x - 1, y + 1);
                }

                if (x < boardWidth - 1) {
                    if (iBoardVisible[y][x + 1] != 2)
                        digTile(x + 1, y);
                    if (y > 0)
                        if (iBoardVisible[y - 1][x + 1] != 2)
                            digTile(x + 1, y - 1);
                    if (y < boardHeight - 1)
                        if (iBoardVisible[y + 1][x + 1] != 2)
                            digTile(x + 1, y + 1);
                }

                if (y > 0)
                    if (iBoardVisible[y - 1][x] != 2)
                        digTile(x, y - 1);

                if (y < boardHeight - 1)
                    if (iBoardVisible[y + 1][x] != 2)
                        digTile(x, y + 1);
            }
        }

        if (iTilesLeft == 0) {
            bHasWon = true;
            for (size_t i = 0; i < boardHeight; i++)
                for (size_t j = 0; j < boardWidth; j++)
                    iBoardVisible[i][j] = 2;
        }
        drawTile(x, y);
    }

  public:
    Example() {
        sAppName = "Minesweeper";
    }

  public:
    bool OnUserCreate() override {
        sprTiles[0] = std::make_unique<olc::Sprite>("0.png");
        sprTiles[1] = std::make_unique<olc::Sprite>("1.png");
        sprTiles[2] = std::make_unique<olc::Sprite>("2.png");
        sprTiles[3] = std::make_unique<olc::Sprite>("3.png");
        sprTiles[4] = std::make_unique<olc::Sprite>("4.png");
        sprTiles[5] = std::make_unique<olc::Sprite>("5.png");
        sprTiles[6] = std::make_unique<olc::Sprite>("6.png");
        sprTiles[7] = std::make_unique<olc::Sprite>("7.png");
        sprTiles[8] = std::make_unique<olc::Sprite>("8.png");
        sprTiles[9] = std::make_unique<olc::Sprite>("mine.png");
        sprTiles[10] = std::make_unique<olc::Sprite>("flag.png");
        sprTiles[11] = std::make_unique<olc::Sprite>("hidden.png");
        sprTiles[12] = std::make_unique<olc::Sprite>("explosion.png");

        srand(time(NULL));
        for (size_t i = 0; i < boardHeight; i++) {
            for (size_t j = 0; j < boardWidth; j++) {
                iBoard[i][j] = 0;
                iBoardVisible[i][j] = 0;
            }
        }

        while (iMineCount > 0) {
            size_t iMineX = 0;
            size_t iMineY = 0;
            while (iBoard[iMineY][iMineX] == -1) {
                iMineX = rand() % boardWidth;
                iMineY = rand() % boardHeight;
            }
            iBoard[iMineY][iMineX] = -1;

            if (iMineX > 0) {
                if (iBoard[iMineY][iMineX - 1] != -1)
                    iBoard[iMineY][iMineX - 1]++;
                if (iMineY > 0)
                    if (iBoard[iMineY - 1][iMineX - 1] != -1)
                        iBoard[iMineY - 1][iMineX - 1]++;
                if (iMineY < boardHeight - 1)
                    if (iBoard[iMineY + 1][iMineX - 1] != -1)
                        iBoard[iMineY + 1][iMineX - 1]++;
            }

            if (iMineX < boardWidth - 1) {
                if (iBoard[iMineY][iMineX + 1] != -1)
                    iBoard[iMineY][iMineX + 1]++;
                if (iMineY > 0)
                    if (iBoard[iMineY - 1][iMineX + 1] != -1)
                        iBoard[iMineY - 1][iMineX + 1]++;
                if (iMineY < boardHeight - 1)
                    if (iBoard[iMineY + 1][iMineX + 1] != -1)
                        iBoard[iMineY + 1][iMineX + 1]++;
            }

            if (iMineY > 0)
                if (iBoard[iMineY - 1][iMineX] != -1)
                    iBoard[iMineY - 1][iMineX]++;

            if (iMineY < boardHeight - 1)
                if (iBoard[iMineY + 1][iMineX] != -1)
                    iBoard[iMineY + 1][iMineX]++;

            iMineCount--;
        }

        for (size_t i = 0; i < boardHeight; i++)
            for (size_t j = 0; j < boardWidth; j++)
                DrawSprite(j * 16, i * 16, sprTiles[11].get());

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (IsFocused()) {
            if (GetKey(olc::ESCAPE).bPressed)
                return false;

            if (GetMouse(0).bPressed || GetMouse(1).bPressed) {
                size_t iClickX = floor(GetMouseX() / 16);
                size_t iClickY = floor(GetMouseY() / 16);
                if (GetMouse(0).bPressed) {
                    digTile(iClickX, iClickY);
                } else {
                    switch (iBoardVisible[iClickY][iClickX]) {
                    case 0:
                        iBoardVisible[iClickY][iClickX] = 1;
                        break;
                    case 1:
                        iBoardVisible[iClickY][iClickX] = 0;
                        break;
                    }
                    drawTile(iClickX, iClickY);
                }
            }
        }

        if (bHasLost || bHasWon) {
            if (bEndProgram) {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                return false;
            }
            bEndProgram = true;
            for (size_t i = 0; i < boardHeight; i++) {
                for (size_t j = 0; j < boardWidth; j++)
                    drawTile(j, i);
            }
        }

        return true;
    }
};

int main() {
    Example demo;
    if (demo.Construct(boardWidth * 16, boardHeight * 16, 2, 2, true))
        demo.Start();
    return 0;
}
