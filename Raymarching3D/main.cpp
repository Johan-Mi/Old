#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "vectorlib.h"
using namespace vectorlib;

#pragma region Defintions
constexpr int iMaxIterations = 15;
constexpr float
fFovX = 3,
fFovY = 1.6875,
fMinDist = 0.001;
const olc::Pixel bgColour = olc::BLACK;
struct Ray { Vector3 pos = Vector3(); float pitch = 0, yaw = 0; };
struct Material { const olc::Pixel colour; };
struct Shape {
	Vector3 pos;
	float size;
	Material mat = {olc::WHITE};
	virtual float distance(Ray r) = 0;
	Shape(float x, float y, float z, float inSize, Material inMat) : pos(x, y, z), size(inSize), mat(inMat) {}
	~Shape() {};
};
Ray camera{};

struct Sphere : Shape {
	float distance(Ray r) override { return (r.pos - this->pos).magnitude() - this->size; }
	Sphere(float x, float y, float z, float inSize, Material inMat) : Shape(x, y, z, inSize, inMat) {}
};
#pragma endregion

#pragma region Materials and shapes
Material materials[]{
	{olc::GREEN},
	{olc::BLUE},
};

Shape* shapes[] {
	new Sphere(3, 4, 5, 1, materials[0]),
	new Sphere(5, 5, 2, 1.5, materials[1]),
};

#pragma endregion

class Example : public olc::PixelGameEngine {
public:
	Example() { sAppName = "Raymarching 3D"; }

public:
	bool OnUserCreate() override {
		SetCursorPos(960, 540);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
#pragma region Controls
		if(GetKey(olc::ESCAPE).bHeld)
			return false;

		POINT p;
		GetCursorPos(&p);
		int iMouseDx = p.x;
		int iMouseDy = p.y;
		SetCursorPos(960, 540);
		GetCursorPos(&p);
		iMouseDx -= p.x;
		iMouseDy -= p.y;

		camera.yaw += 0.03 * iMouseDx;
		camera.pitch += 0.03 * iMouseDy;

		if(camera.pitch > pi / 2)
			camera.pitch = pi / 2;
		if(camera.pitch < -pi / 2)
			camera.pitch = -pi / 2;

		if(GetKey(olc::SPACE).bHeld)
			camera.pos.y -= 3 * fElapsedTime;
		if(GetKey(olc::SHIFT).bHeld)
			camera.pos.y += 3 * fElapsedTime;
		if(GetKey(olc::W).bHeld) {
			camera.pos.x += 3 * cosf(camera.yaw) * fElapsedTime;
			camera.pos.z += 3 * sinf(camera.yaw) * fElapsedTime;
		}
		if(GetKey(olc::S).bHeld) {
			camera.pos.x -= 3 * cosf(camera.yaw) * fElapsedTime;
			camera.pos.z -= 3 * sinf(camera.yaw) * fElapsedTime;
		}
		if(GetKey(olc::A).bHeld) {
			camera.pos.x += 3 * sinf(camera.yaw) * fElapsedTime;
			camera.pos.z -= 3 * cosf(camera.yaw) * fElapsedTime;
		}
		if(GetKey(olc::D).bHeld) {
			camera.pos.x -= 3 * sinf(camera.yaw) * fElapsedTime;
			camera.pos.z += 3 * cosf(camera.yaw) * fElapsedTime;
		}
#pragma endregion

#pragma region Rendering
		Clear(bgColour);

		for(int iScreenX = 0; iScreenX < ScreenWidth(); iScreenX++) {
			for(int iScreenY = 0; iScreenY < ScreenHeight(); iScreenY++) {
				Ray ray{camera.pos, camera.pitch + atanf(((float)iScreenY / ScreenHeight() - 0.5) * fFovY), camera.yaw + atanf(((float)iScreenX / ScreenWidth() - 0.5) * fFovX)};
				Vector3 moveDir = vector3FromAngles(ray.pitch, ray.yaw);
				bool bCollided = false;
				float fMarchLength = INFINITY;
				int iIterations = 0;

				while(!bCollided && iIterations++ < iMaxIterations) {
					Shape* closestShape = nullptr;
					for(int currentShape = 0; currentShape < sizeof(shapes) / sizeof(shapes[0]); currentShape++) {
						float fDist = shapes[currentShape]->distance(ray);
						if(fDist < fMarchLength) {
							fMarchLength = fDist;
							closestShape = shapes[currentShape];
						}
					}

					ray.pos += moveDir * fMarchLength;

					if(fMarchLength < fMinDist) {
						Draw(iScreenX, iScreenY, closestShape->mat.colour);
						bCollided = true;
					}
				}
			}
		}
#pragma endregion

#pragma region Overlay
		DrawString(0, 0, std::to_string(camera.pos.x) + ' ' + std::to_string(camera.pos.y) + ' ' + std::to_string(camera.pos.z), olc::WHITE);
#pragma endregion
		return true;
	}
};

int main() {
	Example demo;
	if(demo.Construct(240, 135, 4, 4, true))
		demo.Start();
	return 0;
}