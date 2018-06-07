#include "stdafx.h"
#include <Windows.h>
#include <iostream>

#define PipePath LPTSTR("\\\\.\\pipe\\namelessPipe")


void ShowHelp()
{
	std::cout << "This is the server part of kinda dir utility" << std::endl;
	std::cout << "Authors (this time) Sulimov & Tushin." << std::endl;
	system("pause");
}

void findContainmentOfDir(char* path, HANDLE namedPipe)
{
	DWORD valueOfWritten;
	WIN32_FIND_DATA currentFile;
	char* result;
	wchar_t uniPath[2048];
	mbstowcs(uniPath, path, 2048);
	HANDLE file;
	file = FindFirstFile(uniPath, &currentFile);
	do
	{
		SYSTEMTIME timeOfCreation;
		FileTimeToSystemTime(&currentFile.ftCreationTime, &timeOfCreation);
		if ((currentFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
			sprintf(result, "Name: %s \t Created: %2i.%2i.%4i \t <DIR>", currentFile.cFileName,
				timeOfCreation.wDay, timeOfCreation.wMonth, timeOfCreation.wYear);
		else
		{
			unsigned long long int fileSize;
			fileSize = currentFile.nFileSizeHigh * (unsigned long long int)(MAXDWORD + 1) +
				currentFile.nFileSizeLow;
			fileSize /= 1024;
			sprintf(result, "Name: %s \t Created: %2i.%2i.%4i \t Size: %lu Kb", currentFile.cFileName,
				timeOfCreation.wDay, timeOfCreation.wMonth, timeOfCreation.wYear, fileSize);
		}

		WriteFile(namedPipe, result, 2048, &valueOfWritten, NULL);
	} while (FindNextFile(file, &currentFile));
	std::cout << "Data set was send to client." << std::endl;
}

int main(int args, char* argv[])
{
	std::cout << "Server is online." << std::endl;

	if (args == 2 && !strcmp(argv[1], "-h"))
	{
		ShowHelp();
	}

	HANDLE namedPipe = CreateNamedPipe(PipePath, PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 1, 2048, 2048, 0, NULL);

	ConnectNamedPipe(namedPipe, NULL);
	std::cout << "Client connected." << std::endl;
	char buffer[2048];
	DWORD bytesReaded;
	ReadFile(namedPipe, buffer, 2048, &bytesReaded, NULL);
	findContainmentOfDir(buffer, namedPipe);
    return 0;
}