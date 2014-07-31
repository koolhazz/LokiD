#ifndef __HEADER_H_
#define __HEADER_H_

#include <stdint.h>

#ifndef HEADER_SIZE 
#error "not defined HEAD_SIZE"
#endif

#pragma pack(push, 1)

#if HEADER_SIZE == 9
typedef struct header_s header_t;
struct header_s {
	unsigned short 	len;
	char 			mark[2];
	char 			main;
	char 			sub;
	unsigned short 	cmd;
	char 			code;	
};
#elif HEADER_SIZE == 14

#endif

#pragma pack(pop)

typedef struct message_s message_t;
struct message_s {
	header_t 		*h;
	char 			*data;
	unsigned int 	len;
	unsigned int 	idx;
};

#define WRITE(type, ptype, param) 	\
	ptype message_write_##type(message_t* m, ptype param)
#define READ(type) \
	type message_read_##type(message_t* m)  

message_t*
message_new(char* buff, unsigned int len);

int
message_write_hdr(const header_t* h, message_t* m);

int
message_free(message_t* m);

int
message_write_uint64(message_t* m, uint64_t u);

int
message_write_double(message_t* m, double d);

int
message_write_uint8(message_t* m, uint8_t u);

int
message_write_uint16(message_t* m, uint16_t u);

int
message_write_uint32(message_t* m, uint32_t u);

int
message_write_string(message_t* m, const char* s, unsigned int len);

uint8_t
message_read_uint8(message_t* m);

uint16_t
message_read_uint16(message_t* m);

uint32_t
message_read_uint32(message_t* m);

uint64_t
message_read_uint64(message_t* m);

double
message_read_double(message_t* m);

const char*
message_read_string(message_t* m);

int 
encode(message_t* m);

int
decode(message_t* m);

int
is_header(const char* data);

int
is_body(const char* data);


#endif
