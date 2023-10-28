#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "vectorlib.h"

using namespace vectorlib;

constexpr int iMaxIterations = 15;
constexpr float fFovX = 3;
constexpr float fFovY = 1.6875;
constexpr float fMinDist = 0.001;
const olc::Pixel bgColour = olc::BLACK;

struct Ray {
    Vector3 pos = Vector3();
    float pitch = 0;
    float yaw = 0;
};

struct Material {
    const olc::Pixel colour;
};

struct Shape {
    Vector3 pos;
    float size;
    Material mat = {olc::WHITE};

    virtual float distance(Ray r) = 0;
    Shape(float x, float y, float z, float size, Material mat)
        : pos(x, y, z), size(size), mat(mat) {}
    ~Shape(){};
};

Ray cam{};

struct Sphere : Shape {
    float distance(Ray r) override {
        return (r.pos - this->pos).magnitude() - this->size;
    }
    using Shape::Shape;
};

Material materials[]{
    {olc::GREEN},
    {olc::BLUE},
};

Shape *shapes[]{
    new Sphere(3, 4, 5, 1, materials[0]),
    new Sphere(5, 5, 2, 1.5, materials[1]),
};

class Example : public olc::PixelGameEngine {
  public:
    Example() {
        sAppName = "Raymarching 3D";
    }

  public:
    bool OnUserCreate() override {
        /* SetCursorPos(960, 540); */
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (GetKey(olc::ESCAPE).bHeld) {
            return false;
        }

        /* POINT p; */
        /* GetCursorPos(&p); */
        /* int iMouseDx = p.x; */
        /* int iMouseDy = p.y; */
        /* SetCursorPos(960, 540); */
        /* GetCursorPos(&p); */
        /* iMouseDx -= p.x; */
        /* iMouseDy -= p.y; */

        /* cam.yaw += 0.03 * iMouseDx; */
        /* cam.pitch += 0.03 * iMouseDy; */

        if (cam.pitch > pi / 2) {
            cam.pitch = pi / 2;
        }
        if (cam.pitch < -pi / 2) {
            cam.pitch = -pi / 2;
        }

        if (GetKey(olc::SPACE).bHeld) {
            cam.pos.y -= 3 * fElapsedTime;
        }
        if (GetKey(olc::SHIFT).bHeld) {
            cam.pos.y += 3 * fElapsedTime;
        }
        if (GetKey(olc::W).bHeld) {
            cam.pos.x += 3 * cosf(cam.yaw) * fElapsedTime;
            cam.pos.z += 3 * sinf(cam.yaw) * fElapsedTime;
        }
        if (GetKey(olc::S).bHeld) {
            cam.pos.x -= 3 * cosf(cam.yaw) * fElapsedTime;
            cam.pos.z -= 3 * sinf(cam.yaw) * fElapsedTime;
        }
        if (GetKey(olc::A).bHeld) {
            cam.pos.x += 3 * sinf(cam.yaw) * fElapsedTime;
            cam.pos.z -= 3 * cosf(cam.yaw) * fElapsedTime;
        }
        if (GetKey(olc::D).bHeld) {
            cam.pos.x -= 3 * sinf(cam.yaw) * fElapsedTime;
            cam.pos.z += 3 * cosf(cam.yaw) * fElapsedTime;
        }

        Clear(bgColour);

        for (int iScreenX = 0; iScreenX < ScreenWidth(); iScreenX++) {
            for (int iScreenY = 0; iScreenY < ScreenHeight(); iScreenY++) {
                Ray ray{
                    cam.pos,
                    cam.pitch +
                        atanf(
                            ((float)iScreenY / ScreenHeight() - 0.5f) * fFovY
                        ),
                    cam.yaw +
                        atanf(((float)iScreenX / ScreenWidth() - 0.5) * fFovX)};
                Vector3 moveDir = vector3FromAngles(ray.pitch, ray.yaw);
                bool bCollided = false;
                float fMarchLength = INFINITY;
                int iIterations = 0;

                while (!bCollided && iIterations++ < iMaxIterations) {
                    Shape *closestShape = nullptr;
                    for (auto currShape : shapes) {
                        float fDist = currShape->distance(ray);
                        if (fDist < fMarchLength) {
                            fMarchLength = fDist;
                            closestShape = currShape;
                        }
                    }

                    ray.pos += moveDir * fMarchLength;

                    if (fMarchLength < fMinDist) {
                        Draw(iScreenX, iScreenY, closestShape->mat.colour);
                        bCollided = true;
                    }
                }
            }
        }

        DrawString(
            0, 0,
            std::to_string(cam.pos.x) + ' ' + std::to_string(cam.pos.y) + ' ' +
                std::to_string(cam.pos.z),
            olc::WHITE
        );
        return true;
    }
};

int main() {
    Example demo;
    if (demo.Construct(240, 135, 4, 4, true)) {
        demo.Start();
    }
    return 0;
}
