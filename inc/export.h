#ifndef __EXPORT_H_
#define __EXPORT_H_

#define MYSQL_RESULT "g_t_mysql_result" /* mysql 结果查询记录 */

extern int 
connect_mysql(const char* host, 
			  const char* user, 
			  const char* password, 
			  const char* dbname, 
			  unsigned int port);
				  
extern int 
query(const char* mysql);

extern void
debug(const char* msg);

extern void
info(const char* mgs);

extern void
error(const char* msg);

#endif
