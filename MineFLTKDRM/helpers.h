#pragma once
#include "stdafx.h"

inline bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

inline std::string hash(const std::string& in) {
	size_t i = 0;
	uint32_t hash = 0;
	std::string result;
	while (result.length() != 128) {
		hash += in[(i++) % in.length()];
		hash += hash << 10;
		hash ^= hash >> 6;
		result += static_cast<char>(hash);
		hash += hash << 3;
		hash ^= hash >> 11;
		hash += hash << 15;
	}
	return result;
}

inline unsigned char getChecksum(const std::vector<unsigned char>& str) {
	unsigned char result = 0;
	for (size_t i = 0; i < str.size(); i++) {
		result ^= static_cast<unsigned char>(str[i]);
	}
	return result;
}

inline unsigned char getChecksum(const std::string& str) {
	unsigned char result = 0;
	for (size_t i = 0; i < str.length(); i++) {
		result ^= static_cast<unsigned char>(str[i]);
	}
	return result;
}
bool checkFiletime(const FILETIME file)
{
	SYSTEMTIME cmparable;
	FileTimeToSystemTime(&file, &cmparable);
	if (cmparable.wMinute != 20) {
		std::cout << "m" << cmparable.wMinute;
		return false;
	}
	if (cmparable.wHour != 07) {
		std::cout << "h" << cmparable.wHour;
		return false;
	}
	if (cmparable.wDay != 07) {
		std::cout << "day" << cmparable.wDay;
		return false;
	}
	if (cmparable.wMonth != 01) {
		std::cout << "mon" << cmparable.wMonth;
		return false;
	}
	if (cmparable.wYear != 2019) {
		std::cout << "y" << cmparable.wYear;
		return false;
	}
	return true;
}

void changeTime(const std::string& path) 
{
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
	HANDLE filename = CreateFile((path).c_str(),
		FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	SetFileTime(filename, &thefiletime, &thefiletime1, &thefiletime2);
	CloseHandle(filename);
}