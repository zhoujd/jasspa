/* -*- C -*- ****************************************************************
 *
 * Copyright (c) 2020 Maxinity Software Ltd (www.maxinity.co.uk).
 * 
 * All Rights Reserved.
 * 
 * This  document  may  not, in  whole  or in  part, be  copied,  photocopied,
 * reproduced,  translated,  or  reduced to any  electronic  medium or machine
 * readable form without prior written consent from Maxinity Software Ltd.
 *
 ****************************************************************************/

#define meSOCK_DEBUG 1
#define meSOCK_USE_DLL 1
#define meSOCKBUFSIZ 4096
#if meSOCK_USE_DLL
#define MEHTTPFunc(f) sslF_##f
#else
#define MEHTTPFunc(f) f
#endif
#define MEHTTP_STRINGQUT(str) #str
#define MEHTTP_STRINGIFY(str) MEHTTP_STRINGQUT(str)

#include "mesock.h"
#include <time.h>
#include <ctype.h>
#if MEOPT_OPENSSL
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/conf.h>
#include <openssl/engine.h>
#endif

#ifdef _WIN32
#define meBadSocket        INVALID_SOCKET
#define meSockLibGetFunc   GetProcAddress
#define meSockGetError     GetLastError
#define meSocketGetError   WSAGetLastError
#define meSocketOpen       socket
#define meSocketConnect    connect
#define meSocketClose      closesocket
#define meSocketSetOpt     setsockopt
#define meSocketRead       recv
#define meSocketWrite      send
#define meSocketShutdown   shutdown
#define meGetservbyname    getservbyname
#define meGethostbyname    gethostbyname
#define meInet_addr        inet_addr
#define meHtons            htons
#define snprintf           sprintf_s          
#else
#include <errno.h>
#include <unistd.h>
#include <dlfcn.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/un.h>
#define meBadSocket        -1
#define meSockLibGetFunc   dlsym
#define meSockGetError()   (errno)
#define meSocketGetError() (errno)
#define meSocketOpen       socket
#define meSocketConnect    connect
#define meSocketClose      close
#define meSocketSetOpt     setsockopt
#define meSocketRead       recv
#define meSocketWrite      send
#define meSocketShutdown   shutdown
#define meGetservbyname    getservbyname
#define meGethostbyname    gethostbyname
#define meInet_addr        inet_addr
#define meHtons            htons
#endif

#if MEOPT_OPENSSL

#if meSOCK_USE_DLL

typedef BIO *(*meSOCKF_BIO_new)(const BIO_METHOD *type);
typedef int (*meSOCKF_BIO_free)(BIO *a);
typedef uint64_t (*meSOCKF_BIO_number_written)(BIO *bio);
typedef int (*meSOCKF_BIO_read)(BIO *b, void *data, int dlen);
typedef const BIO_METHOD *(*meSOCKF_BIO_s_mem)(void);
typedef void (*meSOCKF_CRYPTO_free)(void *ptr, const char *file, int line);
typedef void (*meSOCKF_ERR_clear_error)(void);
typedef char *(*meSOCKF_ERR_error_string)(unsigned long e, char *buf);
typedef unsigned long (*meSOCKF_ERR_get_error)(void);
typedef int (*meSOCKF_OPENSSL_sk_num)(const OPENSSL_STACK *);
typedef void *(*meSOCKF_OPENSSL_sk_value)(const OPENSSL_STACK *, int);
typedef void (*meSOCKF_OPENSSL_sk_pop_free)(OPENSSL_STACK *st, void (*func) (void *));
typedef const char *(*meSOCKF_RAND_file_name)(char *file, size_t num);
typedef int (*meSOCKF_RAND_load_file)(const char *file, long max_bytes);
typedef int (*meSOCKF_RAND_status)(void);
typedef const char *(*meSOCKF_SSL_CIPHER_get_name)(const SSL_CIPHER *c);
typedef long (*meSOCKF_SSL_CTX_ctrl)(SSL_CTX *ctx, int cmd, long larg, void *parg);
typedef void (*meSOCKF_SSL_CTX_free)(SSL_CTX *);
typedef X509_STORE *(*meSOCKF_SSL_CTX_get_cert_store)(const SSL_CTX *);
typedef int (*meSOCKF_SSL_CTX_load_verify_locations)(SSL_CTX *ctx, const char *CAfile, const char *CApath);
typedef SSL_CTX *(*meSOCKF_SSL_CTX_new)(const SSL_METHOD *meth);
typedef int (*meSOCKF_SSL_CTX_set1_param)(SSL_CTX *ctx, X509_VERIFY_PARAM *vpm);
typedef int (*meSOCKF_SSL_CTX_set_default_verify_paths)(SSL_CTX *ctx);
typedef void (*meSOCKF_SSL_CTX_set_verify)(SSL_CTX *ctx, int mode, SSL_verify_cb callback);
typedef int (*meSOCKF_SSL_connect)(SSL *ssl);
typedef long (*meSOCKF_SSL_ctrl)(SSL *ssl, int cmd, long larg, void *parg);
typedef void (*meSOCKF_SSL_free)(SSL *ssl);
typedef const SSL_CIPHER *(*meSOCKF_SSL_get_current_cipher)(const SSL *s);
typedef int (*meSOCKF_SSL_get_error)(const SSL *s, int ret_code);
typedef X509 *(*meSOCKF_SSL_get_peer_certificate)(const SSL *s);
typedef long (*meSOCKF_SSL_get_verify_result)(const SSL *ssl);
typedef const char *(*meSOCKF_SSL_get_version)(const SSL *s);
typedef SSL *(*meSOCKF_SSL_new)(SSL_CTX *ctx);
typedef int (*meSOCKF_SSL_read)(SSL *ssl, void *buf, int num);
typedef int (*meSOCKF_SSL_set_fd)(SSL *s, int fd);
typedef int (*meSOCKF_SSL_shutdown)(SSL *s);
typedef int (*meSOCKF_SSL_write)(SSL *ssl, const void *buf, int num);
typedef const SSL_METHOD *(*meSOCKF_TLS_client_method)(void);
typedef int (*meSOCKF_X509_NAME_print_ex)(BIO *out, const X509_NAME *nm, int indent, unsigned long flags);
typedef int (*meSOCKF_X509_STORE_add_cert)(X509_STORE *ctx, X509 *x);
typedef void (*meSOCKF_X509_VERIFY_PARAM_free)(X509_VERIFY_PARAM *param);
typedef X509_VERIFY_PARAM *(*meSOCKF_X509_VERIFY_PARAM_new)(void);
typedef int (*meSOCKF_X509_VERIFY_PARAM_set_flags)(X509_VERIFY_PARAM *param, unsigned long flags);
typedef void *(*meSOCKF_X509_get_ext_d2i)(const X509 *x, int nid, int *crit, int *idx);
typedef X509_NAME *(*meSOCKF_X509_get_issuer_name)(const X509 *a);
typedef X509_NAME *(*meSOCKF_X509_get_subject_name)(const X509 *a);
typedef const char *(*meSOCKF_X509_verify_cert_error_string)(long n);
typedef ASN1_OCTET_STRING *(*meSOCKF_a2i_IPADDRESS)(const char *ipasc);
typedef ASN1_STRING *(*meSOCKF_X509_NAME_ENTRY_get_data)(const X509_NAME_ENTRY *ne);
typedef X509_NAME_ENTRY *(*meSOCKF_X509_NAME_get_entry)(const X509_NAME *name, int loc);
typedef int (*meSOCKF_X509_NAME_get_index_by_NID)(X509_NAME *name, int nid, int lastpos);
typedef int (*meSOCKF_ASN1_STRING_length)(const ASN1_STRING *x);
typedef char *(*meSOCKF_X509_NAME_oneline)(const X509_NAME *a, char *buf, int size);
typedef int (*meSOCKF_X509_NAME_print_ex_fp)(FILE *fp, const X509_NAME *nm, int indent, unsigned long flags);
typedef int (*meSOCKF_ASN1_STRING_cmp)(const ASN1_STRING *a, const ASN1_STRING *b);
typedef int (*meSOCKF_ASN1_STRING_to_UTF8)(unsigned char **out, const ASN1_STRING *in);
typedef int (*meSOCKF_CONF_modules_load_file)(const char *filename, const char *appname, unsigned long flags);
typedef void (*meSOCKF_ENGINE_load_builtin_engines)(void);
typedef int (*meSOCKF_OPENSSL_init_ssl)(uint64_t opts, const OPENSSL_INIT_SETTINGS *settings);
typedef void (*meSOCKF_OPENSSL_load_builtin_modules)(void);
typedef int (*meSOCKF_RAND_poll)(void);
typedef const SSL_METHOD *(*meSOCKF_TLS_client_method)(void);
typedef long (*meSOCKF_SSL_get_verify_result)(const SSL *ssl);
typedef void (*meSOCKF_X509_free)(X509 *a);
typedef void (*meSOCKF_GENERAL_NAME_free)(GENERAL_NAME *a);
typedef void (*meSOCKF_CRYPTO_free)(void *ptr, const char *file, int line);
typedef void (*meSOCKF_ASN1_OCTET_STRING_free)(ASN1_OCTET_STRING *a);
typedef X509 *(*meSOCKF_d2i_X509)(X509 **a, const unsigned char **in, long len);
typedef int (*meSOCKF_X509_NAME_get_text_by_NID)(X509_NAME *name, int nid, char *buf, int len);

