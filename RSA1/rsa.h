#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <assert.h>
#include <errno.h>

typedef struct
{
  mpz_t e;
  mpz_t n;
} OKey_t;

typedef struct
{
  mpz_t d;
  mpz_t n;
} CKey_t;

typedef int errno_t;

enum Flags {
  Generation_f,
  Encryption_f,
  Decryption_f
};

errno_t find_d(mpz_t* d, const mpz_t f_e, const mpz_t e);
void func_eul(mpz_t* f_e, mpz_t p1, mpz_t p2);
void init_n(mpz_t* n, const mpz_t p1, const mpz_t p2);
void init_OKey(OKey_t* open_key);
void destr_OKey(OKey_t* open_key);
void init_CKey(CKey_t* close_key);
void destr_CKey(CKey_t* close_key);
errno_t gen_keys(OKey_t* open_key, CKey_t* close_key, FILE* fdata);
errno_t read_open_key(OKey_t* open_key, FILE* fokey);
errno_t read_close_key(CKey_t* close_key, FILE* fckey);
errno_t encrypt(char* origin, const OKey_t open_key, FILE* fout);
size_t decrypt(FILE* fin, const CKey_t close_key, char* to_orig, const size_t to_len);
errno_t parser_flags(const int argc, char* argv[], int* flag, char* ffrom_name, char* fto_name);
errno_t run(const int flag, FILE* ffrom, FILE* fto);

extern const int MaxFileName;
