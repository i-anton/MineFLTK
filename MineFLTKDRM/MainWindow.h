#pragma once
#include "stdafx.h"
#include "Minesweeper.h"
#include <FL/Fl_Menu_Bar.H>

class MainWindow : public Fl_Window
{
public:
	MainWindow();
	~MainWindow();
private:
	void initFieldGUI();
	void buildMenu();
	static void quit_cb(Fl_Widget*,void*);
	static void about_cb(Fl_Widget*, void*);
	static void reset_cb(Fl_Widget*, void*);
	static void cell_cb(Fl_Widget*, void*);
private:
	void resetModel();
	void updateWidgets();
	void onClick(unsigned int x, unsigned int y,int rmb);
private:
	std::unique_ptr<Fl_Menu_Bar> menu;
	std::unique_ptr<Fl_Button> buttons[TABLE_ROWS][TABLE_ROWS];
	std::wstring hpath;
	std::string uname;
	std::string spath;
	std::string hash_user_combo;
	unsigned int ws_here = 0xDEADBEEF;
	Minesweeper* model;
	bool first_click;
	const char * vals[9] = { " ","1","2","3","4","5","6","7","8" };
};
struct CellClickEvent {
	MainWindow* win;
	unsigned int x, y;
};
