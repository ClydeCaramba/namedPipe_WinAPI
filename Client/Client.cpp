
#include "stdafx.h"
#include <iostream>
#include <Windows.h>

#define PipePath TEXT("\\\\.\\pipe\\namelessPipe")

void ShowHelp()
{
	std::cout << "This is the server part of kinda dir utility" << std::endl;
	std::cout << "Authors (this time) Sulimov & Tushin." << std::endl;
	system("pause");
}

int main(int args, char* argv[])
{
	system("pause");
	DWORD valueOfWritten;
	std::string path;

	if (args == 2 && !strcmp(argv[1], "-h"))
	{
		ShowHelp();
	}
	else if (args == 2)
	{
		path = argv[1];
	}
	else if (args == 1)
	{
		std::string temp = argv[0];
		int dir = temp.find_last_of('\\');
		path = temp.substr(0, dir);
	}
	path += "\\*";
	HANDLE namedPipe;
	namedPipe = CreateFile(PipePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	WriteFile(namedPipe, path.c_str(), 2048, &valueOfWritten, NULL);
	char* result;
	ReadFile(namedPipe, &result, 2048, &valueOfWritten, NULL);
	fprintf(stdout, result);
	system("pause");
    return 0;
}

