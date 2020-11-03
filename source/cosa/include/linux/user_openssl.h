/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

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

#ifndef _openssl_h
#define _openssl_h


#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <errno.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#ifdef HAVE_STRING_H
# include <string.h>
#else
# include <strings.h>
#endif

#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <stdbool.h>

#define BBHM_CA_CERT_FILE		"/bbhm/config/acs-ca.pem"
#define BBHM_CA_DIRECTORY	 	"/bbhm/config/"
#define BBHM_CERT_FILE  		"/bbhm/config/bbhm-chain.pem"
#define BBHM_PRIVATE_KEY 		"/bbhm/config/bbhm-key.pem"

typedef struct _hostNames
{
	bool peerVerify;
	char **hostNames;
	int numHosts;
}hostNames;

int openssl_init (int who_calls);

int openssl_read (int fd, char *buf, int bufsize, void *ctx);

int openssl_write (int fd, char *buf, int bufsize, void *ctx);

int openssl_select_fd (int fd, long maxtime, int wait_for);

int openssl_poll (int fd, long timeout, int wait_for, void *ctx);

int openssl_peek (int fd, char *buf, int bufsize, void *ctx);

void openssl_close (int fd, void *ctx);

SSL * openssl_connect (int fd, hostNames *hosts);

SSL * openssl_accept (int conn_fd);

enum
{
    SSL_SERVER_CALLS,  /*act as SSL server*/
    SSL_CLIENT_CALLS,  /*act as SSL client*/
    SSL_CTX_NUM
};
    
int openssl_validate_certificate (int fd, char * data, SSL *ssl, int who_calls, bool isSecure);

/* Platform/Board Specific Calls */
void openssl_priv_verify(SSL_CTX *ssl_ctx);
void openssl_priv_validate_hostname(char *common_name);

#ifdef __cplusplus
}
#endif

#endif
