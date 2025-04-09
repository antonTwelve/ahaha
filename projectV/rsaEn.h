#pragma once
#include <string>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/ssl.h>
using std::string;

string public_key =
"-----BEGIN PUBLIC KEY-----\n\
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAxWQqtitWDnvIBSf18I8a\n\
FDoZMBjjPoj1I7cJzuiu2boxe8r67uSrxDRd4vEQJUDw0spM3mY0uf3uh6EJDj5e\n\
k9RfnK7z4LBz9wUMnes/olwFgDxRouAMIZKAp8I7zpPo7y2JTE41vF8y5xA7xOO9\n\
t5CcLcai8HFopbUDqcdjDECiOmONT6v2uu106rfkiMLxdWMu14jJBnneY1I77NBK\n\
RDZpPriO9I0PfM9VZAC6oAFHrma6PnC3pDqbuzYfbeIjk9lKoz8zoLUtb+Rz68nc\n\
yiilBNIO21pOg32LKWT3XzxRdxAjzQdlsNf74f+9uULHIiuOYGS9IR7wgc5NMW4s\n\
xwIDAQAB\n\
-----END PUBLIC KEY-----\n";

void RsaEn(char* input, char* output, int length)
{
    BIO* bio = BIO_new_mem_buf((void*)public_key.c_str(), -1);
    RSA* rsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);
    BIO_free(bio);
    RSA_public_encrypt(length, (unsigned char*)input, (unsigned char*)output, rsa, RSA_PKCS1_PADDING);
    RSA_free(rsa);
}