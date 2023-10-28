#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

constexpr float pi = 3.1415926535f;
constexpr float fMinDrawDistance = 0.02f;

struct Point2D {
    float x, y;
};
struct Point3D {
    float x, y, z;
};
struct Line3D {
    Point3D p1, p2;
};
struct Tri2D {
    Point2D p1, p2, p3;
    olc::Pixel colour = olc::WHITE;
};
struct Tri3D {
    Point3D p1, p2, p3;
    olc::Pixel colour = olc::WHITE;
};
struct Camera {
    float x, y, z, pitch, yaw;
};

std::vector<std::pair<Tri2D, float>> storedTris;
Camera mainCamera{14, 3, 7, 0, pi};
bool bRenderPoints = false;
bool bRenderLines = false;
bool bRenderTris = true;

const std::vector<Point3D> points = {

};
const std::vector<Line3D> lines = {

};
const std::vector<Tri3D> tris = {
    {{0, 1, 0}, {1, 1, 0}, {0, 4, 0}, olc::CYAN},
    {{1, 4, 0}, {1, 1, 0}, {0, 4, 0}, olc::CYAN},

    {{1, 0, 0}, {1, 1, 0}, {3, 0, 0}, olc::CYAN},
    {{3, 1, 0}, {1, 1, 0}, {3, 0, 0}, olc::CYAN},

    {{1, 4, 0}, {1, 5, 0}, {3, 4, 0}, olc::CYAN},
    {{3, 5, 0}, {1, 5, 0}, {3, 4, 0}, olc::CYAN},

    {{2, 1, 0}, {2, 2, 0}, {3, 1, 0}, olc::CYAN},
    {{3, 2, 0}, {2, 2, 0}, {3, 1, 0}, olc::CYAN},

    {{4, 0, 0}, {5, 0, 0}, {4, 5, 0}, olc::YELLOW},
    {{5, 5, 0}, {5, 0, 0}, {4, 5, 0}, olc::YELLOW},

    {{6, 0, 0}, {7, 0, 0}, {6, 2, 0}, olc::YELLOW},
    {{7, 2, 0}, {7, 0, 0}, {6, 2, 0}, olc::YELLOW},

    {{6, 3, 0}, {7, 3, 0}, {6, 4, 0}, olc::YELLOW},
    {{7, 4, 0}, {7, 3, 0}, {6, 4, 0}, olc::YELLOW},

    {{5, 2, 0}, {6, 2, 0}, {5, 3, 0}, olc::YELLOW},
    {{6, 3, 0}, {6, 2, 0}, {5, 3, 0}, olc::YELLOW},

    {{5, 4, 0}, {6, 4, 0}, {5, 5, 0}, olc::YELLOW},
    {{6, 5, 0}, {6, 4, 0}, {5, 5, 0}, olc::YELLOW},

    {{8, 0, 0}, {9, 0, 0}, {8, 4, 0}, olc::RED},
    {{9, 4, 0}, {9, 0, 0}, {8, 4, 0}, olc::RED},

    {{10, 0, 0}, {11, 0, 0}, {10, 4, 0}, olc::RED},
    {{11, 4, 0}, {11, 0, 0}, {10, 4, 0}, olc::RED},

    {{9, 2, 0}, {10, 2, 0}, {9, 3, 0}, olc::RED},
    {{10, 3, 0}, {10, 2, 0}, {9, 3, 0}, olc::RED},

    {{9, 4, 0}, {10, 4, 0}, {9, 5, 0}, olc::RED},
    {{10, 5, 0}, {10, 4, 0}, {9, 5, 0}, olc::RED},

    {{13, 0, 0}, {14, 0, 0}, {13, 4, 0}, olc::MAGENTA},
    {{14, 4, 0}, {14, 0, 0}, {13, 4, 0}, olc::MAGENTA},

    {{12, 4, 0}, {15, 4, 0}, {12, 5, 0}, olc::MAGENTA},
    {{15, 5, 0}, {15, 4, 0}, {12, 5, 0}, olc::MAGENTA},

    {{17, 0, 0}, {18, 0, 0}, {17, 4, 0}, olc::GREEN},
    {{18, 4, 0}, {18, 0, 0}, {17, 4, 0}, olc::GREEN},

    {{16, 4, 0}, {19, 4, 0}, {16, 5, 0}, olc::GREEN},
    {{19, 5, 0}, {19, 4, 0}, {16, 5, 0}, olc::GREEN},

    {{20, 0, 0}, {21, 0, 0}, {20, 5, 0}, olc::CYAN},
    {{21, 5, 0}, {21, 0, 0}, {20, 5, 0}, olc::CYAN},

    {{22, 0, 0}, {24, 0, 0}, {22, 1, 0}, olc::YELLOW},
    {{24, 1, 0}, {24, 0, 0}, {22, 1, 0}, olc::YELLOW},

    {{22, 3, 0}, {23, 3, 0}, {22, 4, 0}, olc::YELLOW},
    {{23, 4, 0}, {23, 3, 0}, {22, 4, 0}, olc::YELLOW},

    {{24, 1, 0}, {25, 1, 0}, {24, 2, 0}, olc::YELLOW},
    {{25, 2, 0}, {25, 1, 0}, {24, 2, 0}, olc::YELLOW},

    {{23, 2, 0}, {24, 2, 0}, {23, 3, 0}, olc::YELLOW},
    {{24, 3, 0}, {24, 2, 0}, {23, 3, 0}, olc::YELLOW},

    {{23, 4, 0}, {25, 4, 0}, {23, 5, 0}, olc::YELLOW},
    {{25, 5, 0}, {25, 4, 0}, {23, 5, 0}, olc::YELLOW},

    {{26, 0, 0}, {27, 0, 0}, {26, 1, 0}, olc::RED},
    {{27, 1, 0}, {27, 0, 0}, {26, 1, 0}, olc::RED},

    {{26, 2, 0}, {27, 2, 0}, {26, 5, 0}, olc::RED},
    {{27, 5, 0}, {27, 2, 0}, {26, 5, 0}, olc::RED},
};

