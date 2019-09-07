#include "InstallThread.h"
#include <Shlobj.h>
#include <Lmcons.h>
#include <stdio.h>

InstallThread::InstallThread(InstallationWindow * wnd) :
	parent(wnd), i(0.f)
{}

void InstallThread::operator()() {
	Fl_Progress& progress = *(parent->progress);
	srand(static_cast<unsigned int>(time(0)));
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	createConfFiles();
	incStep(progress);
	if (parent->isCanceled) return;
	Fl::lock();
	parent->finish();
	Fl::unlock();
}

void InstallThread::incStep(Fl_Progress& progress)
{
	i += 0.1f;
	Fl::lock();
	progress.value(i);
	Fl::unlock();
}

void InstallThread::createConfFiles()
{
	TCHAR user_name[UNLEN + 1];
	DWORD user_name_size = UNLEN;
	if (GetUserName(user_name, &user_name_size))
	{
		std::string uname(user_name, user_name_size);
		std::string fullhash = hash(uname);
		PWSTR path = NULL;
		HRESULT hr = SHGetKnownFolderPath(FOLDERID_Profile,
			KF_FLAG_NO_ALIAS, NULL, &path);
		if (SUCCEEDED(hr))
		{
			std::wstring pathw(path);
			static std::string keys[] =
			{
				"A5l2P1aCa9",
				"692ABEB0pP",
				"2C1ja9HRLI",
				"832EDn)UVO",
				"Ev921Akble"
			};
			pathw += L"\\AppData\\Roaming\\MineFLTK\\";
			std::string pathS(pathw.begin(), pathw.end());
			if (CreateDirectory(pathS.c_str(), NULL) ||
				ERROR_ALREADY_EXISTS == GetLastError())
			{
				createSingleConfFile(pathS, "AJ23KL5.bin", fullhash.substr(0, 64), hash(keys[4]).substr(0, 64));
				createSingleConfFile(pathS, "B81024G.bin", fullhash.substr(64, 64), hash(keys[4]).substr(64, 64));
				createSingleConfFile(pathS, "DKJ618K.bin", randomhash(), randomhash());
				createSingleConfFile(pathS, "HJI2196.bin", randomhash(), randomhash());
				createSingleConfFile(pathS, "L127JIS.bin", randomhash(), randomhash());
				createSingleConfFile(pathS, "RORJ2JA.bin", randomhash(), randomhash());
				ExtractResource(101, parent->install_path+"\\mineFLTK.exe", "BINARY");
			}
			else
			{
				fl_alert(u8"Can't create folder!");
			}
			CoTaskMemFree(path);
		}
	}
}

void InstallThread::createSingleConfFile(const std::string& path,
	const std::string& filenameS,
	const std::string& content,
	const std::string& some_hash)
{
	std::ofstream file(path + filenameS,
		std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
	file << content;
	file << some_hash;
	auto chksum = getChecksum(content+some_hash);
	file << chksum;
	file.close();
	SYSTEMTIME thesystemtime;
	GetSystemTime(&thesystemtime);
	thesystemtime.wMinute = 20;
	thesystemtime.wHour = 07;
	thesystemtime.wDay = 07;
	thesystemtime.wMonth = 01;
	thesystemtime.wYear = 2019;
	FILETIME thefiletime, thefiletime1, thefiletime2;
	SystemTimeToFileTime(&thesystemtime, &thefiletime);
	SystemTimeToFileTime(&thesystemtime, &thefiletime1);
	SystemTimeToFileTime(&thesystemtime, &thefiletime2);
	HANDLE filename = CreateFile((path + filenameS).c_str(),
		FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	SetFileTime(filename, &thefiletime, &thefiletime1, &thefiletime2);
	CloseHandle(filename);
}

std::string InstallThread::hash(const std::string& in) {
	size_t i = 0;
	uint32_t hash = 0;
	std::string result;
	while (result.length() != 128) {
		hash += in[(i++) % in.length()];
		hash += hash << 10;
		hash ^= hash >> 6;
		result += static_cast<unsigned char>(hash);
		hash += hash << 3;
		hash ^= hash >> 11;
		hash += hash << 15;
	}
	return result;
}


std::string InstallThread::randomhash() {
	std::string result;
	while (result.length() != 64) {
		result += static_cast<unsigned char>(rand());
	}
	return result;
}

unsigned char InstallThread::getChecksum(const std::string& str) {
	unsigned char result = 0;
	for (size_t i = 0; i < str.length(); i++) {
		result ^= static_cast<unsigned char>(str[i]);
	}
	return result;
}

bool InstallThread::ExtractResource(std::uint16_t ResourceID, std::string OutputFileName, const char* ResType)
{
	try
	{
		HRSRC hResource = FindResource(nullptr, MAKEINTRESOURCE(ResourceID), ResType);
		if (hResource == nullptr)
			return false;
		HGLOBAL hFileResource = LoadResource(nullptr, hResource);
		if (hFileResource == nullptr)
			return false;
		void* lpFile = LockResource(hFileResource);
		if (lpFile == nullptr)
			return false;
		std::uint32_t dwSize = SizeofResource(nullptr, hResource);
		if (dwSize == 0)
			return false;
		HANDLE hFile = CreateFile(OutputFileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		HANDLE hFilemap = CreateFileMapping(hFile, nullptr, PAGE_READWRITE, 0, dwSize, nullptr);
		if (hFilemap == nullptr)
			return false;
		void* lpBaseAddress = MapViewOfFile(hFilemap, FILE_MAP_WRITE, 0, 0, 0);
		CopyMemory(lpBaseAddress, lpFile, dwSize);
		UnmapViewOfFile(lpBaseAddress);
		CloseHandle(hFilemap);
		CloseHandle(hFile);
		return true;
	}
	catch (...) {}
	return false;
}