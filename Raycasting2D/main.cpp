#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <cmath>
#include <experimental/array>
#include <string_view>

constexpr float fFov = 3;
constexpr float fRayStep = 0.01;
constexpr float fMaxDist = 32;
constexpr float walkSpeed = 4;

// Maze from: https://www.asciiart.eu/art-and-design/mazes
/* constexpr std::string_view map[] = { */
constexpr auto map = std::experimental::fundamentals_v2::make_array<
    std::string_view>(
    "#####################################################################RRRR",
    "#   #               #               #           #                   #   R",
    "#   #               #               #           #                   #   R",
    "#   #               #               #           #                   #   R",
    "#   #   #########   #   #####   #########   #####   #####   #####   #   #",
    "#               #       #   #           #           #   #   #       #   #",
    "#               #       #   #           #           #   #   #       #   #",
    "#               #       #   #           #           #   #   #       #   #",
    "#########   #   #########   #########   #####   #   #   #   #########   #",
    "#       #   #               #           #   #   #   #   #           #   #",
    "#       #   #               #           #   #   #   #   #           #   #",
    "#       #   #               #           #   #   #   #   #           #   #",
    "#   #   #############   #   #   #########   #####   #   #########   #   #",
    "#   #               #   #   #       #           #           #       #   #",
    "#   #               #   #   #       #           #           #       #   #",
    "#   #               #   #   #       #           #           #       #   #",
    "#   #############   #####   #####   #   #####   #########   #   #####   #",
    "#           #       #   #       #   #       #           #   #           #",
    "#           #       #   #       #   #       #           #   #           #",
    "#           #       #   #       #   #       #           #   #           #",
    "#   #####   #####   #   #####   #   #########   #   #   #   #############",
    "#       #       #   #   #       #       #       #   #   #       #       #",
    "#       #       #   #   #       #       #       #   #   #       #       #",
    "#       #       #   #   #       #       #       #   #   #       #       #",
    "#############   #   #   #   #########   #   #####   #   #####   #####   #",
    "#           #   #           #       #   #       #   #       #           #",
    "#           #   #           #       #   #       #   #       #           #",
    "#           #   #           #       #   #       #   #       #           #",
    "#   #####   #   #########   #####   #   #####   #####   #############   #",
    "#   #       #           #           #       #   #   #               #   #",
    "#   #       #           #           #       #   #   #               #   #",
    "#   #       #           #           #       #   #   #               #   #",
    "#   #   #########   #   #####   #########   #   #   #############   #   #",
    "#   #           #   #   #   #   #           #               #   #       #",
    "#   #           #   #   #   #   #           #               #   #       #",
    "#   #           #   #   #   #   #           #               #   #       #",
    "#   #########   #   #   #   #####   #########   #########   #   #########",
    "#   #       #   #   #           #           #   #       #               #",
    "#   #       #   #   #           #           #   #       #               #",
    "#   #       #   #   #           #           #   #       #               #",
    "#   #   #####   #####   #####   #########   #####   #   #########   #   #",
    "G   #                   #           #               #               #   #",
    "G   #                   #           #               #               #   #",
    "G   #                   #           #               #               #   #",
    "GGGG#####################################################################"
);

class Example : public olc::PixelGameEngine {
    float fPlayerX = 70;
    float fPlayerY = 3;
    float fPlayerDir = 0;
    bool bDrawMinimap = false;

  public:
    Example() {
        sAppName = "Raycasting";
    }

