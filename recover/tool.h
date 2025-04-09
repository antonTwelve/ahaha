#pragma once
#include <Windows.h>
#include <string>
#include <shlobj.h>
using std::string;

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
