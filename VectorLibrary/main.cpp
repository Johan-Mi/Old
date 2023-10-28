#include <iostream>
#include "vectorlib.h"

using namespace vectorlib;

int main() {
	Vector2 v = vector2FromAngle(pi / 4);
	std::cout << v << '\n';
	return 0;
}
