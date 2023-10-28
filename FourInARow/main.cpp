#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <memory.h>
#include <memory>

constexpr size_t boardWidth = 7;
constexpr size_t boardHeight = 6;
uint8_t board[boardHeight][boardWidth];
uint8_t turn = 1;
uint8_t winner = 0;
std::unique_ptr<olc::Sprite> sprites[3];
bool bGameEnded = false;

class Example : public olc::PixelGameEngine {
  public:
    Example() {
        sAppName = "Four in a row";
    }

  public:
    bool OnUserCreate() override {
        sprites[0] = std::make_unique<olc::Sprite>("0.png");
        sprites[1] = std::make_unique<olc::Sprite>("1.png");
        sprites[2] = std::make_unique<olc::Sprite>("2.png");
        memset(board, 0, boardWidth * boardHeight);
        for (size_t i = 0; i < boardHeight; i++)
            for (size_t j = 0; j < boardWidth; j++)
                DrawSprite(j * 16, i * 16, sprites[0].get());
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (bGameEnded) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return false;
        } else if (winner != 0) {
            bGameEnded = true;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            Clear(olc::WHITE);
            if (winner == 1)
                DrawString(20, 40, "Red won!", olc::BLACK);
            else if (winner == 2)
                DrawString(20, 40, "Blue won!", olc::BLACK);
            else
                DrawString(25, 40, "Tie!", olc::BLACK);
        } else if (GetMouse(0).bPressed) {
            size_t iSelectedCol = floor(GetMouseX() / 16);
            if (board[0][iSelectedCol] == 0) {
                size_t iTestRow = 0;
                while (iTestRow < boardHeight - 1 &&
                       board[iTestRow + 1][iSelectedCol] == 0)
                    iTestRow++;
                board[iTestRow][iSelectedCol] = turn;
                DrawSprite(
                    iSelectedCol * 16, iTestRow * 16, sprites[turn].get()
                );

                for (size_t i = 0; i < boardHeight; i++) {
                    for (size_t j = 0; j < boardWidth - 3; j++) {
                        for (int k = 0; k < 4; k++)
                            if (board[i][j + k] != turn)
                                goto no_win_1;
                        winner = turn;
                    no_win_1:;
                    }
                }

                for (size_t i = 0; i < boardHeight - 3; i++) {
                    for (size_t j = 0; j < boardWidth; j++) {
                        for (int k = 0; k < 4; k++)
                            if (board[i + k][j] != turn)
                                goto no_win_2;
                        winner = turn;
                    no_win_2:;
                    }
                }

                for (size_t i = 0; i < boardHeight - 3; i++) {
                    for (size_t j = 0; j < boardWidth - 3; j++) {
                        for (int k = 0; k < 4; k++)
                            if (board[i + k][j + k] != turn)
                                goto no_win_3;
                        winner = turn;
                    no_win_3:;
                    }
                }

                for (size_t i = 3; i < boardHeight; i++) {
                    for (size_t j = 0; j < boardWidth - 3; j++) {
                        for (int k = 0; k < 4; k++)
                            if (board[i - k][j + k] != turn)
                                goto no_win_4;
                        winner = turn;
                    no_win_4:;
                    }
                }

                for (size_t i = 0; i < boardHeight; i++)
                    for (size_t j = 0; j < boardWidth; j++)
                        if (board[i][j] == 0)
                            goto no_win_5;
                winner = 3;
            no_win_5:;

                turn = 3 - turn;
            }
        }
        return true;
    }
};

int main() {
    Example demo;
    if (demo.Construct(112, 96, 4, 4))
        demo.Start();

    return 0;
}
