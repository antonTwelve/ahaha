#pragma once
#include <string>
#include <io.h>
#include <Windows.h>
#include <shlobj.h>
using std::string;
using std::wstring;
//�ж����ļ�����Ŀ¼
//0Ϊ�ļ�
//1ΪĿ¼
//-1Ϊδ֪
inline int fileOrDir(struct _finddata_t fileinfo)
{
	unsigned dir[] = { 16, 18, 17, 9238, 8208, 22 };
	unsigned file[] = { 32, 544, 8224 };
	for (int i = 0; i < sizeof(dir) / sizeof(unsigned); i++)
		if (fileinfo.attrib == dir[i]) //�ļ��� ����
			return 1;
	for (int i = 0; i < sizeof(file) / 4; i++)
		if (fileinfo.attrib == file[i]) //�ļ�����
			return 0;
	return -1;
}

//��ȡ���������·��
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
//FUCK��MICROSOFT

//��ȡ��ǰexe�ļ���·��
string GetExePath()
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	string path = szFilePath;
	return path;
}

//stringתwstring
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
