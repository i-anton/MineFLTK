#pragma once
#include "stdafx.h"

class InstallationWindow : public Fl_Window
{
public:
	InstallationWindow(std::string path);
	~InstallationWindow();
	static void quit_cb(Fl_Widget*, void*);
	void finish();
	void waitThr();
	static void window_cb(Fl_Widget *widget, void *);
public:
	bool isCanceled;
	std::unique_ptr<Fl_Progress> progress;
	std::string install_path;
private:
	std::unique_ptr<Fl_Button> ok;
	std::unique_ptr<Fl_Box> title;
	std::unique_ptr<std::thread> install_thread;
};
