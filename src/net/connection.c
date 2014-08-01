#include "connection.h"

static buffer_t*
__buffer_new(unsigned int size)
{
	buffer_t *b;

	b = (buffer_t*)malloc(sizeof *b);

	if (b == NULL) return b;

	b->sz = size;
	b->idx = 0;
	b->data = (char*)calloc(1, size);

	return b;
}

connection_t*
connection_new(int fd, unsigned int bs)
{
	connection_t *c;

	c = (conncection_t*)malloc(sizeof *c);

	if (c == NULL) return c;

	c->fd = fd;
	c->event = EPOLLIN | EPOLLET;
	c->rh = NULL;
	c->wh = NULL;
	c->stat = connected;
	c->wb = __buffer_new(bs);
	c->rb = __buffer_new(bs);
}



