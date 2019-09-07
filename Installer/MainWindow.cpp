#include "MainWindow.h"
#include "InstallationWindow.h"
#include <FL/fl_ask.H>

MainWindow::MainWindow() :
	Fl_Window(window_width, window_height, STR_WINDOW_TITLE)
{
	this->label();
	srand(static_cast<unsigned int>(time(0)));
	this->begin();
	title = std::unique_ptr<Fl_Box>(
		new Fl_Box(FL_FRAME_BOX, gui_offset, gui_offset,
			window_width - 2 * gui_offset,
			button_height, STR_WINDOW_TITLE));
	title->color(FL_GRAY - 4);
	title->labelsize(20);
	title->labelfont(FL_BOLD);
	title->labeltype(FL_ENGRAVED_LABEL);
	start_install = std::unique_ptr<Fl_Button>(
		new Fl_Button(window_width - gui_offset - button_width,
			window_height - gui_offset - button_height,
			button_width,
			button_height, STR_DONE));
	start_install->shortcut(FL_Enter);
	start_install->callback(install_cb,
		(void*)this);
	start_install->deactivate();
	install_dest = std::unique_ptr<Fl_Output>(
		new Fl_Output(6 * gui_offset,
			button_height + 2 * gui_offset,
			window_width - 8 * gui_offset - button_width,
			button_height, STR_INSTALL_PATH));
	select_install_dest = std::unique_ptr<Fl_Button>(
		new Fl_Button(window_width - gui_offset - button_width,
			button_height + 2 * gui_offset,
			button_width,
			button_height, STR_CHANGE_PATH));
	select_install_dest->shortcut(FL_Enter);
	select_install_dest->callback(open_cb,
		(void*)this);
	this->end();
}

void MainWindow::quit_cb(Fl_Widget *, void *)
{
	exit(0);
}

void MainWindow::open_cb(Fl_Widget *, void * data)
{
	Fl_Native_File_Chooser fc;
	fc.title(u8"Installation path");
	fc.type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);
	int result = fc.show();
	if (result == 0) 
	{
		static_cast<MainWindow*>(data)->update_path(fc.filename());
	}
	else if (result == -1)
	{
		fl_alert(fc.errmsg());
	}
}

void MainWindow::install_cb(Fl_Widget *, void *data)
{
	auto wnd = static_cast<MainWindow*>(data);
	wnd->hide();
	auto install = std::unique_ptr<InstallationWindow>(
		new InstallationWindow(wnd->install_path));
	install->show();
	while (install->shown()) Fl::wait();
}

void MainWindow::update_path(std::string new_path)
{
	install_path = new_path.c_str();
	install_dest->value(new_path.c_str());
	start_install->activate();
}

