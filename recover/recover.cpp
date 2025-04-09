#include <string>
#include <stdio.h>
#include <fstream>
#include <shlobj.h>
#include <windows.h>
using namespace std;

#include "var.h"
#include "R.h"

int KeyRead()
{
    char Buf[49];
    ifstream KeyInput(KeyPath.c_str(), ios::in | ios::binary);
    if (!KeyInput)
        return -1;
    KeyInput.read(Buf, 49);
    KeyInput.close();
    aesKeyLen = Buf[0];
    for (int i = 0; i < 32; i++)
        aesKey[i] = Buf[i + 1];
    for (int i = 0; i < 16; i++)
        aesIv[i] = Buf[i + 33];
    return 1;
}

int main()
{
    if (KeyRead() != 1)
    {
        printf("key error!\n");
        system("pause"); //调试, 暂停程序,便于观察现象
        return 0;
    }
    AES_set_decrypt_key(aesKey, aesKeyLen * 8, &AesKey);
    recover();
    system("pause"); //调试, 暂停程序,便于观察现象
    return 0;
}