
#include "stdafx.h"
#include <iostream>
#include <Windows.h>

#define PipePath TEXT("\\\\.\\pipe\\NamelessPipe")

void ShowHelp()
{
	std::cout << "This is the server part of kinda dir utility" << std::endl;
	std::cout << "Authors (this time) Sulimov & Tushin." << std::endl;
	system("pause");
}

int main(int argc, char* argv[])
{
	DWORD valueOfWritten;
	std::string path;

	if (argc == 2 && !strcmp(argv[1], "-h"))
	{
		ShowHelp();
	}
	else if (argc == 2)
	{
		std::string temp(argv[1]);
		path = temp;
	}
	else if (argc == 1)
	{
		std::string temp = argv[0];
		int dir = temp.find_last_of('\\');
		path = temp.substr(0, dir);
	}
	path += "\\*";
	HANDLE namedPipe;
	namedPipe = CreateFile(PipePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	WriteFile(namedPipe, path.c_str(), 2048, &valueOfWritten, NULL);
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
		if (!strcmp(result,"over"))
			fprintf(stdout, result);
		else
			i = false;
		fprintf(stdout, "\n");
	} while (i);
    return 0;
}

