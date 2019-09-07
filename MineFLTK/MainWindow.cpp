#include "MainWindow.h"
#include "AboutWindow.h"
#include <FL/fl_ask.H>

constexpr unsigned int menu_height = 25;
constexpr unsigned int window_width = button_width * TABLE_ROWS;
constexpr unsigned int window_height = window_width + menu_height;
MainWindow::MainWindow() :
	Fl_Window(window_width, window_height),
	first_click(true)
{
	this->label(S_MAIN_WINDOW);
	srand(static_cast<unsigned int>(time(0)));
	this->begin();
	initFieldGUI();
	buildMenu();
	this->end();
	model = new Minesweeper();
}

MainWindow::~MainWindow()
{
	delete model;
}

void MainWindow::initFieldGUI()
{
	int id = 0;
	int ycoord = menu_height;
	for (size_t y = 0; y < TABLE_ROWS; ++y)
	{
		int xcoord = 0;
		for (size_t x = 0; x < TABLE_ROWS; ++x)
		{
			buttons[y][x] = std::unique_ptr<Fl_Button>(
				new Fl_Button(xcoord, ycoord, button_width, button_width, u8" "));
			buttons[y][x]->callback(cell_cb, (void*)(new CellClickEvent{ this,x,y }));
			xcoord += button_width;
		}
		ycoord += button_width;
	}
}

void MainWindow::buildMenu()
{
	menu = std::unique_ptr<Fl_Menu_Bar>(
		new Fl_Menu_Bar(0, 0, window_width, menu_height));
	menu->add(S_MENU_RESET, FL_CTRL + 'r', reset_cb, (void*)this);
	menu->add(S_MENU_EXIT, FL_CTRL + 'q', quit_cb, (void*)this);
	menu->add(S_MENU_ABOUT, FL_F + 1, about_cb, (void*)this);
}

void MainWindow::quit_cb(Fl_Widget *, void *)
{
	exit(0);
}

void MainWindow::about_cb(Fl_Widget *, void *)
{
	auto w = std::unique_ptr<AboutWindow>(new AboutWindow());
	w->show();
	while (w->shown()) Fl::wait();
}

void MainWindow::reset_cb(Fl_Widget *, void * window)
{
	static_cast<MainWindow*>(window)->resetModel();
}

void MainWindow::cell_cb(Fl_Widget *, void * value)
{
	auto ev = static_cast<CellClickEvent*>(value);
	ev->win->onClick(ev->x, ev->y, Fl::event_button());
}

void MainWindow::resetModel()
{
	delete model;
	model = new Minesweeper();
	updateWidgets();
	first_click = true;
}

void MainWindow::updateWidgets()
{
	for (size_t y = 0; y < TABLE_ROWS; y++)
	{
		for (size_t x = 0; x < TABLE_ROWS; x++)
		{
			Cell st = model->getCell(x, y);
			auto button = buttons[y][x].get();
			if (st.marked)
			{
				button->label(u8"P");
				button->redraw();
				continue;
			}
			if (st.opened)
			{
				button->deactivate();
				button->label((st.neighbours == -1) ? u8"*" : vals[st.neighbours]);
			}
			else
			{
				button->activate();
				button->label(u8" ");
			}
			button->redraw();
		}
	}
}

void MainWindow::onClick(unsigned int x, unsigned int y, int rmb)
{
	if (model->getState() != GameState::Playing)
	{
		return;
	}
	if (rmb == 3)
	{
		model->mark(x, y);
		updateWidgets();
		return;
	}
	else if (rmb == 1)
	{
		if (first_click)
		{
			model->generate(x, y, 20);
			first_click = false;
		}
		model->reveal(x, y);
		auto st = model->getState();
		updateWidgets();
		if (st == GameState::Win)
		{
			fl_alert(S_WIN_GAME);
		}
		else if (st == GameState::Lose)
		{
			fl_alert(S_LOSE_GAME);
		}
	}
}
