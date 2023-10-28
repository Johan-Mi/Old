#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <memory>

class Example : public olc::PixelGameEngine {
    std::unique_ptr<olc::Sprite> image;

  public:
    Example() {
        sAppName = "Example";
    }

  public:
    bool OnUserCreate() override {
        image = std::make_unique<olc::Sprite>("smiley.png");
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::WHITE);
        DrawSprite(GetMouseX(), GetMouseY(), image.get());
        return true;
    }
};

int main() {
    Example demo;
    if (demo.Construct(120, 100, 2, 2))
        demo.Start();

    return 0;
}
