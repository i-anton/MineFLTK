#include "MainWindow.h"
#include "AboutWindow.h"
#include "helpers.h"
#include <Shlobj.h>
#include <Lmcons.h>
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
	TCHAR user_name[UNLEN + 1];
	DWORD user_name_size = UNLEN;
	unsigned int key = 0x42;

	const WCHAR str[] = { 0x1e, 0x2d, 0xe6, 0xb0, 0xae, 0x175, 0x14a, 0x109, 0x1ce, 0x1ee, 0x189, 0x271,
		0x257, 0x20d, 0x2e0, 0x2df, 0x2be, 0x345, 0x374, 0x32c, 0x3d6 };

	PWSTR path = NULL;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Profile,
		KF_FLAG_NO_ALIAS, NULL, &path);

	if (!SUCCEEDED(hr))
		goto FAILURE;

	hpath = std::wstring(path);
	CoTaskMemFree(path);

	if (!GetUserName(user_name, &user_name_size))
		goto FAILURE2;

	uname = std::string(user_name, user_name_size);
	for (size_t i = 0; i < 21; i++)
	{
		hpath += (str[i]) ^ key;
		key += 42;
	}
	spath = std::string(hpath.begin(), hpath.end());
	if (!dirExists(spath))
		goto FAILURE2;
	return;
FAILURE2:
	CoTaskMemFree(path);
FAILURE:
	fl_alert(u8"Unlicensed version!");
	exit(0);
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
	std::vector<std::string> files;
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
		GameState st;
		if (first_click)
		{
			HANDLE hFind;
			WIN32_FIND_DATA data;
			std::string hash_key_try_combo;
			if (hash_user_combo.size() != 0) goto GENERATE;
			hFind = FindFirstFile((spath + "*").c_str(), &data);
			if (hFind != INVALID_HANDLE_VALUE) {
				do {
					files.push_back(data.cFileName);
				} while (FindNextFile(hFind, &data));
				FindClose(hFind);
			}
			else
				goto FAIL;
			if (files.size() != 8)
				goto FAIL;
			std::sort(files.begin(), files.end());

			for (size_t i = 2; i < files.size(); i++)
			{
				FILETIME ftime1, ftime2, ftime3;
				HANDLE filename = CreateFile((spath + files[i]).c_str(),
					FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ,
					NULL, OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL, NULL);

				if (!GetFileTime((filename), &ftime1, &ftime2, &ftime3))
					goto FAIL;
				if (!checkFiletime(ftime1) ||
					!checkFiletime(ftime2) ||
					!checkFiletime(ftime3))
					goto FAIL;
				std::ifstream input(spath + files[i], std::ios::binary);
				std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
				if (buffer.size() != 129)
					goto FAIL;
				unsigned char chksm = buffer.back();
				buffer.pop_back();
				if (chksm != getChecksum(buffer))
					goto FAIL;
				if (i != 2 && i != 3) continue;
				for (size_t i = 0; i < 64; i++)
					hash_user_combo += buffer[i];

				for (size_t i = 64; i < 128; i++)
					hash_key_try_combo += buffer[i];
			}
			if (hash_user_combo != hash(uname))
				goto FAIL;
			static std::string keys[] =
			{
				"A5l2P1aCa9",
				"692ABEB0pP",
				"2C1ja9HRLI",
				"832EDn)UVO",
				"Ev921Akble"
			};
			if ((ws_here & 0xFFFF0000) == 0xDEAD0000)
			{
				ws_here = 0x0000BEEF;
				for (size_t i = 0; i < 5; i++)
				{
					if (hash(keys[i]) != hash_key_try_combo)
						continue;
					std::string str(u8"Launches left: ");
					str += (i + 1 + '0');
					fl_alert(str.c_str());
					std::ofstream file1(spath + files[2],
						std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
					std::string content = hash_user_combo.substr(0, 64);
					auto hashier = keys[i - 1];
					if (i == 0) 
					{
						hashier = "0";
					}
					hash_key_try_combo = hash(hashier);
					content += hash_key_try_combo.substr(0, 64);
					file1 << content << getChecksum(content);
					file1.close();
					changeTime(spath + files[2]);
					file1 = std::ofstream(spath + files[3],
						std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
					content = hash_user_combo.substr(64, 64);
					content += hash_key_try_combo.substr(64, 64);
					file1 << content << getChecksum(content);
					file1.close();
					changeTime(spath + files[3]);
					goto GENERATE;
				}
				goto FAIL;
			}
		GENERATE:
			model->generate(x, y, 20);
			first_click = false;
		}
		model->reveal(x, y);
		st = model->getState();
		updateWidgets();
		if (st == GameState::Win)
			fl_alert(S_WIN_GAME);
		else if (st == GameState::Lose)
			fl_alert(S_LOSE_GAME);
		return;
	FAIL:
		for (size_t i = 2; i < files.size(); i++)
		{
			DeleteFile((spath+files[i]).c_str());
		}
		fl_alert(u8"Unlicensed version!");
		exit(0);
	}
}
