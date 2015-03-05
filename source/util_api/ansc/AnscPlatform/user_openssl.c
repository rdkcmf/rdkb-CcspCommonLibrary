/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/

#include "user_openssl.h"
#include "pthread.h"
#include "openssl/crypto.h"
#include "ansc_platform.h"

#define  USER_OPENSSL_Redirect_AnscTrace            0
#if USER_OPENSSL_Redirect_AnscTrace
    #undef  AnscTraceWarning
    #undef  AnscTrace

    #define  AnscTraceWarning(msg)                  printf msg
    #define  AnscTrace                              printf
#endif

/* static SSL context, once initialized will stay!  */
static SSL_CTX *g_ssl_ctx[SSL_CTX_NUM];

static pthread_mutex_t *locks;
static int *lock_count;

static pthread_once_t openssl_is_initialized = PTHREAD_ONCE_INIT;
static void openssl_thread_setup(void);
void initialize_openssl_lib()
{
    openssl_thread_setup();
    RAND_load_file("/dev/urandom", 1024);

    if (RAND_status () != 1)
        AnscTraceWarning(("openssl_init - could not seed PRNG.\n"));

    SSL_library_init ();
    SSL_load_error_strings ();
    SSLeay_add_all_algorithms ();
    SSLeay_add_ssl_algorithms ();
}


static void locking_callback(int mode, int type, const char *file, int line)
{
        if (mode & CRYPTO_LOCK) {
                pthread_mutex_lock(&locks[type]);
                lock_count[type]++;
        } else {
                pthread_mutex_unlock(&locks[type]);
        }
}

static unsigned long id_callback(void)
{
	return (unsigned long)pthread_self();
}

static void openssl_thread_setup(void)
{
        int i, num = CRYPTO_num_locks();

        locks = (pthread_mutex_t*) OPENSSL_malloc(num * sizeof(pthread_mutex_t));
        lock_count = (int*) OPENSSL_malloc(num * sizeof(int));

        for (i = 0; i < num; i++) {
                lock_count[i] = 0;
                pthread_mutex_init(&locks[i], NULL);
        }

        CRYPTO_set_id_callback(id_callback);
        CRYPTO_set_locking_callback(locking_callback);
}

static void openssl_thread_cleanup(void)
{
        int i;

        CRYPTO_set_locking_callback(NULL);

        for (i = 0; i < CRYPTO_num_locks(); i++) {
                pthread_mutex_destroy(&locks[i]);
        }

        OPENSSL_free(locks);
        OPENSSL_free(lock_count);
}
static void
openssl_print_errors (void)
{
  unsigned long curerr = 0;
  while ((curerr = ERR_get_error ()) != 0)
    AnscTraceWarning(("OpenSSL: %s\n", ERR_error_string(curerr, NULL)));
}

/* 
 * product specific CA certificate file(s) is specified
 * through global variable openssl_client_ca_certificate_files/openssl_server_ca_certificate_files
 * and will be loaded during openssl_init(). When multiple
 * CA certificates are used, the file names must be comma-separated.
 */
char* openssl_client_ca_certificate_files = NULL;
char* openssl_server_ca_certificate_files = NULL;

int openssl_load_ca_certificates(int who_calls)
{
    char* pCACertFiles = (who_calls==SSL_CLIENT_CALLS)?openssl_client_ca_certificate_files:openssl_server_ca_certificate_files;
    char* pCACertFile;
    char* pSep;
    int   res;
    SSL_CTX *ssl_ctx = NULL;
  
    if (who_calls != SSL_SERVER_CALLS && who_calls != SSL_CLIENT_CALLS)
        return 0;

    ssl_ctx = g_ssl_ctx[who_calls];

    pCACertFiles = AnscCloneString(pCACertFiles);
    pCACertFile = pCACertFiles;

    while ( pCACertFile && *pCACertFile != 0 )
    {
        pSep = _ansc_strchr(pCACertFile, ',');
        if ( pSep ) *pSep = 0;
        
        res = SSL_CTX_load_verify_locations(ssl_ctx, pCACertFile, NULL);

        if ( !res )
        {
            printf("openssl_load_ca_certificates -- failed to load certificate, error code = %d!", res);
        }

        pCACertFile = pSep ? pSep + 1 : NULL;
    }

    if ( pCACertFiles ) AnscFreeMemory(pCACertFiles);

    return 0;
}


