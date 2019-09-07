#include "InstallationWindow.h"
#include "InstallThread.h"

InstallationWindow::InstallationWindow(std::string path) :
	Fl_Window(window_width, window_height, STR_WINDOW_TITLE),
	isCanceled(false), install_path(path)
{
	this->begin();
	title = std::unique_ptr<Fl_Box>(
		new Fl_Box(FL_FRAME_BOX, gui_offset, gui_offset,
			window_width - 2 * gui_offset,
			button_height, STR_WINDOW_TITLE));
	title->color(FL_GRAY - 4);
	title->labelsize(20);
	title->labelfont(FL_BOLD);
	title->labeltype(FL_ENGRAVED_LABEL);
	progress = std::unique_ptr<Fl_Progress>(
		new Fl_Progress(gui_offset,
			button_height + 2 * gui_offset,
			window_width - 2 * gui_offset,
			button_height));
	progress->maximum(1.f);
	progress->minimum(0.f);
	ok = std::unique_ptr<Fl_Button>(
		new Fl_Button(window_width - gui_offset - button_width,
			window_height - gui_offset - button_height,
			button_width,
			button_height, STR_DONE));
	ok->callback(quit_cb, (void*)this);
	ok->deactivate();
	auto task = new InstallThread(this);
	install_thread = std::unique_ptr<std::thread>(
		new std::thread(
			*(task)
		));
	this->end();
	this->callback(window_cb);
}

InstallationWindow::~InstallationWindow() {}

void InstallationWindow::quit_cb(Fl_Widget *, void * data)
{
	auto wnd = static_cast<InstallationWindow*>(data);
	wnd->hide();
	exit(0);
}

void InstallationWindow::finish()
{
	progress->value(1.f);
	ok->activate();
}

void InstallationWindow::waitThr()
{
	install_thread->join();
}

void InstallationWindow::window_cb(Fl_Widget * widget, void *)
{
	auto window = static_cast<InstallationWindow *>(widget);
	window->isCanceled = true;
	window->finish();
	window->waitThr();
	window->hide();
}