Point2D pointToScreen(Point3D p) {
    return {p.x / p.z, p.y / p.z};
}
inline void addTri(Tri2D t, float depth) {
    storedTris.emplace_back(t, depth);
}

Point3D pointCameraTransform(Point3D p, Camera c) {
    p.x -= c.x;
    p.y -= c.y;
    p.z -= c.z;
    Point3D midpoint{
        p.x * cosf(c.yaw) + p.z * sinf(c.yaw), p.y,
        -p.x * sinf(c.yaw) + p.z * cosf(c.yaw)};
    return Point3D{
        midpoint.x, midpoint.y * cosf(c.pitch) + midpoint.z * sinf(c.pitch),
        -midpoint.y * sinf(c.pitch) + midpoint.z * cosf(c.pitch)};
}

Point3D nearPlaneLineIntersection(Point3D inP1, Point3D inP2) {
    float t = (fMinDrawDistance - inP1.z) / (inP2.z - inP1.z);
    return Point3D{
        inP1.x + t * (inP2.x - inP1.x), inP1.y + t * (inP2.y - inP1.y),
        fMinDrawDistance};
}

class Example : public olc::PixelGameEngine {
  public:
    Example() {
        sAppName = "Example";
    }

  public:
    bool pointOnScreen(Point2D p) {
        return p.x > 0 && p.x < ScreenWidth() && p.y > 0 &&
               p.y < ScreenHeight();
    }
    void renderPoint(Point2D p) {
        DrawCircle(
            ScreenWidth() / 2 - 200 * p.x, ScreenHeight() / 2 - 200 * p.y, 2,
            olc::WHITE
        );
    }
    void renderLine(Point2D p1, Point2D p2) {
        DrawLine(
            ScreenWidth() / 2 - 200 * p1.x, ScreenHeight() / 2 - 200 * p1.y,
            ScreenWidth() / 2 - 200 * p2.x, ScreenHeight() / 2 - 200 * p2.y,
            olc::WHITE
        );
    }

    void renderClippedTri(Tri2D t) {
        auto clippedTriangles =
            triClipDown(triClipUp(triClipRight(triClipLeft(std::vector<Tri2D>{
                {Point2D{
                     ScreenWidth() / 2 - 200 * t.p1.x,
                     ScreenHeight() / 2 - 200 * t.p1.y},
                 Point2D{
                     ScreenWidth() / 2 - 200 * t.p2.x,
                     ScreenHeight() / 2 - 200 * t.p2.y},
                 Point2D{
                     ScreenWidth() / 2 - 200 * t.p3.x,
                     ScreenHeight() / 2 - 200 * t.p3.y},
                 t.colour}}))));
        for (auto k : clippedTriangles)
            FillTriangle(
                k.p1.x, k.p1.y, k.p2.x, k.p2.y, k.p3.x, k.p3.y, k.colour
            );
    }