  public:
    bool OnUserCreate() override {
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (GetKey(olc::ESCAPE).bPressed) {
            return false;
        }
        if (GetKey(olc::M).bPressed) {
            bDrawMinimap = !bDrawMinimap;
        }

        if (GetKey(olc::J).bHeld) {
            fPlayerDir -= 2 * fElapsedTime;
        }
        if (GetKey(olc::K).bHeld) {
            fPlayerDir += 2 * fElapsedTime;
        }

        float fPlayerOldX = fPlayerX;
        float fPlayerOldY = fPlayerY;

        if (GetKey(olc::W).bHeld) {
            fPlayerX += walkSpeed * cosf(fPlayerDir) * fElapsedTime;
            fPlayerY += walkSpeed * sinf(fPlayerDir) * fElapsedTime;
        }
        if (GetKey(olc::S).bHeld) {
            fPlayerX -= walkSpeed * cosf(fPlayerDir) * fElapsedTime;
            fPlayerY -= walkSpeed * sinf(fPlayerDir) * fElapsedTime;
        }
        if (GetKey(olc::D).bHeld) {
            fPlayerX -= walkSpeed * sinf(fPlayerDir) * fElapsedTime;
            fPlayerY += walkSpeed * cosf(fPlayerDir) * fElapsedTime;
        }
        if (GetKey(olc::A).bHeld) {
            fPlayerX += walkSpeed * sinf(fPlayerDir) * fElapsedTime;
            fPlayerY -= walkSpeed * cosf(fPlayerDir) * fElapsedTime;
        }

        while (map[(int)(fPlayerY - 0.4)][(int)(fPlayerX - 0.4)] != ' ' ||
               map[fPlayerY - 0.4][fPlayerX + 0.4] != ' ' ||
               map[fPlayerY + 0.4][fPlayerX - 0.4] != ' ' ||
               map[fPlayerY + 0.4][fPlayerX + 0.4] != ' ') {
            fPlayerX += std::copysign(0.01f, fPlayerOldX - fPlayerX);
            fPlayerY += std::copysign(0.01f, fPlayerOldY - fPlayerY);
        }

        int iShade;
        for (size_t x = 0; x < ScreenHeight() / 2; x++) {
            iShade = 60 - (float)x / ScreenHeight() * 120;
            if (iShade < 0) {
                iShade = 0;
            }
            DrawLine(
                0, x, ScreenWidth(), x, olc::Pixel(iShade, iShade, iShade)
            );
            DrawLine(
                0, ScreenHeight() - x, ScreenWidth(), ScreenHeight() - x,
                olc::Pixel(iShade, iShade, iShade)
            );
        }

        for (int iRayNum = 0; iRayNum < ScreenWidth(); iRayNum++) {
            float fRayDist = 0;
            const float fRayDir =
                fPlayerDir +
                atan(((float)iRayNum / ScreenWidth() - 0.5) * fFov);
            const float fEyeX = cosf(fRayDir);
            const float fEyeY = sinf(fRayDir);
            int iTestX, iTestY;

            do {
                fRayDist += fRayStep;
                iTestX = (int)(fPlayerX + fEyeX * fRayDist);
                iTestY = (int)(fPlayerY + fEyeY * fRayDist);
            } while (fRayDist < fMaxDist && map[iTestY][iTestX] == ' ');

            if (fRayDist < fMaxDist) {
                fRayDist *= cos(fPlayerDir - fRayDir);
                int iCeiling =
                    (float)(ScreenHeight() / 2) - ScreenHeight() / fRayDist;
                int iFloor = ScreenHeight() - iCeiling;
                int iShadeR = 255 - fRayDist * 12;
                if (iShadeR < 10) {
                    iShadeR = 10;
                }
                int iShadeG = iShadeR;
                int iShadeB = iShadeR;

                switch (map[iTestY][iTestX]) {
                case 'R':
                    iShadeG = 0;
                    iShadeB = 0;
                    break;
                case 'G':
                    iShadeR = 0;
                    iShadeB = 0;
                    break;
                case 'B':
                    iShadeR = 0;
                    iShadeG = 0;
                    break;
                }
                DrawLine(
                    iRayNum, iCeiling, iRayNum, iFloor,
                    olc::Pixel(iShadeR, iShadeG, iShadeB)
                );
            }
        }

        if (bDrawMinimap) {
            for (size_t y = 0; y < map.size(); y++) {
                for (size_t x = 0; x < map[0].length(); x++) {
                    if (map[y][x] != ' ') {
                        FillRect(x * 5 + 3, y * 5 + 3, 5, 5, olc::BLUE);
                    }
                }
            }
            DrawLine(
                fPlayerX * 5 + 3, fPlayerY * 5 + 3,
                fPlayerX * 5 + 3 + 6 * cos(fPlayerDir),
                fPlayerY * 5 + 3 + 6 * sin(fPlayerDir), olc::GREY
            );
            FillCircle(fPlayerX * 5 + 3, fPlayerY * 5 + 3, 2, olc::RED);
        }

        return true;
    }
};

int main() {
    Example demo;
    if (demo.Construct(1600, 900, 1, 1, true)) {
        demo.Start();
    }

    return 0;
}