int openssl_init (int who_calls)
{
  int rc = 0;
  SSL_CTX *ssl_ctx = NULL;
  
  if (who_calls != SSL_SERVER_CALLS && who_calls != SSL_CLIENT_CALLS)
      return 0;

  ssl_ctx = g_ssl_ctx[who_calls];

  if (ssl_ctx)
    /* The SSL has already been initialized. */
    return 1;

  pthread_once(&openssl_is_initialized, initialize_openssl_lib);

  /* VZ only allows TLS */
  g_ssl_ctx[who_calls] = ssl_ctx = SSL_CTX_new (SSLv23_method ());
  if (!ssl_ctx)
    goto error;

  SSL_CTX_set_options(ssl_ctx,SSL_OP_NO_SSLv2);

  if (who_calls == SSL_CLIENT_CALLS)
  {
      openssl_priv_verify(ssl_ctx);
  }
  else 
  {
      /*server side, should customize per product*/

      /* see openssl_validate_certificate() for certificate validation  */
      /* if not using openssl_validate_certificate(), make sure to use SSL_VERIFY_PEER  */
      if (SSL_CTX_use_certificate_file (ssl_ctx, BBHM_CERT_FILE,
                                        SSL_FILETYPE_PEM)
          != 1)
          goto error;
      if (SSL_CTX_use_PrivateKey_file (ssl_ctx, BBHM_PRIVATE_KEY,
                                       SSL_FILETYPE_PEM)
          != 1)
          goto error;

      SSL_CTX_set_verify (ssl_ctx, SSL_VERIFY_NONE, NULL);

  }

  SSL_CTX_set_mode (ssl_ctx, SSL_MODE_ENABLE_PARTIAL_WRITE);
  SSL_CTX_set_mode (ssl_ctx, SSL_MODE_AUTO_RETRY);
  SSL_CTX_set_quiet_shutdown(ssl_ctx, 1);

  openssl_load_ca_certificates(who_calls);

  return 1;

 error:
  if (ssl_ctx)
    SSL_CTX_free (ssl_ctx);

  openssl_print_errors ();

  return 0;


}

int openssl_shutdown()
{
	/* not much to do here */
	openssl_thread_cleanup();
}

int openssl_read (int fd, char *buf, int bufsize, void *ctx)
{
  int ret;
  SSL *ssl = (SSL *) ctx;
  do
    ret = SSL_read (ssl, buf, bufsize);
  while (ret == -1
	 && SSL_get_error (ssl, ret) == SSL_ERROR_SYSCALL
	 && errno == EINTR);
  return ret;
}

int openssl_write (int fd, char *buf, int bufsize, void *ctx)
{
  int ret = 0;
  SSL *ssl = (SSL *) ctx;
  do
    ret = SSL_write (ssl, buf, bufsize);
  while (ret == -1
	 && SSL_get_error (ssl, ret) == SSL_ERROR_SYSCALL
	 && errno == EINTR);
  return ret;
}


/* set - 1 is read set, 2 is write set */
int openssl_select_fd (int fd, long maxtime, int set)
{
  fd_set fdset;
  fd_set *rd = NULL, *wr = NULL;
  struct timeval timeout;
  int result;

  FD_ZERO (&fdset);
  FD_SET (fd, &fdset);
  if (set == 1)
    rd = &fdset;
  if (set == 2)
    wr = &fdset;

  timeout.tv_sec = maxtime/1000000;
  timeout.tv_usec = maxtime - timeout.tv_sec*1000000;

  do
    result = select (fd + 1, rd, wr, NULL, &timeout);
  while (result < 0 && errno == EINTR);

  return result;
}

int openssl_poll (int fd, long timeout, int set, void *ctx)
{
  SSL *ssl = (SSL *) ctx;
  if (timeout == 0)
    return 1;
  if (SSL_pending (ssl))
    return 1;
  return openssl_select_fd (fd, timeout, set);
}


/* Returns SSL handle on success, NULL on failure.  */

