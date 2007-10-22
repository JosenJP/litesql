#ifndef _litesql_string_h_
#define _litesql_string_h_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned char* data;
} lsqlString;

int lsqlStringNew(lsqlString* s);

int lsqlStringCopy(lsqlString* dst, const char* src);

int lsqlStringCat(lsqlString* dst, lsqlString* src);

size_t lsqlStringSize(lsqlString* dst);

void lsqlStringDelete(lsqlString* s);

#ifdef __cplusplus
} // extern "C"
#endif

#endif

