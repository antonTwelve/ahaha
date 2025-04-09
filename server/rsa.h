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

string private_key =
"-----BEGIN RSA PRIVATE KEY-----\n\
MIIEowIBAAKCAQEAxWQqtitWDnvIBSf18I8aFDoZMBjjPoj1I7cJzuiu2boxe8r6\n\
7uSrxDRd4vEQJUDw0spM3mY0uf3uh6EJDj5ek9RfnK7z4LBz9wUMnes/olwFgDxR\n\
ouAMIZKAp8I7zpPo7y2JTE41vF8y5xA7xOO9t5CcLcai8HFopbUDqcdjDECiOmON\n\
T6v2uu106rfkiMLxdWMu14jJBnneY1I77NBKRDZpPriO9I0PfM9VZAC6oAFHrma6\n\
PnC3pDqbuzYfbeIjk9lKoz8zoLUtb+Rz68ncyiilBNIO21pOg32LKWT3XzxRdxAj\n\
zQdlsNf74f+9uULHIiuOYGS9IR7wgc5NMW4sxwIDAQABAoIBAQCLua7+Ssjc8cQR\n\
2kqeoT2Nqd6gjE13u80DHhMkUtNKriFPBs0ABACOn58CATAznreaDrfBCkoyikyp\n\
11Uorv+V11Ri/fkGSQGdjwZyVs8xp8ABGTGUgNI1hZXqqnRZc5wEipBQ4l/pWt4W\n\
cAhPBAMMiKH1SVGXsD+qZOsV6Fz1kx/87BjVKJ5Yt0qwShdz6uqP6abqvlwlvIeK\n\
MKVQLrTPtnCgkdWFfzupdHRUHXP92kX1ugl9Hl1HR4awC7WLhkn5iX6FrBGAGx9u\n\
ZshP+TViLlC4GFiBvcvLPUWEFN3Dkx3h/nD4qfs4SpiCZr/fAtC38xPlf/GMGQcm\n\
6TnFJ8iBAoGBAPEH0Oer5W/NSX34W3qcAQ93cvEAQqtOcNtXpVSXqBDENYXJy3iL\n\
S3Krau/jfLnuptH9U9m9zVJtUedy9jxP5pwTMwbujRWSWoAMAfbDVcGz8pSZlpEq\n\
OWlujWUoA75uRFoa9lpXZlqa7339O3U1KGXeUxT8qqwE0V8+vaTrc99BAoGBANGm\n\
hexT8fKnqMTTgmL+aIy5U/3TiVXSsinGUJGHhGLOyuPTkAggwKd9+KFtgDBf9PC9\n\
L7skhZr2/dFFtBPWReLD7Rd4WvRojZeAbJu5rRMr3eBlcX1ni7ASm+bYq0jXtkNA\n\
o8tq/AkPEQT0Tgvh0+mfsYmuk/1/jVuUhjif8JIHAoGAGpRNEi8fhvuxXgXUOKoZ\n\
Z9Y5Q6BOuctVoKqCndh3fRnV+SHuoSVjNYr3qOqCJ5LMrJ1t8LYAfVV20boYsz/w\n\
Y0euudX2kulnt3UbhlW2ED1Cw+ZH90wbtqetSflmqEpKmVEd14qJy/YH8PHK67rn\n\
8Ivax5axT3Tcjc9CZHxec8ECgYB0BE36UR4EJWnMuitecuESdwCW53gJiVZUrc0q\n\
K69EKqG2X/5FsTJLvfdYBcu5eXCHxHnkezqAXgxpwcP8ckgsTDYpFc5nkHBClawE\n\
pUS05kKbiie6uRFhN3sUzXREFDNlliDOUf71WGQXtMZ35tmpYFkUEwsMboigVFnO\n\
fhvYAwKBgB+wQ+tV+GiFSKyyQIn0BoBqSdQLfZmV+qRbnnHNGn8Rfv4RAOqMheYr\n\
gUz9o7XSYwPzTqsCYpa6u5mrepdNcSaLIgEPrl0ZJgfHjvVJACCi9awZ7DklX88k\n\
T+TsWJoYJE3Vtqv4wFBTW0sSJBea3Qh+TimRdwVyIF5SD02vA4/V\n\
-----END RSA PRIVATE KEY-----\n";

void RsaDe(char* input, char* output, int length)
{
	BIO* bio = BIO_new_mem_buf((void*)private_key.c_str(), -1);
	RSA* rsa = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);
	BIO_free(bio);
	RSA_private_decrypt(length, (unsigned char*)input, (unsigned char*)output, rsa, RSA_PKCS1_PADDING);
	RSA_free(rsa);
}