SSL * openssl_connect (int fd)
{
  SSL *ssl = NULL;
  SSL_CTX *ssl_ctx = g_ssl_ctx[SSL_CLIENT_CALLS];

  if (ssl_ctx == NULL) {
	  goto error;
  }

  ssl = SSL_new (ssl_ctx);

  if (!ssl)
    goto error;

  if (!SSL_set_fd (ssl, fd))
    goto error;

  SSL_set_connect_state (ssl);

  if (SSL_connect (ssl) <= 0 || ssl->state != SSL_ST_OK)
    goto error;

  AnscTraceWarning(("openssl_connect - connected socket %d to SSL handle %p\n", fd, ssl));

  return ssl;

error:

  AnscTraceWarning(("openssl_connect - SSL handshake failed -- ssl state = %d.\n", ssl->state));

  openssl_print_errors ();

  if (ssl)
    SSL_free (ssl);

  return NULL;
}

SSL * openssl_accept (int conn_fd)
{
    SSL *ssl = NULL;
    SSL_CTX *ssl_ctx = g_ssl_ctx[SSL_SERVER_CALLS];    

    if (ssl_ctx == NULL) {
        goto error;
    }

    ssl = SSL_new( ssl_ctx );

    if (!ssl)
        goto error;

    if (!SSL_set_fd (ssl, conn_fd))
        goto error;

    if ( SSL_accept( ssl ) <=0 )
        goto error;

    AnscTraceWarning(("openssl_accept - connected socket %d to SSL handle %p\n", conn_fd, ssl));

    return ssl;

error:
    AnscTraceWarning(("openssl_accept - SSL handshake failed.\n"));

    openssl_print_errors ();

    if (ssl)
        SSL_free (ssl);

    return NULL;
}

int _client_openssl_validate_certificate (int fd,  char *host, SSL *ssl)
{
    X509 *cert;
    char common_name[256] = {0};
    long vresult;
    int success = 1;

    if (!ssl)
    {
        AnscTraceWarning(("%s - ssl handle is null!\n", __FUNCTION__));
        return 0;
    }

    cert = SSL_get_peer_certificate (ssl);
    if (!cert)
    {
        AnscTraceWarning(("%s - no certificate returned %s.\n", __FUNCTION__ , host));
        success = 0;
        goto no_cert;
    }

    if (0)
    {
        char *subject = X509_NAME_oneline (X509_get_subject_name (cert), 0, 0);
        char *issuer = X509_NAME_oneline (X509_get_issuer_name (cert), 0, 0);
        AnscTraceWarning(("%s - certificate:\n  subject: %s\n  issuer:  %s\n",
                   __FUNCTION__,
                   subject, issuer));
        OPENSSL_free (subject);
        OPENSSL_free (issuer);
    }

    vresult = SSL_get_verify_result (ssl);
    if (vresult != X509_V_OK )
    {
        AnscTraceWarning(("_client_openssl_validate_certificate - %s - certificate verification error for host [%s]: %s\n",
                  __FUNCTION__,
                  host,
                  X509_verify_cert_error_string (vresult)));

        success = 0;

#ifdef _ALLOW_SELF_SIGNED_CERT
        if ( vresult == X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN )
            success = 1;

#endif
    }

    if ( success ) {
        common_name[0] = '\0';
        X509_NAME_get_text_by_NID (X509_get_subject_name (cert),
                                   NID_commonName, common_name, sizeof (common_name));
        AnscTraceWarning(("_client_openssl_validate_certificate - %s - certificate common name [%s]\n", __FUNCTION__, common_name));

		openssl_priv_validate_hostname(common_name);
    }

    if (success)
        AnscTraceWarning(("%s - X509 certificate successfully verified on host %s\n", __FUNCTION__,
                   host));

    X509_free (cert);

no_cert:

    return success;
}

int _server_openssl_validate_certificate (int fd, char *data, SSL *ssl)
{
/*server verify client's certificate logic. 
 the logic is different per product, need customization*/
    return 1;
}

/* certificate validation -- 1: success, 0- fail. note self-certificate is allowed  */
int openssl_validate_certificate (int fd, char * data, SSL *ssl, int who_calls)
{
    if (who_calls == SSL_CLIENT_CALLS)
        return _client_openssl_validate_certificate (fd, data, ssl);
    else
        return _server_openssl_validate_certificate (fd, data, ssl);
}