meSOCKF_ASN1_OCTET_STRING_free sslF_ASN1_OCTET_STRING_free;
meSOCKF_ASN1_STRING_cmp sslF_ASN1_STRING_cmp;
meSOCKF_ASN1_STRING_length sslF_ASN1_STRING_length;
meSOCKF_ASN1_STRING_to_UTF8 sslF_ASN1_STRING_to_UTF8;
meSOCKF_BIO_free sslF_BIO_free;
meSOCKF_BIO_new sslF_BIO_new;
meSOCKF_BIO_number_written sslF_BIO_number_written;
meSOCKF_BIO_read sslF_BIO_read;
meSOCKF_BIO_s_mem sslF_BIO_s_mem;
meSOCKF_CONF_modules_load_file sslF_CONF_modules_load_file;
meSOCKF_CRYPTO_free sslF_CRYPTO_free;
meSOCKF_CRYPTO_free sslF_CRYPTO_free;
meSOCKF_ENGINE_load_builtin_engines sslF_ENGINE_load_builtin_engines;
meSOCKF_ERR_clear_error sslF_ERR_clear_error;
meSOCKF_ERR_error_string sslF_ERR_error_string;
meSOCKF_ERR_get_error sslF_ERR_get_error;
meSOCKF_GENERAL_NAME_free sslF_GENERAL_NAME_free;
meSOCKF_OPENSSL_init_ssl sslF_OPENSSL_init_ssl;
meSOCKF_OPENSSL_load_builtin_modules sslF_OPENSSL_load_builtin_modules;
meSOCKF_OPENSSL_sk_num sslF_OPENSSL_sk_num;
meSOCKF_OPENSSL_sk_value sslF_OPENSSL_sk_value;
meSOCKF_OPENSSL_sk_pop_free sslF_OPENSSL_sk_pop_free;
meSOCKF_RAND_file_name sslF_RAND_file_name;
meSOCKF_RAND_load_file sslF_RAND_load_file;
meSOCKF_RAND_poll sslF_RAND_poll;
meSOCKF_RAND_status sslF_RAND_status;
meSOCKF_SSL_CIPHER_get_name sslF_SSL_CIPHER_get_name;
meSOCKF_SSL_CTX_ctrl sslF_SSL_CTX_ctrl;
meSOCKF_SSL_CTX_free sslF_SSL_CTX_free;
meSOCKF_SSL_CTX_get_cert_store sslF_SSL_CTX_get_cert_store;
meSOCKF_SSL_CTX_load_verify_locations sslF_SSL_CTX_load_verify_locations;
meSOCKF_SSL_CTX_new sslF_SSL_CTX_new;
meSOCKF_SSL_CTX_set1_param sslF_SSL_CTX_set1_param;
meSOCKF_SSL_CTX_set_default_verify_paths sslF_SSL_CTX_set_default_verify_paths;
meSOCKF_SSL_CTX_set_verify sslF_SSL_CTX_set_verify;
meSOCKF_SSL_connect sslF_SSL_connect;
meSOCKF_SSL_ctrl sslF_SSL_ctrl;
meSOCKF_SSL_free sslF_SSL_free;
meSOCKF_SSL_get_current_cipher sslF_SSL_get_current_cipher;
meSOCKF_SSL_get_error sslF_SSL_get_error;
meSOCKF_SSL_get_peer_certificate sslF_SSL_get_peer_certificate;
meSOCKF_SSL_get_verify_result sslF_SSL_get_verify_result;
meSOCKF_SSL_get_verify_result sslF_SSL_get_verify_result;
meSOCKF_SSL_get_version sslF_SSL_get_version;
meSOCKF_SSL_new sslF_SSL_new;
meSOCKF_SSL_read sslF_SSL_read;
meSOCKF_SSL_set_fd sslF_SSL_set_fd;
meSOCKF_SSL_shutdown sslF_SSL_shutdown;
meSOCKF_SSL_write sslF_SSL_write;
meSOCKF_TLS_client_method sslF_TLS_client_method;
meSOCKF_TLS_client_method sslF_TLS_client_method;
meSOCKF_X509_NAME_ENTRY_get_data sslF_X509_NAME_ENTRY_get_data;
meSOCKF_X509_NAME_get_entry sslF_X509_NAME_get_entry;
meSOCKF_X509_NAME_get_index_by_NID sslF_X509_NAME_get_index_by_NID;
meSOCKF_X509_NAME_oneline sslF_X509_NAME_oneline;
meSOCKF_X509_NAME_print_ex sslF_X509_NAME_print_ex;
meSOCKF_X509_NAME_print_ex_fp sslF_X509_NAME_print_ex_fp;
meSOCKF_X509_STORE_add_cert sslF_X509_STORE_add_cert;
meSOCKF_X509_VERIFY_PARAM_free sslF_X509_VERIFY_PARAM_free;
meSOCKF_X509_VERIFY_PARAM_new sslF_X509_VERIFY_PARAM_new;
meSOCKF_X509_VERIFY_PARAM_set_flags sslF_X509_VERIFY_PARAM_set_flags;
meSOCKF_X509_free sslF_X509_free;
meSOCKF_X509_get_ext_d2i sslF_X509_get_ext_d2i;
meSOCKF_X509_get_issuer_name sslF_X509_get_issuer_name;
meSOCKF_X509_get_subject_name sslF_X509_get_subject_name;
meSOCKF_X509_verify_cert_error_string sslF_X509_verify_cert_error_string;
meSOCKF_a2i_IPADDRESS sslF_a2i_IPADDRESS;
meSOCKF_d2i_X509 sslF_d2i_X509;
meSOCKF_X509_NAME_get_text_by_NID sslF_X509_NAME_get_text_by_NID;

static ossl_unused ossl_inline int sslF_sk_GENERAL_NAME_num(const STACK_OF(GENERAL_NAME) *sk)
{
    return MEHTTPFunc(OPENSSL_sk_num)((const OPENSSL_STACK *)sk);
}
static ossl_unused ossl_inline GENERAL_NAME *sslF_sk_GENERAL_NAME_value(const STACK_OF(GENERAL_NAME) *sk, int idx)
{
    return (GENERAL_NAME *) MEHTTPFunc(OPENSSL_sk_value)((const OPENSSL_STACK *)sk, idx);
}
static ossl_unused ossl_inline void sslF_sk_GENERAL_NAME_pop_free(STACK_OF(GENERAL_NAME) *sk, sk_GENERAL_NAME_freefunc freefunc)
{
    MEHTTPFunc(OPENSSL_sk_pop_free)((OPENSSL_STACK *)sk, (OPENSSL_sk_freefunc)freefunc);
}
#endif

static SSL_CTX *meSockCtx=NULL;
#endif
static meUByte *proxyHost=NULL;
static int proxyPort=0;
static meSockLogger logFunc;
static void *logData;
static meUByte *ioBuff=NULL;
static int ioBuffSz=0;
static int timeoutSnd=0;
static int timeoutRcv=0;

