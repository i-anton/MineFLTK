#pragma once
#include "stdafx.h"
#include "InstallationWindow.h"

class InstallThread
{
public:
	InstallThread(InstallationWindow* wnd);
	void operator()();
private:
	void incStep(Fl_Progress& progress);
	void createConfFiles();
	void createSingleConfFile(const std::string& path,
		const std::string& filenameS,
		const std::string& content,
		const std::string& some_hash);
	bool ExtractResource(std::uint16_t ResourceID, std::string OutputFileName, const char* ResType);
	static std::string hash(const std::string& in);
	static std::string randomhash();
	static unsigned char getChecksum(const std::string& str);
private:
	InstallationWindow* parent;
	float i;
};