#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <signal.h>

#define PipePath TEXT("\\\\.\\pipe\\namelessPipe")

void ShowHelp()
{
	std::cout << "This is the server part of kinda dir utility" << std::endl;
	std::cout << "Authors (this time) Sulimov & Tushin." << std::endl;
}


void findContainmentOfDir(char* path, HANDLE namedPipe)
{
	DWORD valueOfWritten;
	WIN32_FIND_DATA currentFile;
	char result[2048];
	wchar_t uniPath[2048];
	mbstowcs(uniPath, path, 2048);
	HANDLE file;
	file = FindFirstFile(uniPath, &currentFile);
	bool isValidDir = true;
	if (file == INVALID_HANDLE_VALUE)
	{
		isValidDir = false;
		WriteFile(namedPipe, &isValidDir, sizeof(bool), &valueOfWritten, NULL);
		exit(-1);
	}
	WriteFile(namedPipe, &isValidDir, sizeof(bool), &valueOfWritten, NULL);
	do
	{
		SYSTEMTIME timeOfCreation;
		FileTimeToSystemTime(&currentFile.ftCreationTime, &timeOfCreation);
		if ((currentFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
			sprintf(result, "Name: %ws \t Created: %d/%d/%d \t <DIR>", currentFile.cFileName,
				timeOfCreation.wDay, timeOfCreation.wMonth, timeOfCreation.wYear);
		else
		{
			unsigned long long int fileSize;
			fileSize = currentFile.nFileSizeHigh * (unsigned long long int)(MAXDWORD + 1) +
				currentFile.nFileSizeLow;
			fileSize /= 1024;
			sprintf(result, "Name: %ws \t Created: %d/%d/%d \t Size: %lu Kb", currentFile.cFileName,
				timeOfCreation.wDay, timeOfCreation.wMonth, timeOfCreation.wYear, fileSize);
		}

		WriteFile(namedPipe, result, 2048, &valueOfWritten, NULL);
	} while (FindNextFile(file, &currentFile));
	WriteFile(namedPipe, "over", 2048, &valueOfWritten, NULL);
	std::cout << "Data set was send to client." << std::endl;
}

int main(int args, char* argv[])
{
	SetConsoleOutputCP(1251);
	std::cout << "Server is online." << std::endl;

	if (args == 2 && !strcmp(argv[1], "-h"))
	{
		ShowHelp();
	}
	bool i = true;
	do 
	{
		HANDLE namedPipe = CreateNamedPipe(PipePath, PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 1, 2048, 2048, 50000, NULL);


		bool isConnected;
		isConnected = ConnectNamedPipe(namedPipe, NULL);
		if (!isConnected)
		{
			int errorCode = GetLastError();
			fprintf(stderr, "Connection failed with code: %d", errorCode);
			CloseHandle(namedPipe);
			break;
		}
		else
			std::cout << "Client connected." << std::endl;
		char buffer[2048];
		DWORD bytesReaded;
		ReadFile(namedPipe, buffer, 2048, &bytesReaded, NULL);
		findContainmentOfDir(buffer, namedPipe);
		char symb = _getch();
		if (symb == 'q' || symb == 'Q')
			i = false;
	} while (i);
	
    return 0;
}