static void
strBase64Encode3(meUByte *dd, meUByte c1, meUByte c2, meUByte c3)
{
    static meUByte base64Table[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" ;
    dd[0] = base64Table[(c1 >> 2)] ;
    dd[1] = base64Table[((c1 << 4) & 0x30) | ((c2 >> 4) & 0x0f)] ;
    dd[2] = base64Table[((c2 << 2) & 0x3c) | ((c3 >> 6) & 0x03)] ;
    dd[3] = base64Table[(c3 & 0x3f)] ;
}

static meUByte *
strBase64Encode(meUByte *dd, meUByte *ss)
{
    meUByte c1, c2, c3 ;

    while((c1=*ss++) != '\0')
    {
        if((c2=*ss++) == '\0')
        {
            c3 = '\0' ;
            strBase64Encode3(dd,c1,c2,c3) ;
            dd += 2 ;
            *dd++ = '=' ;
            *dd++ = '=' ;
            break ;
        }
        else if((c3=*ss++) == '\0')
        {
            strBase64Encode3(dd,c1,c2,c3) ;
            dd += 3 ;
            *dd++ = '=' ;
            break ;
        }
        strBase64Encode3(dd,c1,c2,c3) ;
        dd += 4 ;
    }
    *dd = '\0' ;
    return dd ;
}

#if MEOPT_OPENSSL
static void
meSockGetSSLErrors(meUByte *buff)
{
    if(logFunc != NULL)
    {
        unsigned long err;
        int bi = 0;
        while((err = MEHTTPFunc(ERR_get_error)()) != 0)
            bi += snprintf((char *) buff+bi,meSOCK_BUFF_SIZE - bi,"\n  OpenSSL: %s",MEHTTPFunc(ERR_error_string)(err,NULL));
        logFunc(buff,logData);
    }
    else
        MEHTTPFunc(ERR_clear_error)();
}

static int
meSockInitPrng(meUByte *buff)
{
    const char *random_file;
    
    if(MEHTTPFunc(RAND_status)())
        return 0;
    
    /* Get the random file name using RAND_file_name. */
    buff[0] = '\0';
    if(((random_file = MEHTTPFunc(RAND_file_name)((char *) buff,meSOCK_BUFF_SIZE)) != NULL) && (random_file[0] != '\0'))
    {
        /* Seed at most 16k (apparently arbitrary value borrowed from
           curl) from random file. */
        MEHTTPFunc(RAND_load_file)(random_file,16384);
        if(MEHTTPFunc(RAND_status)())
            return 0;
    }
    MEHTTPFunc(RAND_poll)();
    if(MEHTTPFunc(RAND_status)())
        return 0;
    if(logFunc != NULL)
    {
        snprintf((char *) buff,meSOCK_BUFF_SIZE,"meSock Error: Could not seed PRNG");
        logFunc(buff,logData);
    }
    return -1;
}

static int
meSockIsValidIp4Address(const char *str, const char *end)
{
    int saw_digit = 0;
    int octets = 0;
    int val = 0;
    
    while (str < end)
    {
        int ch = *str++;
        
        if (ch >= '0' && ch <= '9')
        {
            val = val * 10 + (ch - '0');
            
            if (val > 255)
                return 0;
            if (!saw_digit)
            {
                if (++octets > 4)
                    return 0;
                saw_digit = 1;
            }
        }
        else if (ch == '.' && saw_digit)
        {
            if (octets == 4)
                return 0;
            val = 0;
            saw_digit = 0;
        }
        else
            return 0;
    }
    if (octets < 4)
        return 0;
    
    return 1;
}

#ifdef ENABLE_IPV6
static int
meSockIsValidIp6Address (const char *str, const char *end)
{
    /* Use lower-case for these to avoid clash with system headers.  */
    enum {
        ns_inaddrsz  = 4,
        ns_in6addrsz = 16,
        ns_int16sz   = 2
    };
    
    const char *curtok;
    int tp;
    const char *colonp;
    int saw_xdigit;
    unsigned int val;
    
    tp = 0;
    colonp = NULL;
    
    if (str == end)
        return 0;
    
    /* Leading :: requires some special handling. */
    if (*str == ':')
    {
        ++str;
        if (str == end || *str != ':')
            return 0;
    }
    
    curtok = str;
    saw_xdigit = 0;
    val = 0;
    
    while (str < end)
    {
        int ch = *str++;
        
        /* if ch is a number, add it to val. */
        if (c_isxdigit (ch))
        {
            val <<= 4;
            val |= XDIGIT_TO_NUM (ch);
            if (val > 0xffff)
                return 0;
            saw_xdigit = 1;
            continue;
        }
        
        /* if ch is a colon ... */
        if (ch == ':')
        {
            curtok = str;
            if (!saw_xdigit)
            {
                if (colonp != NULL)
                    return 0;
                colonp = str + tp;
                continue;
            }
            else if (str == end)
                return 0;
            if (tp > ns_in6addrsz - ns_int16sz)
                return 0;
            tp += ns_int16sz;
            saw_xdigit = 0;
            val = 0;
            continue;
        }
        
        /* if ch is a dot ... */
        if (ch == '.' && (tp <= ns_in6addrsz - ns_inaddrsz)
            && meSockIsValidIp4Address(curtok, end) == 1)
        {
            tp += ns_inaddrsz;
            saw_xdigit = 0;
            break;
        }
        
        return 0;
    }
    
    if (saw_xdigit)
    {
        if (tp > ns_in6addrsz - ns_int16sz)
            return 0;
        tp += ns_int16sz;
    }
    
    if (colonp != NULL)
    {
        if (tp == ns_in6addrsz)
            return 0;
        tp = ns_in6addrsz;
    }
    
    if (tp != ns_in6addrsz)
        return 0;
    
    return 1;
}
#endif

int
meSockIsValidIpAddress(const char *name)
{
    const char *endp;
    
    endp = name + strlen(name);
    if(meSockIsValidIp4Address(name,endp))
        return 1;
#ifdef ENABLE_IPV6
    if(meSockIsValidIp6Address (name, endp))
        return 1;
#endif
    return 0;
}
static void
meSockHostnameGetSni(const char *hostname, char *outBuff)
{
    size_t len = strlen(hostname);
    memcpy(outBuff,hostname,len);

  /* Remove trailing dot(s) to fix #47408.
   * Regarding RFC 6066 (SNI): The hostname is represented as a byte
   * string using ASCII encoding without a trailing dot. */
    while((len > 0) && (outBuff[len-1] == '.'))
        len--;
    outBuff[len] = 0;
}


#ifdef _WIN32
void
meSockInitSystemCaCert(SSL_CTX *sslCtx, meUByte logFlags, char *rbuff)
{
    HCERTSTORE hStore;
    PCCERT_CONTEXT cCntx = NULL;
    const unsigned char *ce;
    X509 *x509;
    X509_STORE *store;
    char buf[128];
    int ii=1;
    
    if((store = MEHTTPFunc(SSL_CTX_get_cert_store)(sslCtx)) == NULL)
    {
        if(logFlags & meSOCK_LOG_WARNING)
            logFunc((meUByte *) "OpenSSL: Failed to get SSL_CTX cert store",logData);
        return;
    }

    do {
        // The Root store holds the main root CAs and the CA store holds any intermediate CAs - bail out if we can't open the Root
        if((hStore = CertOpenSystemStore(0,(ii) ? "Root":"CA")) == 0)
        {
            if(logFlags & meSOCK_LOG_WARNING)
                logFunc((meUByte *) "OpenSSL: Failed to open Root system cert store",logData);
            return;
        }
        
        while((cCntx = CertEnumCertificatesInStore(hStore,cCntx)) != NULL)
        {
            //uncomment the line below if you want to see the certificates as pop ups
            //CryptUIDlgViewContext(CERT_STORE_CERTIFICATE_CONTEXT,cCntx,NULL,NULL,0,NULL);
            ce = cCntx->pbCertEncoded;
            x509 = MEHTTPFunc(d2i_X509)(NULL,&ce,cCntx->cbCertEncoded);
            if (x509)
            {
                MEHTTPFunc(X509_NAME_oneline)(MEHTTPFunc(X509_get_subject_name)(x509),buf,sizeof(buf));
                if(MEHTTPFunc(X509_STORE_add_cert)(store,x509) == 1)
                {
                    if(logFlags & meSOCK_LOG_VERBOSE)
                    {
                        snprintf(rbuff,meSOCK_BUFF_SIZE,"OpenSSL: Loaded & added Windows Root certificate for: subject='%s'",buf);
                        logFunc((meUByte *) rbuff,logData);
                    }
                }
                else if(logFlags & meSOCK_LOG_WARNING)
                {
                    snprintf(rbuff,meSOCK_BUFF_SIZE,"OpenSSL: Loaded but failed to added Windows Root certificate for: subject='%s'",buf);
                    logFunc((meUByte *) rbuff,logData);
                }
                MEHTTPFunc(X509_free)(x509);
            }
            else if(logFlags & meSOCK_LOG_WARNING)
            {
                snprintf(rbuff,meSOCK_BUFF_SIZE,"OpenSSL: Failed to generate x509 for Windows Root certificate",logData);
                logFunc((meUByte *) rbuff,logData);
            }
        }
        CertCloseStore(hStore,0);
    } while(--ii >= 0);
}
#endif          

int
meSockInit(meUByte logFlags, meUByte *buff)
{
#if meSOCK_USE_DLL
#ifdef _WIN32
    HINSTANCE libHandle ; 
#else
    void *libHandle ; 
#endif
#endif          
    SSL_METHOD const *meth;
#ifdef X509_V_FLAG_PARTIAL_CHAIN
    X509_VERIFY_PARAM *param;
#endif          
    if(meSockCtx != NULL)
        /* The SSL has already been initialized. */
        return 1;
    
#if meSOCK_USE_DLL
    if(logFlags & meSOCK_LOG_VERBOSE)
        logFunc((meUByte *) "About to get OpenSSL functions",logData);
#ifdef _WIN32
    libHandle = LoadLibrary(MEHTTP_STRINGIFY(_OPENSSLCNM));
#else
    libHandle = dlopen(MEHTTP_STRINGIFY(_OPENSSLCNM),RTLD_LOCAL|RTLD_LAZY);
#endif
    if(libHandle == NULL)
    {
        snprintf((char *) buff,meSOCK_BUFF_SIZE,"meSock Error: Failed to load " MEHTTP_STRINGIFY(_OPENSSLCNM) " library - OpenSSL installed? (%d)",meSockGetError());
        if(logFunc != NULL)
            logFunc(buff,logData);
        return -1;
    }
    if(((sslF_ASN1_OCTET_STRING_free = (meSOCKF_ASN1_OCTET_STRING_free) meSockLibGetFunc(libHandle,"ASN1_OCTET_STRING_free")) == NULL) ||
       ((sslF_ASN1_STRING_cmp = (meSOCKF_ASN1_STRING_cmp) meSockLibGetFunc(libHandle,"ASN1_STRING_cmp")) == NULL) ||
       ((sslF_ASN1_STRING_length = (meSOCKF_ASN1_STRING_length) meSockLibGetFunc(libHandle,"ASN1_STRING_length")) == NULL) ||
       ((sslF_ASN1_STRING_to_UTF8 = (meSOCKF_ASN1_STRING_to_UTF8) meSockLibGetFunc(libHandle,"ASN1_STRING_to_UTF8")) == NULL) ||
       ((sslF_BIO_free = (meSOCKF_BIO_free) meSockLibGetFunc(libHandle,"BIO_free")) == NULL) ||
       ((sslF_BIO_new = (meSOCKF_BIO_new) meSockLibGetFunc(libHandle,"BIO_new")) == NULL) ||
       ((sslF_BIO_number_written = (meSOCKF_BIO_number_written) meSockLibGetFunc(libHandle,"BIO_number_written")) == NULL) ||
       ((sslF_BIO_read = (meSOCKF_BIO_read) meSockLibGetFunc(libHandle,"BIO_read")) == NULL) ||
       ((sslF_BIO_s_mem = (meSOCKF_BIO_s_mem) meSockLibGetFunc(libHandle,"BIO_s_mem")) == NULL) ||
       ((sslF_CONF_modules_load_file = (meSOCKF_CONF_modules_load_file) meSockLibGetFunc(libHandle,"CONF_modules_load_file")) == NULL) ||
       ((sslF_CRYPTO_free = (meSOCKF_CRYPTO_free) meSockLibGetFunc(libHandle,"CRYPTO_free")) == NULL) ||
       ((sslF_ENGINE_load_builtin_engines = (meSOCKF_ENGINE_load_builtin_engines) meSockLibGetFunc(libHandle,"ENGINE_load_builtin_engines")) == NULL) ||
       ((sslF_ERR_clear_error = (meSOCKF_ERR_clear_error) meSockLibGetFunc(libHandle,"ERR_clear_error")) == NULL) ||
       ((sslF_ERR_error_string = (meSOCKF_ERR_error_string) meSockLibGetFunc(libHandle,"ERR_error_string")) == NULL) ||
       ((sslF_ERR_get_error = (meSOCKF_ERR_get_error) meSockLibGetFunc(libHandle,"ERR_get_error")) == NULL) ||
       ((sslF_GENERAL_NAME_free = (meSOCKF_GENERAL_NAME_free) meSockLibGetFunc(libHandle,"GENERAL_NAME_free")) == NULL) ||
       ((sslF_OPENSSL_load_builtin_modules = (meSOCKF_OPENSSL_load_builtin_modules) meSockLibGetFunc(libHandle,"OPENSSL_load_builtin_modules")) == NULL) ||
       ((sslF_OPENSSL_sk_num = (meSOCKF_OPENSSL_sk_num) meSockLibGetFunc(libHandle,"OPENSSL_sk_num")) == NULL) ||
       ((sslF_OPENSSL_sk_pop_free = (meSOCKF_OPENSSL_sk_pop_free) meSockLibGetFunc(libHandle,"OPENSSL_sk_pop_free")) == NULL) ||
       ((sslF_OPENSSL_sk_value = (meSOCKF_OPENSSL_sk_value) meSockLibGetFunc(libHandle,"OPENSSL_sk_value")) == NULL) ||
       ((sslF_RAND_file_name = (meSOCKF_RAND_file_name) meSockLibGetFunc(libHandle,"RAND_file_name")) == NULL) ||
       ((sslF_RAND_load_file = (meSOCKF_RAND_load_file) meSockLibGetFunc(libHandle,"RAND_load_file")) == NULL) ||
       ((sslF_RAND_poll = (meSOCKF_RAND_poll) meSockLibGetFunc(libHandle,"RAND_poll")) == NULL) ||
       ((sslF_RAND_status = (meSOCKF_RAND_status) meSockLibGetFunc(libHandle,"RAND_status")) == NULL) ||
       ((sslF_X509_NAME_ENTRY_get_data = (meSOCKF_X509_NAME_ENTRY_get_data) meSockLibGetFunc(libHandle,"X509_NAME_ENTRY_get_data")) == NULL) ||
       ((sslF_X509_NAME_get_entry = (meSOCKF_X509_NAME_get_entry) meSockLibGetFunc(libHandle,"X509_NAME_get_entry")) == NULL) ||
       ((sslF_X509_NAME_get_index_by_NID = (meSOCKF_X509_NAME_get_index_by_NID) meSockLibGetFunc(libHandle,"X509_NAME_get_index_by_NID")) == NULL) ||
       ((sslF_X509_NAME_get_text_by_NID = (meSOCKF_X509_NAME_get_text_by_NID) meSockLibGetFunc(libHandle,"X509_NAME_get_text_by_NID")) == NULL) ||
       ((sslF_X509_NAME_oneline = (meSOCKF_X509_NAME_oneline) meSockLibGetFunc(libHandle,"X509_NAME_oneline")) == NULL) ||
       ((sslF_X509_NAME_print_ex = (meSOCKF_X509_NAME_print_ex) meSockLibGetFunc(libHandle,"X509_NAME_print_ex")) == NULL) ||
       ((sslF_X509_NAME_print_ex_fp = (meSOCKF_X509_NAME_print_ex_fp) meSockLibGetFunc(libHandle,"X509_NAME_print_ex_fp")) == NULL) ||
       ((sslF_X509_STORE_add_cert = (meSOCKF_X509_STORE_add_cert) meSockLibGetFunc(libHandle,"X509_STORE_add_cert")) == NULL) ||
       ((sslF_X509_VERIFY_PARAM_free = (meSOCKF_X509_VERIFY_PARAM_free) meSockLibGetFunc(libHandle,"X509_VERIFY_PARAM_free")) == NULL) ||
       ((sslF_X509_VERIFY_PARAM_new = (meSOCKF_X509_VERIFY_PARAM_new) meSockLibGetFunc(libHandle,"X509_VERIFY_PARAM_new")) == NULL) ||
       ((sslF_X509_VERIFY_PARAM_set_flags = (meSOCKF_X509_VERIFY_PARAM_set_flags) meSockLibGetFunc(libHandle,"X509_VERIFY_PARAM_set_flags")) == NULL) ||
       ((sslF_X509_free = (meSOCKF_X509_free) meSockLibGetFunc(libHandle,"X509_free")) == NULL) ||
       ((sslF_X509_get_ext_d2i = (meSOCKF_X509_get_ext_d2i) meSockLibGetFunc(libHandle,"X509_get_ext_d2i")) == NULL) ||
       ((sslF_X509_get_issuer_name = (meSOCKF_X509_get_issuer_name) meSockLibGetFunc(libHandle,"X509_get_issuer_name")) == NULL) ||
       ((sslF_X509_get_subject_name = (meSOCKF_X509_get_subject_name) meSockLibGetFunc(libHandle,"X509_get_subject_name")) == NULL) ||
       ((sslF_X509_verify_cert_error_string = (meSOCKF_X509_verify_cert_error_string) meSockLibGetFunc(libHandle,"X509_verify_cert_error_string")) == NULL) ||
       ((sslF_a2i_IPADDRESS = (meSOCKF_a2i_IPADDRESS) meSockLibGetFunc(libHandle,"a2i_IPADDRESS")) == NULL) ||
       ((sslF_d2i_X509 = (meSOCKF_d2i_X509) meSockLibGetFunc(libHandle,"d2i_X509")) == NULL))
    {
        snprintf((char *) buff,meSOCK_BUFF_SIZE,"meSock Error: Failed to load libcrypto functions (%d)",meSockGetError());
        if(logFunc != NULL)
            logFunc(buff,logData);
        return -2;
    }
          
#ifdef _WIN32
    libHandle = LoadLibrary(MEHTTP_STRINGIFY(_OPENSSLLNM));
#else
    libHandle = dlopen(MEHTTP_STRINGIFY(_OPENSSLLNM),RTLD_LOCAL|RTLD_LAZY);
#endif
    if(libHandle == NULL)
    {
        snprintf((char *) buff,meSOCK_BUFF_SIZE,"meSock Error: Failed to load " MEHTTP_STRINGIFY(_OPENSSLLNM) " library - OpenSSL installed? (%d)",meSockGetError());
        if(logFunc != NULL)
            logFunc(buff,logData);
        return -3;
    }
    if(((sslF_OPENSSL_init_ssl = (meSOCKF_OPENSSL_init_ssl) meSockLibGetFunc(libHandle,"OPENSSL_init_ssl")) == NULL) ||
       ((sslF_SSL_CIPHER_get_name = (meSOCKF_SSL_CIPHER_get_name) meSockLibGetFunc(libHandle,"SSL_CIPHER_get_name")) == NULL) ||
       ((sslF_SSL_CTX_ctrl = (meSOCKF_SSL_CTX_ctrl) meSockLibGetFunc(libHandle,"SSL_CTX_ctrl")) == NULL) ||
       ((sslF_SSL_CTX_free = (meSOCKF_SSL_CTX_free) meSockLibGetFunc(libHandle,"SSL_CTX_free")) == NULL) ||
       ((sslF_SSL_CTX_get_cert_store = (meSOCKF_SSL_CTX_get_cert_store) meSockLibGetFunc(libHandle,"SSL_CTX_get_cert_store")) == NULL) ||
       ((sslF_SSL_CTX_load_verify_locations = (meSOCKF_SSL_CTX_load_verify_locations) meSockLibGetFunc(libHandle,"SSL_CTX_load_verify_locations")) == NULL) ||
       ((sslF_SSL_CTX_new = (meSOCKF_SSL_CTX_new) meSockLibGetFunc(libHandle,"SSL_CTX_new")) == NULL) ||
       ((sslF_SSL_CTX_set1_param = (meSOCKF_SSL_CTX_set1_param) meSockLibGetFunc(libHandle,"SSL_CTX_set1_param")) == NULL) ||
       ((sslF_SSL_CTX_set_default_verify_paths = (meSOCKF_SSL_CTX_set_default_verify_paths) meSockLibGetFunc(libHandle,"SSL_CTX_set_default_verify_paths")) == NULL) ||
       ((sslF_SSL_CTX_set_verify = (meSOCKF_SSL_CTX_set_verify) meSockLibGetFunc(libHandle,"SSL_CTX_set_verify")) == NULL) ||
       ((sslF_SSL_connect = (meSOCKF_SSL_connect) meSockLibGetFunc(libHandle,"SSL_connect")) == NULL) ||
       ((sslF_SSL_ctrl = (meSOCKF_SSL_ctrl) meSockLibGetFunc(libHandle,"SSL_ctrl")) == NULL) ||
       ((sslF_SSL_free = (meSOCKF_SSL_free) meSockLibGetFunc(libHandle,"SSL_free")) == NULL) ||
       ((sslF_SSL_get_current_cipher = (meSOCKF_SSL_get_current_cipher) meSockLibGetFunc(libHandle,"SSL_get_current_cipher")) == NULL) ||
       ((sslF_SSL_get_error = (meSOCKF_SSL_get_error) meSockLibGetFunc(libHandle,"SSL_get_error")) == NULL) ||
       ((sslF_SSL_get_peer_certificate = (meSOCKF_SSL_get_peer_certificate) meSockLibGetFunc(libHandle,"SSL_get_peer_certificate")) == NULL) ||
       ((sslF_SSL_get_verify_result = (meSOCKF_SSL_get_verify_result) meSockLibGetFunc(libHandle,"SSL_get_verify_result")) == NULL) ||
       ((sslF_SSL_get_version = (meSOCKF_SSL_get_version) meSockLibGetFunc(libHandle,"SSL_get_version")) == NULL) ||
       ((sslF_SSL_new = (meSOCKF_SSL_new) meSockLibGetFunc(libHandle,"SSL_new")) == NULL) ||
       ((sslF_SSL_read = (meSOCKF_SSL_read) meSockLibGetFunc(libHandle,"SSL_read")) == NULL) ||
       ((sslF_SSL_set_fd = (meSOCKF_SSL_set_fd) meSockLibGetFunc(libHandle,"SSL_set_fd")) == NULL) ||
       ((sslF_SSL_shutdown = (meSOCKF_SSL_shutdown) meSockLibGetFunc(libHandle,"SSL_shutdown")) == NULL) ||
       ((sslF_SSL_write = (meSOCKF_SSL_write) meSockLibGetFunc(libHandle,"SSL_write")) == NULL) ||
       ((sslF_TLS_client_method = (meSOCKF_TLS_client_method) meSockLibGetFunc(libHandle,"TLS_client_method")) == NULL) ||
       0)
    {
        snprintf((char *) buff,meSOCK_BUFF_SIZE,"meSock Error: Failed to load libssl functions (%d)",meSockGetError());
        if(logFunc != NULL)
            logFunc(buff,logData);
        return -4;
    }
#endif        
       
    /* Init the PRNG.  If that fails, bail out.  */
    if(meSockInitPrng(buff) < 0)
    {
        meSockGetSSLErrors(buff);
        return -7;
    }
    
    MEHTTPFunc(OPENSSL_load_builtin_modules)();
#ifndef OPENSSL_NO_ENGINE
    MEHTTPFunc(ENGINE_load_builtin_engines)();
#endif
    MEHTTPFunc(CONF_modules_load_file)(NULL,NULL,CONF_MFLAGS_DEFAULT_SECTION|CONF_MFLAGS_IGNORE_MISSING_FILE);
    MEHTTPFunc(OPENSSL_init_ssl)(0, NULL);
    
    meth = MEHTTPFunc(TLS_client_method)();
    meSockCtx = MEHTTPFunc(SSL_CTX_new)(meth);
    if(!meSockCtx)
    {
        meSockGetSSLErrors(buff);
        return -8;
    }
    
    MEHTTPFunc(SSL_CTX_set_default_verify_paths)(meSockCtx);
    MEHTTPFunc(SSL_CTX_load_verify_locations)(meSockCtx, NULL,NULL);
#ifdef _WIN32
    // load windows system ca certs
    meSockInitSystemCaCert(meSockCtx,logFlags,(char *) buff);
#endif
    
#ifdef X509_V_FLAG_PARTIAL_CHAIN
    /* Set X509_V_FLAG_PARTIAL_CHAIN to allow the client to anchor trust in
     * a non-self-signed certificate. This defies RFC 4158 (Path Building)
     * which defines a trust anchor in terms of a self-signed certificate.
     * However, it substantially reduces attack surface by pruning the tree
     * of unneeded trust points. For example, the cross-certified
     * Let's Encrypt X3 CA, which protects gnu.org and appears as an
     * intermediate CA to clients, can be used as a trust anchor without
     * the entire IdentTrust PKI.
     */
    param = MEHTTPFunc(X509_VERIFY_PARAM_new)();
    if (param)
    {
        /* We only want X509_V_FLAG_PARTIAL_CHAIN, but the OpenSSL docs
         * say to use X509_V_FLAG_TRUSTED_FIRST also. It looks like
         * X509_V_FLAG_TRUSTED_FIRST applies to a collection of trust
         * anchors and not a single trust anchor.
         */
        (void) MEHTTPFunc(X509_VERIFY_PARAM_set_flags)(param, X509_V_FLAG_TRUSTED_FIRST | X509_V_FLAG_PARTIAL_CHAIN);
        if(MEHTTPFunc(SSL_CTX_set1_param)(meSockCtx, param) == 0)
        {
            if(logFlags & meSOCK_LOG_WARNING)
                logFunc((meUByte *) "meSock Warning: Failed set trust to partial chain",logData);
        }
        /* We continue on error */
        MEHTTPFunc(X509_VERIFY_PARAM_free)(param);
    }
    else if(logFlags & meSOCK_LOG_WARNING)
    {
        logFunc((meUByte *) "meSock Warning: Failed to allocate verification param",logData);
        /* We continue on error */
    }
#endif
    
    /* SSL_VERIFY_NONE instructs OpenSSL not to abort SSL_connect if the certificate is invalid. We
     * verify the certificate separately in meSockCheckCertificate, which provides much better
     * diagnostics than examining the error stack after a failed SSL_connect. */
    MEHTTPFunc(SSL_CTX_set_verify)(meSockCtx, SSL_VERIFY_NONE, NULL);
    
    
    /* The OpenSSL library can handle renegotiations automatically, so tell it to do so. */
    MEHTTPFunc(SSL_CTX_ctrl)(meSockCtx,SSL_CTRL_MODE,SSL_MODE_AUTO_RETRY,NULL);
    
    return 0;
}

#define ASTERISK_EXCLUDES_DOT   /* mandated by rfc2818 */

/* Return true is STRING (case-insensitively) matches PATTERN, false otherwise. The recognized
 * wildcard character is "*", which matches any character in STRING except ".". Any number of the
 * "*" wildcard may be present in the pattern.
 * 
 * This is used to match of hosts as indicated in rfc2818:
 * 
 *      Names may contain the wildcard character * which is considered to match any single domain
 *      name component or component fragment. E.g., *.a.com matches foo.a.com but not bar.foo.a.com.
 *      f*.com matches foo.com but not bar.com [or foo.bar.com].
 * 
 * If the pattern contain no wildcards, pattern_match(a, b) is equivalent to !strcasecmp(a, b).
 */

static int
meSockPatternMatch(const char *pattern, const char *string)
{
    const char *p = pattern, *n = string;
    char c;
    for (; (c = tolower(*p++)) != '\0'; n++)
    {
        if (c == '*')
        {
            for (c = tolower(*p); c == '*'; c = tolower(*++p))
                ;
            for (; *n != '\0'; n++)
            {
                if (tolower(*n) == c && meSockPatternMatch(p, n))
                    return 1;
#ifdef ASTERISK_EXCLUDES_DOT
                else if (*n == '.')
                    return 0;
#endif
            }
            return (c == '\0');
        }
        else if (c != tolower(*n))
            return 0;
    }
    return *n == '\0';
}

static char *
meSockGetRfc2253FormattedName(X509_NAME *name)
{
    int len;
    char *out=NULL;
    BIO* b;
    
    if((b = MEHTTPFunc(BIO_new)(MEHTTPFunc(BIO_s_mem)())))
    {
        if((MEHTTPFunc(X509_NAME_print_ex)(b,name,0,XN_FLAG_RFC2253) >= 0) && 
           ((len = (int) MEHTTPFunc(BIO_number_written)(b)) > 0))
        {
            out = malloc(len+1);
            MEHTTPFunc(BIO_read)(b,out,len);
            out[len] = 0;
        }
        MEHTTPFunc(BIO_free)(b);
    }
    
    return (out == NULL) ? strdup(""):out;
}

int
meSockCheckCertificate(meSockFile *sFp, const char *sniHost, meUByte *rbuff)
{
    X509 *cert;
    GENERAL_NAMES *subjectAltNames;
    char common_name[256];
    long vresult;
    int ret, alt_name_checked = 0;
    const char *severityLbl;
    
#if 0
    int pinsuccess = opt.pinnedpubkey == NULL;
    /* The user explicitly said to not check for the certificate.  */
    if ((sFp->flags == meSOCK_IGN_CRT_ERR) && pinsuccess)
        return success;
#else
    if(sFp->flags == meSOCK_IGN_CRT_ERR)
        /* don't error and no logging - so stop now */
        return 1;
#endif
    severityLbl = (sFp->flags & meSOCK_IGN_CRT_ERR) ? "Warning":"Error";
        
    if((cert = MEHTTPFunc(SSL_get_peer_certificate)(sFp->ssl)) == NULL)
    {
        snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSock %s: No certificate presented by %s",severityLbl,sniHost);
        if(logFunc != NULL)
            logFunc(rbuff,logData);
        return (sFp->flags & meSOCK_IGN_CRT_ERR) ? 1:0;
    }
    
    if(sFp->flags & meSOCK_LOG_VERBOSE)
    {
        char *subject = meSockGetRfc2253FormattedName(MEHTTPFunc(X509_get_subject_name)(cert));
        char *issuer = meSockGetRfc2253FormattedName(MEHTTPFunc(X509_get_issuer_name)(cert));
        snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSock Debug - Certificate:\n  subject: %s\n  issuer:  %s",subject,issuer);
        logFunc(rbuff,logData);
        free(subject);
        free(issuer);
    }
    
    ret = 1;
    vresult = MEHTTPFunc(SSL_get_verify_result)(sFp->ssl);
    if (vresult != X509_V_OK)
    {
        /* TODO only warn if option to disable cert check */
        char *issuer = meSockGetRfc2253FormattedName(MEHTTPFunc(X509_get_issuer_name)(cert));
        snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSock %s: Cannot verify %s's certificate, issued by %s:",severityLbl,sniHost,issuer);
        free(issuer);
        if(logFunc != NULL)
        {
            logFunc(rbuff,logData);
            /* Try to print more user-friendly (and translated) messages for
               the frequent verification errors.  */
            switch (vresult)
            {
            case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY:
                logFunc((meUByte *) "  Unable to locally verify the issuer's authority",logData);
                break;
            case X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN:
            case X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT:
                logFunc((meUByte *) "  Self-signed certificate encountered",logData);
                break;
            case X509_V_ERR_CERT_NOT_YET_VALID:
                logFunc((meUByte *) "  Issued certificate not yet valid",logData);
                break;
            case X509_V_ERR_CERT_HAS_EXPIRED:
                logFunc((meUByte *) "  Issued certificate has expired",logData);
                break;
            default:
                /* For the less frequent error strings, simply provide the OpenSSL error message. */
                /* TODO - this erases the prefered return string */
                snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"  %s", MEHTTPFunc(X509_verify_cert_error_string)(vresult));
                logFunc(rbuff,logData);
            }
        }
        ret = 0;
        /* Fall through, so that the user is warned about *all* issues
           with the cert (important with --no-check-certificate.)  */
    }
    
    /* Check that HOST matches the common name in the certificate. The following remains to be
     * done:
     * 
     * - When matching against common names, it should loop over all common names and choose the
     *   most specific one, i.e. the last one, not the first one, which the current code picks.
     * 
     * - Ensure that ASN1 strings from the certificate are encoded as UTF-8 which can be
     *   meaningfully compared to HOST.
     */
    if((subjectAltNames = MEHTTPFunc(X509_get_ext_d2i)(cert,NID_subject_alt_name,NULL,NULL)) != NULL)
    {
        /* Test subject alternative names */
        /* Do we want to check for dNSNames or ipAddresses (see RFC 2818)?
         * Signal it by host_in_octet_string. */
        ASN1_OCTET_STRING *host_in_octet_string = MEHTTPFunc(a2i_IPADDRESS)(sniHost);
        
        int numaltnames = MEHTTPFunc(sk_GENERAL_NAME_num)(subjectAltNames);
        int ii;
        for(ii=0; ii<numaltnames; ii++)
        {
            const GENERAL_NAME *name = MEHTTPFunc(sk_GENERAL_NAME_value)(subjectAltNames,ii);
            if (name)
            {
                if (host_in_octet_string)
                {
                    if (name->type == GEN_IPADD)
                    {
                        /* Check for ipAddress */
                        /* TODO: Should we convert between IPv4-mapped IPv6 addresses and IPv4 addresses? */
                        alt_name_checked = 1;
                        if (!MEHTTPFunc(ASN1_STRING_cmp)(host_in_octet_string,name->d.iPAddress))
                            break;
                    }
                }
                else if (name->type == GEN_DNS)
                {
                    /* dNSName should be IA5String (i.e. ASCII), however who does trust CA? Convert
                     * it into UTF-8 for sure. */
                    unsigned char *name_in_utf8 = NULL;
                    
                    /* Check for dNSName */
                    alt_name_checked = 1;
                    
                    if (0 <= MEHTTPFunc(ASN1_STRING_to_UTF8)(&name_in_utf8, name->d.dNSName))
                    {
                        /* Compare and check for NULL attack in ASN1_STRING */
                        if (meSockPatternMatch((char *) name_in_utf8,sniHost) &&
                            (strlen((char *) name_in_utf8) == (size_t) MEHTTPFunc(ASN1_STRING_length)(name->d.dNSName)))
                        {
                            MEHTTPFunc(CRYPTO_free)(name_in_utf8,OPENSSL_FILE,OPENSSL_LINE);
                            break;
                        }
                        MEHTTPFunc(CRYPTO_free)(name_in_utf8,OPENSSL_FILE,OPENSSL_LINE);
                    }
                }
            }
        }
        MEHTTPFunc(sk_GENERAL_NAME_pop_free)(subjectAltNames,MEHTTPFunc(GENERAL_NAME_free));
        if(host_in_octet_string)
            MEHTTPFunc(ASN1_OCTET_STRING_free)(host_in_octet_string);
        
        if((alt_name_checked == 1) && (ii >= numaltnames))
        {
            snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSock %s: No certificate subject alternative name matches requested host name %s",severityLbl,sniHost);
            if(logFunc != NULL)
                logFunc(rbuff,logData);
            ret = 0;
        }
    }
    
    if (alt_name_checked == 0)
    {
        /* Test commomName */
        X509_NAME *xname = MEHTTPFunc(X509_get_subject_name)(cert);
        common_name[0] = '\0';
        MEHTTPFunc(X509_NAME_get_text_by_NID)(xname,NID_commonName,common_name,sizeof(common_name));
        
        if (!meSockPatternMatch(common_name,sniHost))
        {
            snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSock %s: Certificate common name %s doesn't match requested host name %s",severityLbl,common_name,sniHost);
            if(logFunc != NULL)
                logFunc(rbuff,logData);
            ret = 0;
        }
        else
        {
            /* We now determine the length of the ASN1 string. If it differs from common_name's
             * length, then there is a \0 before the string terminates. This can be an instance of a
             * null-prefix attack.
             * 
             * https://www.blackhat.com/html/bh-usa-09/bh-usa-09-archives.html#Marlinspike
             */
            int ii=-1, jj;
            X509_NAME_ENTRY *xentry;
            ASN1_STRING *sdata;
            
            if(xname)
            {
                for (;;)
                {
                    jj = MEHTTPFunc(X509_NAME_get_index_by_NID)(xname, NID_commonName, ii);
                    if(jj == -1)
                        break;
                    ii = jj;
                }
            }
            
            xentry = MEHTTPFunc(X509_NAME_get_entry)(xname,ii);
            sdata = MEHTTPFunc(X509_NAME_ENTRY_get_data)(xentry);
            if (strlen(common_name) != (size_t) MEHTTPFunc(ASN1_STRING_length)(sdata))
            {
                snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSock %s: Certificate common name is invalid (contains a NUL character).\n  This may be an indication that the host is not who it claims to be\n  (that is, it is not the real %s).",severityLbl,sniHost);
                if(logFunc != NULL)
                    logFunc(rbuff,logData);
                ret = 0;
            }
        }
    }
#if 0
    // TODO - believe this is for server with a specific public key (i.e. like ssh)
    pinsuccess = pkp_pin_peer_pubkey (cert, opt.pinnedpubkey);
    if (!pinsuccess)
    {
        snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"The public key for %s does not match pinned public key",sniHost);
        if(logFunc != NULL)
            logFunc(rbuff,logData);
        ret = 0;
    }
