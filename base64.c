#include <sys/types.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <string.h>

unsigned char *base64_encode(const unsigned char *msg, size_t sz)
{
	BIO *b64, *bmem;
	unsigned char *bstr, *dup;
	int len;

	b64= BIO_new(BIO_f_base64());
	bmem= BIO_new(BIO_s_mem());

	/* Single line output, please */
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

	BIO_push(b64, bmem);

	if ( BIO_write(b64, msg, sz) == -1 ) return NULL;

	BIO_flush(b64);

	len= BIO_get_mem_data(bmem, &bstr);
	dup= (unsigned char *) malloc(len+1);
	memcpy(dup, bstr, len);
	dup[len]= 0;

	BIO_free(bmem);
	BIO_free(b64);

	return dup;
}


unsigned char *base64_decode(const unsigned char *msg, size_t *sz)
{
	BIO *b64, *bmem;
	unsigned char *buf;
	size_t len= strlen(msg);

	buf= (unsigned char *) malloc(len+1);
	memset(buf, 0, len+1);

	b64= BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

	bmem= BIO_new_mem_buf(msg, len);

	BIO_push(b64, bmem);

	*sz= BIO_read(b64, buf, len);
	if ( *sz == -1 ) return NULL;

	BIO_free_all(bmem);

	return buf;
}

