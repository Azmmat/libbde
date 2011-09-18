/*
 * AES encryption functions
 *
 * Copyright (C) 2011, Joachim Metz <jbmetz@users.sourceforge.net>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBBDE_AES_H )
#define _LIBBDE_AES_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( WINAPI )
#include <wincrypt.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
#include <openssl/aes.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/evp.h>

#endif

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( WINAPI ) && ( WINVER >= 0x0600 )
#define LIBBDE_HAVE_AES_SUPPORT

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
#define LIBBDE_HAVE_AES_SUPPORT

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#define LIBBDE_HAVE_AES_SUPPORT

#endif

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
#define LIBBDE_AES_CRYPT_MODE_DECRYPT	AES_DECRYPT
#define LIBBDE_AES_CRYPT_MODE_ENCRYPT	AES_ENCRYPT

#else
enum LIBBDE_AES_CRYPT_MODES
{
	LIBBDE_AES_CRYPT_MODE_DECRYPT	= 0,
	LIBBDE_AES_CRYPT_MODE_ENCRYPT	= 1
};

#endif

#if defined( WINAPI ) && ( WINVER >= 0x0600 )

typedef struct libbde_aes_key libbde_aes_key_t;

struct libbde_aes_key
{
	/* The public key structure (PUBLICKEYSTRUC) aka blob header (BLOBHEADER)
	 */
	PUBLICKEYSTRUC header;

	/* The (key) data size
	 */
	DWORD data_size;

	/* The (key) data
	 */
	BYTE data[ 32 ];
};

int libbde_aes_key_initialize(
     libbde_aes_key_t **key,
     liberror_error_t **error );

int libbde_aes_key_set(
     libbde_aes_key_t *key,
     const uint8_t *key_data,
     size_t bit_size,
     liberror_error_t **error );

int libbde_aes_key_free(
     libbde_aes_key_t **key,
     liberror_error_t **error );

#endif

typedef struct libbde_aes_context libbde_aes_context_t;

struct libbde_aes_context
{
#if defined( WINAPI ) && ( WINVER >= 0x0600 )
        HCRYPTPROV crypt_provider;
        HCRYPTKEY key;

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
        AES_KEY key;

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	EVP_CIPHER_CTX evp_context;
        uint8_t key[ 32 ];
	size_t bit_size;

#else
	/* The number of round keys
	 */
	uint8_t number_of_round_keys;

	/* The round keys
	 */
	uint32_t *round_keys;

	/* The round keys data
	 */
	uint32_t round_keys_data[ 68 ];

#endif
};

#if !defined( LIBBDE_HAVE_AES_SUPPORT )
int libbde_aes_initialize_tables(
     liberror_error_t **error );
#endif

int libbde_aes_initialize(
     libbde_aes_context_t **context,
     liberror_error_t **error );

int libbde_aes_free(
     libbde_aes_context_t **context,
     liberror_error_t **error );

int libbde_aes_set_decryption_key(
     libbde_aes_context_t *context,
     const uint8_t *key,
     size_t bit_size,
     liberror_error_t **error );

int libbde_aes_set_encryption_key(
     libbde_aes_context_t *context,
     const uint8_t *key,
     size_t bit_size,
     liberror_error_t **error );

int libbde_aes_cbc_crypt(
     libbde_aes_context_t *context,
     int mode,
     uint8_t initialization_vector[ 16 ],
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     liberror_error_t **error );

int libbde_aes_ccm_crypt(
     libbde_aes_context_t *context,
     int mode,
     const uint8_t *initialization_vector,
     size_t initialization_vector_size,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     liberror_error_t **error );

#ifdef TODO
int libbde_aes_cfb_crypt(
     libbde_aes_context_t *context,
     int mode,
     uint8_t initialization_vector[ 16 ],
     size_t *initialization_vector_index,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     liberror_error_t **error );
#endif

int libbde_aes_ecb_crypt(
     libbde_aes_context_t *context,
     int mode,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     liberror_error_t **error );

#ifdef __cplusplus
}
#endif

#endif