#endif    
    
    if(ret && (sFp->flags & meSOCK_LOG_VERBOSE))
    {
        snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSock Debug: X509 certificate successfully verified and matches host %s",sniHost);
        logFunc(rbuff,logData);
    }
    MEHTTPFunc(X509_free)(cert);
    
    return (sFp->flags & meSOCK_IGN_CRT_ERR) ? 1:ret;
}
#endif

static int
meSockReadLine(meSockFile *sFp, meUByte *buff)
{
    meUByte *dd=buff, cc ;
    meInt ret, err;
    for(;;)
    {
#if MEOPT_OPENSSL
        if(sFp->flags & meSOCK_USE_HTTPS)
            ret = MEHTTPFunc(SSL_read)(sFp->ssl,dd,1);
        else
#endif
            ret = meSocketRead(sFp->sck,(char *) dd,1,0);
        if(ret <= 0)
        {
#if MEOPT_OPENSSL
            if(sFp->flags & meSOCK_USE_HTTPS)
                err = MEHTTPFunc(SSL_get_error)(sFp->ssl,ret);
            else
#endif
                err = meSocketGetError();
            snprintf((char *) buff,meSOCK_BUFF_SIZE,"meSock Error: Failed to read line - %d,%d",ret,err);
            if(logFunc != NULL)
                logFunc(buff,logData);
#if MEOPT_OPENSSL
            if(sFp->flags & meSOCK_USE_HTTPS)
                meSockGetSSLErrors(buff);
#endif
            sFp->flags &= ~meSOCK_REUSE;
            return -1;
        }
        if((cc=*dd) == '\n')
            break ;
        if(cc != '\r')
            dd++;
    }
    *dd = '\0';
    return 1;
}


