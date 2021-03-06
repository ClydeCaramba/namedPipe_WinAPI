
#include "stdafx.h"
#include <iostream>
#include <string>
#include <Windows.h>

#define PipePath TEXT("\\\\.\\pipe\\NamelessPipe")

void ShowHelp()
{
	std::cout << "This is the client part of kinda dir utility" << std::endl;
	std::cout << "Authors (this time) Sulimov & Tushin." << std::endl;
	exit(-1);
}

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	DWORD valueOfWritten;
	char path[2048];

	if (argc == 2 && !strcmp(argv[1], "-h"))
	{
		ShowHelp();
	}
	else if (argc == 2)
	{
		std::string temp = argv[1];
		temp += "\\*";
		sprintf(path, temp.c_str());
	}
	else if (argc == 1)
	{
		std::string temp = argv[0];
		int dir = temp.find_last_of('\\');
		temp = temp.substr(0, dir);
		temp += "\\*";
		sprintf(path, temp.c_str());
	}
	HANDLE namedPipe;
	namedPipe = CreateFile(PipePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (namedPipe == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Connection failed with code: %d", GetLastError());
		exit(-1);
	}
	
	if (!WriteFile(namedPipe, path, 2048, &valueOfWritten, NULL))
	{
		std::cout << "Server connection error (WriteFile == false)\n";
		fprintf(stderr, "Error code: %d", GetLastError());
		return 0;
	}

	bool isValidDir = true;
	ReadFile(namedPipe, &isValidDir, sizeof(bool), &valueOfWritten, NULL);
	if (!isValidDir)
	{
		fprintf(stderr, "Invalid directory.");
		CloseHandle(namedPipe);
		return 0;
	}
	char result[2048];
	bool i = true;
	do 
	{
		ReadFile(namedPipe, result, 2048, &valueOfWritten, NULL);
		if (strcmp(result,"over"))
			fprintf(stdout, result);
		else
			i = false;
		fprintf(stdout, "\n");
	} while (i);
    return 0;
}