    Point2D
    lineIntersect2D(Point2D inP1, Point2D inP2, bool bAxis, bool bSide) {
        return bAxis
                   ? (bSide
                          ? Point2D{inP1.x + ((float)ScreenHeight() - inP1.y) * (inP2.x - inP1.x) / (inP2.y - inP1.y), (float)ScreenHeight()}
                          : Point2D{inP1.x - inP1.y * (inP2.x - inP1.x) / (inP2.y - inP1.y), 0}
                     )
                   : (bSide
                          ? Point2D{(float)ScreenWidth(), inP1.y + ((float)ScreenWidth() - inP1.x) * (inP2.y - inP1.y) / (inP2.x - inP1.x)}
                          : Point2D{
                                0, inP1.y - inP1.x * (inP2.y - inP1.y) /
                                                (inP2.x - inP1.x)});
    }

    std::vector<Tri2D> triClipLeft(std::vector<Tri2D> inTris) {
        std::vector<Tri2D> outTris;
        for (auto h : inTris) {
            Point2D p1 = h.p1;
            Point2D p2 = h.p2;
            Point2D p3 = h.p3;
            switch ((p1.x >= 0) + (p2.x >= 0) + (p3.x >= 0)) {
            case 1: {
                Point2D *frontPoint;
                Point2D *backPoint1;
                Point2D *backPoint2;
                if (p1.x >= 0) {
                    frontPoint = &p1;
                    backPoint1 = &p2;
                    backPoint2 = &p3;
                } else if (p2.x >= 0) {
                    frontPoint = &p2;
                    backPoint1 = &p1;
                    backPoint2 = &p3;
                } else {
                    frontPoint = &p3;
                    backPoint1 = &p2;
                    backPoint2 = &p1;
                }
                *backPoint1 =
                    lineIntersect2D(*backPoint1, *frontPoint, false, false);
                *backPoint2 =
                    lineIntersect2D(*backPoint2, *frontPoint, false, false);
                outTris.emplace_back(p1, p2, p3, h.colour);
            } break;

            case 2: {
                Point2D *frontPoint1;
                Point2D *frontPoint2;
                Point2D *backPoint;
                if (p1.x < 0) {
                    frontPoint1 = &p2;
                    frontPoint2 = &p3;
                    backPoint = &p1;
                } else if (p2.x < 0) {
                    frontPoint1 = &p1;
                    frontPoint2 = &p3;
                    backPoint = &p2;
                } else {
                    frontPoint1 = &p1;
                    frontPoint2 = &p2;
                    backPoint = &p3;
                }
                Point2D backPointDraw1 =
                    lineIntersect2D(*backPoint, *frontPoint1, false, false);
                Point2D backPointDraw2 =
                    lineIntersect2D(*backPoint, *frontPoint2, false, false);
                outTris.emplace_back(
                    *frontPoint1, *frontPoint2, backPointDraw1, h.colour
                );
                outTris.emplace_back(
                    backPointDraw2, *frontPoint2, backPointDraw1, h.colour
                );
            } break;

            case 3:
                outTris.push_back(h);
                break;
            }
        }
        return outTris;
    }

