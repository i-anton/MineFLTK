#include "stdafx.h"
#include "MainWindow.h"

int main()
{
	auto window = std::unique_ptr<MainWindow>(new MainWindow());
	window->show();
	return Fl::run();
}