#pragma once
#include <string>
#include <openssl/aes.h>
#include "tool.h"
using std::string;
unsigned char aesKey[32] = { (char)0x34, (char)0x40, (char)0x9d, (char)0xfa, (char)0xb8, (char)0xe6,
					(char)0x3a, (char)0xc, (char)0x43, (char)0x38, (char)0x66, (char)0x87,
					(char)0xf0, (char)0xdc, (char)0x8c, (char)0xfa, (char)0xb8, (char)0xe6,
					(char)0xf0, (char)0xdc, (char)0x8c, (char)0xe2, (char)0xe2, (char)0xc,
					(char)0x8a, (char)0x24, (char)0x19, (char)0x54, (char)0x98, (char)0x4,
					(char)0x68, (char)0xa9 }; //AES密钥, 本版本中只用前16字节
unsigned char aesIv[16] = { (char)0x21, (char)0x56, (char)0xbd, (char)0x55, (char)0x67,
					(char)0x89, (char)0xe3, (char)0x45,	(char)0xf0, (char)0xde, (char)0x79,
					(char)0x56, (char)0x78, (char)0x43, (char)0x78, (char)0x23 };	//初始向量
char aesKeyLen = 16;                       //密钥长度
AES_KEY AesKey;
string DPath = getSpecialPath(CSIDL_DESKTOP) + "\\files.txt";
string KeyPath = getSpecialPath(CSIDL_DESKTOP) + "\\KEY";