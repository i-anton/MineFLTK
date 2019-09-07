#pragma once
#include "stdafx.h"
class AboutWindow : public Fl_Window
{
public:
	AboutWindow();
	~AboutWindow();
	static void quit_cb(Fl_Widget*, void*);
private:
	std::unique_ptr<Fl_Button> ok;
	std::unique_ptr<Fl_Box> title;
	std::unique_ptr<Fl_Box> desc;
};

