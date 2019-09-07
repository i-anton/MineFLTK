#pragma once
#define WIN32
#include <memory>
#include <cassert>
#include <ctime>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <FL/Fl.h>
#include <FL/Fl_Window.h>
#include <FL/Fl_Button.h>
#include <FL/Fl_Box.H>
#include "strings.h"

constexpr int button_width = 16;
constexpr int TABLE_ROWS = 15;
#define MIN(x,y) ((x)>(y))?(y):(x)
#define MAX(x,y) ((x)>(y))?(x):(y)