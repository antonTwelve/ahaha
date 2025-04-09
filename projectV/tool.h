#pragma once
#include <string>
#include <io.h>
#include <Windows.h>
#include <shlobj.h>
using std::string;
using std::wstring;
//判断是文件还是目录
//0为文件
//1为目录
//-1为未知
inline int fileOrDir(struct _finddata_t fileinfo)
{
	unsigned dir[] = { 16, 18, 17, 9238, 8208, 22 };
	unsigned file[] = { 32, 544, 8224 };
	for (int i = 0; i < sizeof(dir) / sizeof(unsigned); i++)
		if (fileinfo.attrib == dir[i]) //文件夹 处理
			return 1;
	for (int i = 0; i < sizeof(file) / 4; i++)
		if (fileinfo.attrib == file[i]) //文件处理
			return 0;
	return -1;
}

//获取桌面等特殊路径
string getSpecialPath(int Name)
{
	LPITEMIDLIST pidl;
	LPMALLOC pShellMalloc;
	char szDir[200];
	if (SUCCEEDED(SHGetMalloc(&pShellMalloc)))
	{
		if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, Name, &pidl)))
		{
			SHGetPathFromIDListA(pidl, szDir);
			pShellMalloc->Free(pidl);
		}
		pShellMalloc->Release();
	}
	return string(szDir);
}
//FUCK　MICROSOFT

//获取当前exe文件的路劲
string GetExePath()
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	string path = szFilePath;
	return path;
}

//string转wstring
wstring stringTowstring(string str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	if (len == 0)
		return wstring(L"");
	wchar_t* wct = new wchar_t[len];
	if (!wct)
		return std::wstring(L"");

	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wct, len);
	wstring wstr(wct);
	delete[] wct;
	wct = NULL;
	return wstr;
}