    std::vector<Tri2D> triClipRight(std::vector<Tri2D> inTris) {
        std::vector<Tri2D> outTris;
        for (auto h : inTris) {
            Point2D p1 = h.p1;
            Point2D p2 = h.p2;
            Point2D p3 = h.p3;
            switch ((p1.x <= ScreenWidth()) + (p2.x <= ScreenWidth()) +
                    (p3.x <= ScreenWidth())) {
            case 1: {
                Point2D *frontPoint;
                Point2D *backPoint1;
                Point2D *backPoint2;
                if (p1.x <= ScreenWidth()) {
                    frontPoint = &p1;
                    backPoint1 = &p2;
                    backPoint2 = &p3;
                } else if (p2.x <= ScreenWidth()) {
                    frontPoint = &p2;
                    backPoint1 = &p1;
                    backPoint2 = &p3;
                } else {
                    frontPoint = &p3;
                    backPoint1 = &p2;
                    backPoint2 = &p1;
                }
                *backPoint1 =
                    lineIntersect2D(*backPoint1, *frontPoint, false, true);
                *backPoint2 =
                    lineIntersect2D(*backPoint2, *frontPoint, false, true);
                outTris.emplace_back(p1, p2, p3, h.colour);
            } break;

            case 2: {
                Point2D *frontPoint1;
                Point2D *frontPoint2;
                Point2D *backPoint;
                if (p1.x >= ScreenWidth()) {
                    frontPoint1 = &p2;
                    frontPoint2 = &p3;
                    backPoint = &p1;
                } else if (p2.x >= ScreenWidth()) {
                    frontPoint1 = &p1;
                    frontPoint2 = &p3;
                    backPoint = &p2;
                } else {
                    frontPoint1 = &p1;
                    frontPoint2 = &p2;
                    backPoint = &p3;
                }
                Point2D backPointDraw1 =
                    lineIntersect2D(*backPoint, *frontPoint1, false, true);
                Point2D backPointDraw2 =
                    lineIntersect2D(*backPoint, *frontPoint2, false, true);
                outTris.emplace_back(
                    *frontPoint1, *frontPoint2, backPointDraw1, h.colour
                );
                outTris.emplace_back(
                    backPointDraw2, *frontPoint2, backPointDraw1, h.colour
                );
            } break;

            case 3:
                outTris.push_back(h);
                break;
            }
        }
        return outTris;
    }

    std::vector<Tri2D> triClipUp(std::vector<Tri2D> inTris) {
        std::vector<Tri2D> outTris;
        for (auto h : inTris) {
            Point2D p1 = h.p1;
            Point2D p2 = h.p2;
            Point2D p3 = h.p3;
            switch ((p1.y >= 0) + (p2.y >= 0) + (p3.y >= 0)) {
            case 1: {
                Point2D *frontPoint;
                Point2D *backPoint1;
                Point2D *backPoint2;
                if (p1.y >= 0) {
                    frontPoint = &p1;
                    backPoint1 = &p2;
                    backPoint2 = &p3;
                } else if (p2.y >= 0) {
                    frontPoint = &p2;
                    backPoint1 = &p1;
                    backPoint2 = &p3;
                } else {
                    frontPoint = &p3;
                    backPoint1 = &p2;
                    backPoint2 = &p1;
                }
                *backPoint1 =
                    lineIntersect2D(*backPoint1, *frontPoint, true, false);
                *backPoint2 =
                    lineIntersect2D(*backPoint2, *frontPoint, true, false);
                outTris.emplace_back(p1, p2, p3, h.colour);
            } break;

            case 2: {
                Point2D *frontPoint1;
                Point2D *frontPoint2;
                Point2D *backPoint;
                if (p1.y < 0) {
                    frontPoint1 = &p2;
                    frontPoint2 = &p3;
                    backPoint = &p1;
                } else if (p2.y < 0) {
                    frontPoint1 = &p1;
                    frontPoint2 = &p3;
                    backPoint = &p2;
                } else {
                    frontPoint1 = &p1;
                    frontPoint2 = &p2;
                    backPoint = &p3;
                }
                Point2D backPointDraw1 =
                    lineIntersect2D(*backPoint, *frontPoint1, true, false);
                Point2D backPointDraw2 =
                    lineIntersect2D(*backPoint, *frontPoint2, true, false);
                outTris.emplace_back(
                    *frontPoint1, *frontPoint2, backPointDraw1, h.colour
                );
                outTris.emplace_back(
                    backPointDraw2, *frontPoint2, backPointDraw1, h.colour
                );
            } break;

            case 3:
                outTris.push_back(h);
                break;
            }
        }
        return outTris;
    }

