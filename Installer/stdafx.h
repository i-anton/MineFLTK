#pragma once
#define WIN32
#include <memory>
#include <iostream>
#include <fstream>
#include <cassert>
#include <ctime>
#include <string>
#include <thread>
#include <chrono>
#include <FL/Fl.h>
#include <FL/fl_ask.H>
#include <FL/Fl_Window.h>
#include <FL/Fl_Box.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Button.h>
#include <FL/Fl_Output.H>
#include <FL/Fl_Native_File_Chooser.H>

#include "strings.h"

#define MIN(x,y) ((x)>(y))?(y):(x)
#define MAX(x,y) ((x)>(y))?(x):(y)

constexpr unsigned int window_width = 420;
constexpr unsigned int window_height = window_width / 2;
constexpr unsigned int button_height = 32;
constexpr unsigned int button_width = 64;
constexpr unsigned int gui_offset = 8;