void
meSockSetup(meSockLogger logger,void *data, int toSnd, int toRcv, int ioBfSz, meUByte *ioBf)
{
    logFunc = logger;
    logData = data;
    timeoutSnd = toSnd;
    timeoutRcv = toRcv;
    ioBuff = ioBf;
    ioBuffSz = ioBfSz;
}

void
meSockSetProxy(meUByte *host, meInt port)
{
    if(proxyHost != NULL)
    {
        if((host == NULL) || strcmp((char *) host,(char *) proxyHost))
        {
            free(proxyHost);
            proxyHost = NULL;
        }
        else
            host = NULL;
    }
    if(host != NULL)
        proxyHost = (meUByte *) strdup((char *) host);
    proxyPort = port;
}

int
meSockHttpOpen(meSockFile *sFp, meUByte flags, meUByte *host, meInt port, meUByte *user, meUByte *pass, meUByte *file, meCookie *cookie, 
               meInt fdLen, meUByte *frmData, meUByte *postFName, meUByte *rbuff)
{
    meUByte *ss;
    meInt pfs, ret, err;
    FILE *pfp=NULL;
    meSOCKET sck;
#if MEOPT_OPENSSL
    SSL *ssl;
#endif

    if(logFunc == NULL)
        flags &= ~(meSOCK_LOG_WARNING|meSOCK_LOG_HEADERS|meSOCK_LOG_VERBOSE);
    
    if(meSockFileIsInUse(sFp) && (!meSockFileCanReuse(sFp) || (sFp->cprt != port) || (sFp->length < 0) || ((sck=sFp->sck) == meBadSocket) || 
#if MEOPT_OPENSSL
                                  (((ssl=sFp->ssl) != NULL) ^ ((flags & meSOCK_USE_HTTPS) != 0)) || 
#else
                                  ((flags & meSOCK_USE_HTTPS) != 0) || 
#endif
                                  strcmp((char *) sFp->chst,(char *) host)))
        meSockClose(sFp,1);
    sFp->length = -1;
    if(meSockFileIsInUse(sFp))
        sFp->flags = flags|meSOCK_INUSE;
    else
    {
        struct sockaddr_in sa;
        struct hostent *hp;
        meUByte *thost;
        meInt tport;
    
#if MEOPT_OPENSSL == 0
        if(flags & meSOCK_USE_HTTPS)
        {
            logFunc((meUByte *) "meSock Error: HTTPS not supported in this build",logData);
            return -24;
        }
#endif
        sFp->flags = flags|meSOCK_INUSE;
        sFp->sck = meBadSocket;
        sFp->ssl = NULL;
        sFp->cprt = -1;
        if(ioBuffSz < meSOCKBUFSIZ)
        {
            if(logFunc != NULL)
                logFunc((meUByte *) "meSock Error: Setup not called with IO buffer",logData);
            return -10;
        }
        if((fdLen > 0) && (postFName != NULL))
        {
            if(logFunc != NULL)
                logFunc((meUByte *) "meSockHttp Error: Cannot use form data and post file at the same time",logData);
            return -21;
        }
#if MEOPT_OPENSSL
        if(flags & meSOCK_USE_HTTPS)
        {
            if((meSockCtx == NULL) && ((ret = meSockInit(flags,rbuff)) < 0))
                return ret;
            MEHTTPFunc(ERR_clear_error)();
        }
#endif
        if(proxyHost != NULL)
        {
            thost = proxyHost ;
            tport = proxyPort ;
        }
        else
        {
            thost = host ;
            tport = port ;
        }
        
        if(((hp = gethostbyname((char *) thost)) == NULL) ||
           (hp->h_length > ((int) sizeof(struct in_addr))) ||
           (hp->h_addrtype != AF_INET))
        {
            if(logFunc != NULL)
            {
                snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSockHttp Error: Failed to get host [%s] %d",thost,(hp == NULL) ? meSocketGetError():-1);
                logFunc(rbuff,logData);
            }
            return -11;
        }
        memset(&sa,0,sizeof(sa)) ;
        sa.sin_family=AF_INET;
        memcpy(&sa.sin_addr.s_addr,hp->h_addr,hp->h_length) ;
        if(tport)
            sa.sin_port=htons(tport);
        else if(flags & meSOCK_USE_HTTPS)
            sa.sin_port=htons(443);
        else
            sa.sin_port=htons(80);
        
#if MEOPT_OPENSSL
        if(flags & meSOCK_USE_HTTPS)
        {
            if((ssl=MEHTTPFunc(SSL_new)(meSockCtx)) == NULL)
            {
                if(logFunc != NULL)
                    logFunc((meUByte *) "meSockHttp Error: Error creating SSL structure",logData);
                return -12;
            }
            sFp->ssl = ssl;
            
            meSockHostnameGetSni((char *) thost,(char *) ioBuff);
            if(!meSockIsValidIpAddress((char *) ioBuff))
            {
                // SSL_set_tlsext_host_name(ssl,ioBuff);
                long rc = MEHTTPFunc(SSL_ctrl)(ssl,SSL_CTRL_SET_TLSEXT_HOSTNAME,TLSEXT_NAMETYPE_host_name,(void *) ioBuff);
                if(rc == 0)
                {
                    MEHTTPFunc(SSL_free)(ssl);
                    if(logFunc != NULL)
                        logFunc((meUByte *) "meSockHttp Error: Failed to set TLS server-name indication",logData);
                    return -13;
                }
            }
#if 0
            // is used in sftp
            if (continue_session)
            {
                /* attempt to resume a previous SSL session */
                ctx = (struct openssl_transport_context *) fd_transport_context (*continue_session);
                if (!ctx || !ctx->sess || !SSL_set_session (ssl, ctx->sess))
                    goto error;
            }
#endif
        }
#endif
        
        if((sck=socket(AF_INET,SOCK_STREAM,(flags & meSOCK_USE_HTTPS) ? 0:IPPROTO_TCP)) == meBadSocket)
        {
            if(logFunc != NULL)
                logFunc((meUByte *) "meSockHttp Error: Failed to create socket",logData);
#if MEOPT_OPENSSL
            if(flags & meSOCK_USE_HTTPS)
                MEHTTPFunc(SSL_free)(ssl);
#endif
            return -14;
        }  
        if(timeoutSnd > 0)
        {
            /* using setsockopt to set the send timeout to given millisecs, alternatively use select(0,fd_set,NULL,NULL,struct timeval) */
            struct timeval to;      
            to.tv_sec = timeoutSnd;
            to.tv_usec = 0;
            if((ret=setsockopt(sck,SOL_SOCKET,SO_SNDTIMEO,(char *) &to,sizeof(to))) < 0)
            {
                if(sFp->flags & meSOCK_LOG_WARNING)
                {
                    snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSockHttp Warning: Failed to set send timeout %d,%d",ret,meSocketGetError());
                    logFunc(rbuff,logData);
                }
            }
        }
        if(timeoutRcv > 0)
        {
            struct timeval to;      
            to.tv_sec = timeoutRcv;
            to.tv_usec = 0;
            if((ret=setsockopt(sck,SOL_SOCKET,SO_RCVTIMEO,(char *) &to,sizeof(to))) < 0)
            {
                if(sFp->flags & meSOCK_LOG_WARNING)
                {
                    snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSockHttp Warning: Failed to set recv timeout %d,%d",ret,meSocketGetError());
                    logFunc(rbuff,logData);
                }
            }
        }
        sFp->sck = sck;
        
#if MEOPT_OPENSSL
        /*Bind the socket to the SSL structure*/
        if((flags & meSOCK_USE_HTTPS) && ((ret = MEHTTPFunc(SSL_set_fd)(ssl,sck)) < 1))
        {
            err = MEHTTPFunc(SSL_get_error)(ssl,ret);
            if(logFunc != NULL)
            {
                snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSockHttp Error: SSL_set_fd error %d,%d",ret,err);
                logFunc(rbuff,logData);
            }
            meSockGetSSLErrors(rbuff);
            meSockClose(sFp,1);
            return -15;
        }
#endif
        if(sFp->flags & meSOCK_LOG_VERBOSE)
            logFunc((meUByte *) "meSockHttp Debug: About to connect socket",logData);
        /* Connect to the server, TCP/IP layer,*/
        if((err=connect(sck,(struct sockaddr*) &sa,sizeof(sa))) != 0)
        {
            if(logFunc != NULL)
            {
                snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSockHttp Error: Socket returned error #%d",meSocketGetError());
                logFunc(rbuff,logData);
            }
            meSockClose(sFp,1);
            return -16;
        }
        
#if MEOPT_OPENSSL
        if(flags & meSOCK_USE_HTTPS)
        {
            /*Connect to the server, SSL layer.*/
            if((ret=MEHTTPFunc(SSL_connect)(ssl)) < 1)
            {
                err = MEHTTPFunc(SSL_get_error)(ssl,ret);
                if(logFunc != NULL)
                {
                    snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSockHttp Error: SSL_connect error %d,%d",ret,err);
                    logFunc(rbuff,logData);
                }
                meSockGetSSLErrors(rbuff);
                meSockClose(sFp,1);
                return -17;
            }
            
            /*Print out connection details*/
            if(sFp->flags & meSOCK_LOG_HEADERS)
            {
                snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSockHttp SSL connected on socket %x, version: %s, cipher: %s",sck,MEHTTPFunc(SSL_get_version)(ssl),
                         MEHTTPFunc(SSL_CIPHER_get_name)(MEHTTPFunc(SSL_get_current_cipher)(ssl)));
                logFunc(rbuff,logData);
            }
            if(!meSockCheckCertificate(sFp,(char *) ioBuff,rbuff))
            {
                meSockClose(sFp,1);
                return -18;
            }
        }
        else
#endif
            if(sFp->flags & meSOCK_LOG_HEADERS)
        {
            snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSockHttp connected on socket %x",sck);
            logFunc(rbuff,logData);
        }
        sFp->cprt = port;
        strcpy((char *) sFp->chst,(char *) host);
    }
    
    /*Send message to the server.*/
    ss = ioBuff;
    if(postFName != NULL)
    {
        if((pfp=fopen((char *) postFName,"rb")) == NULL)
        {
            if(logFunc != NULL)
            {
                snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSockHttp Error: Failed to open post file [%s]",postFName);
                logFunc(rbuff,logData);
            }
            meSockClose(sFp,1);
            return -19;
        }
        /* send POST message to http */
        strcpy((char *) ss,"POST ") ;
        ss += 5;
    }
    else if(fdLen > 0)
    {
        /* send POST message to http */
        strcpy((char *) ss,"POST ") ;
        ss += 5;
    }
    else
    {
        /* send GET message to http */
        strcpy((char *) ss,"GET ") ;
        ss += 4;
    }
    if(proxyHost == NULL)
    {
        strcpy((char *) ss,(char *) file);
        ss += strlen((char *) ss) ;
    }
    else
    {
        strcpy((char *) ss,"http") ;
        ss += 4;
        if(flags & meSOCK_USE_HTTPS)
            *ss++ = 's';
        if(port)
            ss += sprintf((char *) ss,"://%s:%d%s",host,port,file);
        else
            ss += sprintf((char *) ss,"://%s%s",host,file);
    }
    /* TODO send close if not REUSE? */
    ss += sprintf((char *) ss," HTTP/1.0\r\nConnection: keep-alive\r\nHost: %s",host);
    if(port)
        ss += sprintf((char *) ss,":%d",port);
    if(user != NULL)
    {
        /* password supplied, encode */
        strcpy((char *) ss,"\r\nAuthorization: Basic ") ;
        ss += 23 ;
        sprintf((char *) rbuff,"%s:%s",user,pass);
        ss = strBase64Encode(ss,rbuff) ;
    }
    if(pfp != NULL)
    {
        meInt ii, ll;
        meUByte *s1, *s2 ;
        fseek(pfp,0,SEEK_END);
        ii = ftell(pfp) ;
        fseek(pfp,0,SEEK_SET) ;
        if((s1 = (meUByte *) strrchr((char *) postFName,'/')) == NULL)
            s1 = postFName;
        if((s2 = (meUByte *) strrchr((char *) s1,'\\')) == NULL)
            s2 = s1;
        pfs = sprintf((char *) rbuff,"\r\n----5Iz6dTINmxNFw6S42Ryf98IBXX1NCe%x",(meUInt) clock());
        ll = sprintf((char *) rbuff+pfs,"\r\nContent-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\nContent-Type: application/octet-stream\r\n\r\n",s2);
        ss += sprintf((char *) ss,"\r\nContent-Length: %d",pfs-2+ll+ii+pfs+4);
    }
    else if(fdLen > 0)
    {
        ss += sprintf((char *) ss,"\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d",fdLen);
    }
    if(cookie != NULL)
    {
        if((cookie->value != NULL) && (cookie->value[0] != '\0'))
        {
            strcpy((char *) ss,"\r\nCookie: ");
            ss += 10;
            strcpy((char *) ss,(char *) cookie->value);
            ss += strlen((char *) ss);
        }
        if(cookie->buffLen < 0)
            cookie = NULL;
    }
    if(sFp->flags & meSOCK_LOG_HEADERS)
    {
        ss[0] = '\n';
        ss[1] = '\0';
        logFunc((meUByte *) "meSockHttp Request header:",logData);
        logFunc(ioBuff,logData);
    }
    strcpy((char *) ss,"\r\n\r\n");
    ss += 4;