    std::vector<Tri2D> triClipDown(std::vector<Tri2D> inTris) {
        std::vector<Tri2D> outTris;
        for (auto h : inTris) {
            Point2D p1 = h.p1;
            Point2D p2 = h.p2;
            Point2D p3 = h.p3;
            switch ((p1.y <= ScreenHeight()) + (p2.y <= ScreenHeight()) +
                    (p3.y <= ScreenHeight())) {
            case 1: {
                Point2D *frontPoint;
                Point2D *backPoint1;
                Point2D *backPoint2;
                if (p1.y <= ScreenHeight()) {
                    frontPoint = &p1;
                    backPoint1 = &p2;
                    backPoint2 = &p3;
                } else if (p2.y <= ScreenHeight()) {
                    frontPoint = &p2;
                    backPoint1 = &p1;
                    backPoint2 = &p3;
                } else {
                    frontPoint = &p3;
                    backPoint1 = &p2;
                    backPoint2 = &p1;
                }
                *backPoint1 =
                    lineIntersect2D(*backPoint1, *frontPoint, true, true);
                *backPoint2 =
                    lineIntersect2D(*backPoint2, *frontPoint, true, true);
                outTris.emplace_back(p1, p2, p3, h.colour);
            } break;

            case 2: {
                Point2D *frontPoint1;
                Point2D *frontPoint2;
                Point2D *backPoint;
                if (p1.y >= ScreenHeight()) {
                    frontPoint1 = &p2;
                    frontPoint2 = &p3;
                    backPoint = &p1;
                } else if (p2.y >= ScreenHeight()) {
                    frontPoint1 = &p1;
                    frontPoint2 = &p3;
                    backPoint = &p2;
                } else {
                    frontPoint1 = &p1;
                    frontPoint2 = &p2;
                    backPoint = &p3;
                }
                Point2D backPointDraw1 =
                    lineIntersect2D(*backPoint, *frontPoint1, true, true);
                Point2D backPointDraw2 =
                    lineIntersect2D(*backPoint, *frontPoint2, true, true);
                outTris.emplace_back(
                    *frontPoint1, *frontPoint2, backPointDraw1, h.colour
                );
                outTris.emplace_back(
                    backPointDraw2, *frontPoint2, backPointDraw1, h.colour
                );
            } break;

            case 3:
                outTris.push_back(h);
                break;
            }
        }
        return outTris;
    }

    void pointPipeline(Point3D p) {
        Point3D p1 = pointCameraTransform(p, mainCamera);
        if (p1.z >= fMinDrawDistance)
            renderPoint(pointToScreen(p1));
    }

    void linePipeline(Line3D l) {
        Point3D p1 = pointCameraTransform(l.p1, mainCamera);
        Point3D p2 = pointCameraTransform(l.p2, mainCamera);
        if (p1.z > fMinDrawDistance || p2.z > fMinDrawDistance) {
            if (p1.z <= fMinDrawDistance || p2.z <= fMinDrawDistance) {
                Point3D intersection = nearPlaneLineIntersection(p1, p2);
                if (p1.z <= fMinDrawDistance)
                    p1 = intersection;
                else
                    p2 = intersection;
            }
            renderLine(pointToScreen(p1), pointToScreen(p2));
        }
    }

    void triPipeline(Tri3D t) {
        Point3D p1 = pointCameraTransform(t.p1, mainCamera);
        Point3D p2 = pointCameraTransform(t.p2, mainCamera);
        Point3D p3 = pointCameraTransform(t.p3, mainCamera);
        switch ((p1.z >= fMinDrawDistance) + (p2.z >= fMinDrawDistance) +
                (p3.z >= fMinDrawDistance)) {
        case 1: {
            Point3D *frontPoint;
            Point3D *backPoint1;
            Point3D *backPoint2;
            if (p1.z >= fMinDrawDistance) {
                frontPoint = &p1;
                backPoint1 = &p2;
                backPoint2 = &p3;
            } else if (p2.z >= fMinDrawDistance) {
                frontPoint = &p2;
                backPoint1 = &p1;
                backPoint2 = &p3;
            } else {
                frontPoint = &p3;
                backPoint1 = &p2;
                backPoint2 = &p1;
            }
            *backPoint1 = nearPlaneLineIntersection(*backPoint1, *frontPoint);
            *backPoint2 = nearPlaneLineIntersection(*backPoint2, *frontPoint);
            addTri(
                Tri2D{
                    pointToScreen(p1), pointToScreen(p2), pointToScreen(p3),
                    t.colour},
                p1.z + p2.z + p3.z
            );
        } break;

        case 2: {
            Point3D *frontPoint1;
            Point3D *frontPoint2;
            Point3D *backPoint;
            if (p1.z < fMinDrawDistance) {
                frontPoint1 = &p2;
                frontPoint2 = &p3;
                backPoint = &p1;
            } else if (p2.z < fMinDrawDistance) {
                frontPoint1 = &p1;
                frontPoint2 = &p3;
                backPoint = &p2;
            } else {
                frontPoint1 = &p1;
                frontPoint2 = &p2;
                backPoint = &p3;
            }
            Point3D backPointDraw1 =
                nearPlaneLineIntersection(*backPoint, *frontPoint1);
            Point3D backPointDraw2 =
                nearPlaneLineIntersection(*backPoint, *frontPoint2);
            addTri(
                Tri2D{
                    pointToScreen(*frontPoint1), pointToScreen(*frontPoint2),
                    pointToScreen(backPointDraw1), t.colour},
                p1.z + p2.z + p3.z
            );
            addTri(
                Tri2D{
                    pointToScreen(backPointDraw2), pointToScreen(*frontPoint2),
                    pointToScreen(backPointDraw1), t.colour},
                p1.z + p2.z + p3.z
            );
        } break;

        case 3:
            addTri(
                Tri2D{
                    pointToScreen(p1), pointToScreen(p2), pointToScreen(p3),
                    t.colour},
                p1.z + p2.z + p3.z
            );
            break;
        }
    }

