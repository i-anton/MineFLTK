#pragma once
#include "stdafx.h"

class MainWindow : public Fl_Window
{
public:
	MainWindow();
	~MainWindow() {}
private:
	static void quit_cb(Fl_Widget*, void*);
	static void open_cb(Fl_Widget*, void*);
	static void install_cb(Fl_Widget*, void*);
	void update_path(std::string new_path);
private:
	std::unique_ptr<Fl_Box> title;
	std::unique_ptr<Fl_Output> install_dest;
	std::unique_ptr<Fl_Button> select_install_dest;
	std::unique_ptr<Fl_Button> start_install;
	std::string install_path;
};