#if MEOPT_OPENSSL
    if(flags & meSOCK_USE_HTTPS)
        ret = MEHTTPFunc(SSL_write)(ssl,ioBuff,(int) (ss-ioBuff));
    else
#endif
        ret = meSocketWrite(sck,(char *) ioBuff,(int) (ss-ioBuff),0);
    
    if(ret <= 0)
    {
#if MEOPT_OPENSSL
        if(flags & meSOCK_USE_HTTPS)
            err = MEHTTPFunc(SSL_get_error)(ssl,ret);
        else
#endif
            err = meSocketGetError();
        if(logFunc != NULL)
        {
            snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSockHttp Error: Write error %d,%d",ret,err);
            logFunc(rbuff,logData);
        }
#if MEOPT_OPENSSL
        if(flags & meSOCK_USE_HTTPS)
            meSockGetSSLErrors(rbuff);
#endif
        /* err == 6 means the server issued an SSL_shutdown. */
        if(err == 6)
            sFp->flags |= meSOCK_SHUTDOWN;
        meSockClose(sFp,1);
        if(pfp != NULL)
            fclose(pfp);
        return -20;
    }
    if(pfp != NULL)
    {
        meInt ii;
#if MEOPT_OPENSSL
        if(flags & meSOCK_USE_HTTPS)
            ret = MEHTTPFunc(SSL_write)(ssl,rbuff+2,strlen((char *) rbuff+2));
        else
#endif
            ret = meSocketWrite(sck,(char *) (rbuff+2),strlen((char *) rbuff+2),0);
        if(ret > 0)
        {
            while((ii=fread(ioBuff,1,ioBuffSz,pfp)) > 0)
            {
#if MEOPT_OPENSSL
                if(flags & meSOCK_USE_HTTPS)
                    ret = MEHTTPFunc(SSL_write)(ssl,ioBuff,ii);
                else
#endif
                    ret = meSocketWrite(sck,(char *) ioBuff,ii,0);
                if(ret <= 0)
                    break;
            }
            fclose(pfp);
            if(ret > 0)
            {
                strcpy((char *) rbuff+pfs,"--\r\n");
#if MEOPT_OPENSSL
                if(flags & meSOCK_USE_HTTPS)
                    ret = MEHTTPFunc(SSL_write)(ssl,rbuff,pfs+4);
                else
#endif
                    ret = meSocketWrite(sck,(char *) rbuff,pfs+4,0);
            }
        }
        else
            fclose(pfp);
        if(ret <= 0)
        {
#if MEOPT_OPENSSL
            if(flags & meSOCK_USE_HTTPS)
                err = MEHTTPFunc(SSL_get_error)(ssl,ret);
            else
#endif
                err = meSocketGetError();
            if(logFunc != NULL)
            {
                snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSockHttp Error: Post file write error %d,%d",ret,err);
                logFunc(rbuff,logData);
            }
#if MEOPT_OPENSSL
            if(flags & meSOCK_USE_HTTPS)
                meSockGetSSLErrors(rbuff);
#endif
            /* err == 6 means the server issued an SSL_shutdown. */
            if(err == 6)
                sFp->flags |= meSOCK_SHUTDOWN;
            meSockClose(sFp,1);
            return -22;
        }
    }
    else if(fdLen > 0)
    {
#if MEOPT_OPENSSL
        if(flags & meSOCK_USE_HTTPS)
            ret = MEHTTPFunc(SSL_write)(ssl,frmData,fdLen);
        else
#endif
            ret = meSocketWrite(sck,(char *) frmData,fdLen,0);
        if(ret <= 0)
        {
#if MEOPT_OPENSSL
            if(flags & meSOCK_USE_HTTPS)
                err = MEHTTPFunc(SSL_get_error)(ssl,ret);
            else
#endif
                err = meSocketGetError();
            if(logFunc != NULL)
            {
                snprintf((char *) rbuff,meSOCK_BUFF_SIZE,"meSockHttp Error: Form data write error %d,%d",ret,err);
                logFunc(rbuff,logData);
            }
#if MEOPT_OPENSSL
            if(flags & meSOCK_USE_HTTPS)
                meSockGetSSLErrors(rbuff);
#endif
            /* err == 6 means the server issued an SSL_shutdown. */
            if(err == 6)
                sFp->flags |= meSOCK_SHUTDOWN;
            meSockClose(sFp,1);
            return -23;
        }
    }
    /* must read header getting now ditch the header, read up to the first blank line */
    if(sFp->flags & meSOCK_LOG_HEADERS)
        logFunc((meUByte *) "meSockHttp Response header:",logData);
    sFp->flags |= meSOCK_SHUTDOWN;
    err = 1;
    while(meSockReadLine(sFp,ioBuff) > 0)
    {
        if(sFp->flags & meSOCK_LOG_HEADERS)
            logFunc(ioBuff,logData);
        if(ioBuff[0] == '\0')
            return err;
        if(((ioBuff[0]|0x20) == 'c') && ((ioBuff[1]|0x20) == 'o') && ((ioBuff[2]|0x20) == 'n'))
        {
            if(((ioBuff[3]|0x20) == 't') && ((ioBuff[4]|0x20) == 'e') && ((ioBuff[5]|0x20) == 'n') && ((ioBuff[6]|0x20) == 't') && (ioBuff[7] == '-') && ((ioBuff[8]|0x20) == 'l') &&
               ((ioBuff[9]|0x20) == 'e') && ((ioBuff[10]|0x20) == 'n') && ((ioBuff[11]|0x20) == 'g') && ((ioBuff[12]|0x20) == 't') && ((ioBuff[13]|0x20) == 'h') && (ioBuff[14] == ':'))
                sFp->length = atoi((char *) ioBuff+15) ;
            else if(((ioBuff[3]|0x20) == 'n') && ((ioBuff[4]|0x20) == 'e') && ((ioBuff[5]|0x20) == 'c') && ((ioBuff[6]|0x20) == 't') && ((ioBuff[7]|0x20) == 'i') && 
                    ((ioBuff[9]|0x20) == 'o') && ((ioBuff[10]|0x20) == 'n') && (ioBuff[11] == ':'))
            {
                char cc;
                ss = ioBuff+12 ;
                while(((cc=*ss) == ' ') || (cc == '\t'))
                    ss++;
                if((cc|0x20) == 'c')
                    /* Connection: close */
                    sFp->flags &= ~meSOCK_REUSE;
            }
        }
        else if(((ioBuff[0]|0x20) == 'l') && ((ioBuff[1]|0x20) == 'o') && ((ioBuff[2]|0x20) == 'c') && ((ioBuff[3]|0x20) == 'a') && ((ioBuff[4]|0x20) == 't') &&
                ((ioBuff[5]|0x20) == 'i') && ((ioBuff[6]|0x20) == 'o') && ((ioBuff[7]|0x20) == 'n') && (ioBuff[8] == ':'))
        {
            /* The requested file is not here, its at the given location */
            char cc;
            ss = ioBuff+9 ;
            while(((cc=*ss) == ' ') || (cc == '\t'))
                ss++;
            if(cc != '\0')
            {
                strcpy((char *) rbuff,(char *) ss);
                /* note: carry on reading to validate the rest of the header and get updated cookies etc. */
                err = 0;
            }
        }
        else if((cookie != NULL) && ((ioBuff[0]|0x20) == 's') && ((ioBuff[1]|0x20) == 'e') && ((ioBuff[2]|0x20) == 't') && (ioBuff[3] == '-') && ((ioBuff[4]|0x20) == 'c') && 
                ((ioBuff[5]|0x20) == 'o') && ((ioBuff[6]|0x20) == 'o') && ((ioBuff[7]|0x20) == 'k') && ((ioBuff[8]|0x20) == 'i') && ((ioBuff[9]|0x20) == 'e') && (ioBuff[10] == ':'))
        {
            meUByte *vv, *ee, *dd, *de, cc;
            int ll;
            ss = ioBuff+11;
            while(((cc=*ss) == ' ') || (cc == '\t'))
                ss++;
            if((cc != '\0') && (cc != '=') && ((vv = (meUByte *) strchr((char *) ss,'=')) != NULL) && ((ee = (meUByte *) strchr((char *) vv,';')) != NULL))
            {
                vv++;
                *ee = '\0';
                if((cookie->value != NULL) && (cookie->value[0] != '\0'))
                {
                    ll = vv - ss;
                    if(strncmp((char *) cookie->value,(char *) ss,ll))
                    {
                        cc = *vv;
                        *vv = '\0';
                        ss[-2] = ';';
                        ss[-1] = ' ';
                        if((dd=(meUByte *) strstr((char *) cookie->value,(char *) (ss-2))) != NULL)
                        {
                            if((de = (meUByte *) strchr((char *) (dd+ll+2),';')) != NULL)
                            {
                                while((*dd++ = *de++) != '\0')
                                    ;
                            }
                            else
                                *dd = '\0';
                        }  
                        *vv = cc;
                        de = cookie->value;
                    }
                    else if((de = (meUByte *) strchr((char *) (cookie->value+ll),';')) != NULL)
                        de += 2;
                }
                else
                    de = NULL;
                if(*vv != '\0')
                {
                    meUByte *ov=NULL;
                    ll = strlen((char *) ss)+1;
                    if(de != NULL)
                        ll += strlen((char *) de)+2;
                    if(ll > cookie->buffLen)
                    {
                        ov = cookie->value;
                        ll = ((ll + 256) & ~15);
                        if((cookie->value = (meUByte *) malloc(ll)) == NULL)
                        {
                            if(logFunc != NULL)
                                logFunc((meUByte *) "meSock Error: Cookie malloc failure",logData);
                            ll = 0;
                        }
                        cookie->buffLen = ll;
                    }
                    if((dd = cookie->value) != NULL)
                    {
                        if(de != NULL)
                        {
                            if(de == dd)
                            {
                                while(*dd++ != '\0')
                                    ;
                            }
                            else
                            {
                                while((*dd++ = *de++) != '\0')
                                    ;
                            }
                            dd[-1] = ';';
                            *dd++ = ' ';
                        }
                        while((*dd++ = *ss++) != '\0')
                            ;
                    }
                    if(ov != NULL)
                        free(ov);
                }
                else if(de == NULL)
                    cookie->value[0] = '\0';
                else if(de != cookie->value)
                {
                    dd = cookie->value;
                    while((*dd++ = *de++) != '\0')
                        ;
                }
            }
        }
    }
    sFp->length = 0;
    return err;

}

