#ifndef SQL_DOOM
#define SQL_DOOM

#include "sql_class.h"
#include "sql_list.h"

#define DOOM_SQL_LEN  2048
#define DOOM_COMMENT_LEN  2048

bool doom_init_result(THD *thd);

bool doom_add_result(THD *thd, const char* sql, int result, const char * comment);

void doom_end_result(THD *thd);

#endif
