#pragma once
#include <openssl/aes.h>
#include "var.h"

void AesEn(unsigned char* input, unsigned char* output, int len)
{
	unsigned char tmpiv[16];
	memcpy(tmpiv, aesIv, 16);
	AES_cbc_encrypt(input, output, len, &AesKey, tmpiv, AES_ENCRYPT);
}