int
meSockRead(meSockFile *sFp, int sz, meUByte *buff, int offs)
{
    int err,ret;
    
#if MEOPT_OPENSSL
    if(sFp->flags & meSOCK_USE_HTTPS)
    {
        if((ret = MEHTTPFunc(SSL_read)(sFp->ssl,buff+offs,sz)) > 0)
            return ret;
        err = MEHTTPFunc(SSL_get_error)(sFp->ssl,ret);
        if((sFp->length == -1) && (err == SSL_ERROR_ZERO_RETURN))
        {
            /* Conent-lenth not returned and we've likely hit the correct end of the data - return 0 */ 
            if(sFp->flags & meSOCK_LOG_VERBOSE)
                logFunc((meUByte *) "meSock Debug: Got data stream end",logData);
            sFp->length = -2;
            return 0;
        }
        if((err == SSL_ERROR_SYSCALL) || (err == SSL_ERROR_SSL))
            sFp->flags &= ~meSOCK_SHUTDOWN;
    }
    else
#endif
    {
        if((ret = meSocketRead(sFp->sck,(char *) (buff+offs),sz,0)) > 0)
            return ret;
        err = meSocketGetError();
        if((sFp->length == -1) && (ret == 0) && (err == 0))
        {
            /* Conent-lenth not returned and we've likely hit the correct end of the data - return 0 */ 
            if(sFp->flags & meSOCK_LOG_VERBOSE)
                logFunc((meUByte *) "meSock Debug: Got data stream end",logData);
            sFp->length = -2;
            return 0;
        }
    }
    snprintf((char *) buff,meSOCK_BUFF_SIZE,"meSock Error: Read failure - %d,%d",ret,err);
    if(logFunc != NULL)
        logFunc(buff,logData);
#if MEOPT_OPENSSL
    if(sFp->flags & meSOCK_USE_HTTPS)
        meSockGetSSLErrors(buff);
#endif
    sFp->flags &= ~meSOCK_REUSE;
    return -1;
}