    bool OnUserCreate() override {
        SetCursorPos(960, 540);
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (GetKey(olc::ESCAPE).bHeld)
            return false;

        POINT p;
        GetCursorPos(&p);
        int iMouseDx = p.x;
        int iMouseDy = p.y;
        SetCursorPos(960, 540);
        GetCursorPos(&p);
        iMouseDx -= p.x;
        iMouseDy -= p.y;

        mainCamera.yaw += 0.04f * iMouseDx;
        mainCamera.pitch += 0.04f * iMouseDy;

        if (mainCamera.pitch > pi / 2)
            mainCamera.pitch = pi / 2;
        if (mainCamera.pitch < -pi / 2)
            mainCamera.pitch = -pi / 2;

        if (GetKey(olc::SPACE).bHeld)
            mainCamera.y += 3 * fElapsedTime;
        if (GetKey(olc::SHIFT).bHeld)
            mainCamera.y -= 3 * fElapsedTime;
        if (GetKey(olc::A).bHeld) {
            mainCamera.x += 3 * cosf(mainCamera.yaw) * fElapsedTime;
            mainCamera.z += 3 * sinf(mainCamera.yaw) * fElapsedTime;
        }
        if (GetKey(olc::D).bHeld) {
            mainCamera.x -= 3 * cosf(mainCamera.yaw) * fElapsedTime;
            mainCamera.z -= 3 * sinf(mainCamera.yaw) * fElapsedTime;
        }
        if (GetKey(olc::S).bHeld) {
            mainCamera.x += 3 * sinf(mainCamera.yaw) * fElapsedTime;
            mainCamera.z -= 3 * cosf(mainCamera.yaw) * fElapsedTime;
        }
        if (GetKey(olc::W).bHeld) {
            mainCamera.x -= 3 * sinf(mainCamera.yaw) * fElapsedTime;
            mainCamera.z += 3 * cosf(mainCamera.yaw) * fElapsedTime;
        }
        if (GetKey(olc::K1).bPressed)
            bRenderPoints ^= true;
        if (GetKey(olc::K2).bPressed)
            bRenderLines ^= true;
        if (GetKey(olc::K3).bPressed)
            bRenderTris ^= true;

        Clear(olc::BLACK);

        if (bRenderTris) {
            storedTris.clear();
            for (auto i : tris)
                triPipeline(i);
            std::sort(
                storedTris.begin(), storedTris.end(),
                [](std::pair<Tri2D, float> element1,
                   std::pair<Tri2D, float> element2) {
                    return element1.second > element2.second;
                }
            );
            for (auto i : storedTris)
                renderClippedTri(i.first);
        }
        if (bRenderLines)
            for (auto i : lines)
                linePipeline(i);
        if (bRenderPoints)
            for (auto i : points)
                pointPipeline(i);

        return true;
    }
};

int main() {
    Example demo;
    if (demo.Construct(960, 540, 4, 4, true))
        demo.Start();
    return 0;
}
