#include <Windows.h>
#include <thread>
#include <chrono>
#include <iostream>

bool active = false;
int clickSpeed = 10;
POINT p;

int main() {
	while(!(GetAsyncKeyState(VK_NUMPAD0) & 0x8000)) {
		if(GetAsyncKeyState(VK_NUMPAD1) & 0x1) {
			active ^= 1;
			GetCursorPos(&p);
		}
		if(active) {
			mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(clickSpeed));
		}
		if(GetAsyncKeyState(VK_NUMPAD2) & 0x1) {
			active = false;
			system("cls");
			std::cout << "Enter a click delay in milliseconds\n";
			std::cin >> clickSpeed;
			system("cls");
		}
	}
	return 0;
}