int
meSockClose(meSockFile *sFp, int force)
{
    if(meSockFileIsInUse(sFp))
    {
        if(!force && meSockFileCanReuse(sFp))
            return 1;
#if MEOPT_OPENSSL
        if(sFp->ssl != NULL)
        {
            if(sFp->flags & meSOCK_SHUTDOWN)
            {
                int err = MEHTTPFunc(SSL_shutdown)(sFp->ssl);
                if(err < 0)
                {
                    if(logFunc != NULL)
                        logFunc((meUByte *) "meSock Error: Error in shutdown",logData);
                }
                else if((err == 1) && (sFp->flags & meSOCK_LOG_VERBOSE))
                    logFunc((meUByte *) "meSock Debug: Client exited gracefully",logData);
            }
            MEHTTPFunc(SSL_free)(sFp->ssl);
            sFp->ssl = NULL;
        }
#endif
        if(sFp->sck != meBadSocket)
        {
            if(logFunc != NULL)
            {
                meUByte buff[256];
                sprintf((char *)buff,"[Closing socket %d]",sFp->sck);
                logFunc(buff,logData);
            }
            meSocketClose(sFp->sck);
            sFp->sck = meBadSocket;
        }
        sFp->flags = 0;
    }
    return 0;
}


void
meSockEnd()
{
#if MEOPT_OPENSSL
    if(meSockCtx != NULL)
    {
        MEHTTPFunc(SSL_CTX_free)(meSockCtx);
        meSockCtx = NULL;
    }
#endif
}
