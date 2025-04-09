#pragma once
#include <string>
#include <fstream>
#include "aes.h"
using std::ifstream;
using std::string;
using std::ios;
using std::fstream;

void deFileEncryptFunc(string FilePath)
{
	ifstream f_in(FilePath.c_str(), ios::in | ios::binary);
	char input_buf[1024];
	char output_buf[1024];
	unsigned long long FileSize = 0; //文件大小
	int temp = 0;

	f_in.seekg(0, ios::end);
	FileSize = f_in.tellg(); //获取大小
	f_in.seekg(0, ios::beg);
	if (FileSize < 1024 && FileSize >= 16)
	{
		temp = FileSize / 16;
		temp = temp * 16;
		f_in.read(input_buf, temp);
		AesDe((unsigned char*)input_buf, (unsigned char*)output_buf, temp);
	}
	else if (FileSize >= 1024)
	{
		f_in.read(input_buf, 1024);
		AesDe((unsigned char*)input_buf, (unsigned char*)output_buf, 1024);
		temp = 1024;
	}
	else //跳过16字节以下(128位以下)的文件
	{
		return;
	}
	f_in.close();
	fstream f_out(FilePath.c_str(), ios::out | ios::in | ios::binary);
	f_out.write(output_buf, temp);
	f_out.close();
}

void recover()
{
	string filePath;
	ifstream input(DPath.c_str(), ios::in);
	while (input.peek() != EOF)
	{
		getline(input, filePath);
		//printf("%s\n", filePath.c_str());
		deFileEncryptFunc(filePath);
	}
	input.close();
}
