#include "OSStatic.h"
#include "Windows.h"

std::pair<int, int> get_desktop_resolution() {
	auto res = std::pair<int, int>();

	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	res.first = desktop.right;
	res.second = desktop.bottom;

	return res;
}