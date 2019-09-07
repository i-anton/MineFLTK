#include "AboutWindow.h"

AboutWindow::AboutWindow():
	Fl_Window(208, 192)
{
	this->label(S_ABOUT_WINDOW);
	set_modal();
	title = std::unique_ptr<Fl_Box>(
		new Fl_Box(FL_FRAME_BOX,8, 8, 192, 32, S_ABOUT_TITLE));
	title->color(FL_GRAY - 4);
	title->labelsize(20);
	title->labelfont(FL_BOLD);
	title->labeltype(FL_ENGRAVED_LABEL);
	desc = std::unique_ptr<Fl_Box>(
		new Fl_Box(FL_FRAME_BOX, 8, 48, 192, 96, S_ABOUT_DESC));
	ok = std::unique_ptr<Fl_Button>(
		new Fl_Button(128, 152, 64, 32, S_DONE));
	ok->shortcut(FL_Enter);
	ok->callback(quit_cb, (void*)this);
	desc->labelsize(14);
}

AboutWindow::~AboutWindow()
{}

void AboutWindow::quit_cb(Fl_Widget *, void * wnd)
{
	auto awnd = static_cast<AboutWindow*>(wnd);
	awnd->hide();
}
