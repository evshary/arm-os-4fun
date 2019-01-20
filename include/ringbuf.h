#ifndef __RINGBUF_H__
#define __RINGBUF_H__

#define MAX_BUF 1024

typedef struct {
    char buf[MAX_BUF];
    int start;
    int end;
} RING_BUF;

void ringbuf_init(RING_BUF *ring_buf);
int ringbuf_len(RING_BUF *ring_buf);
int ringbuf_write(RING_BUF *ring_buf, char data);
int ringbuf_read(RING_BUF *ring_buf, char *data);

#endif /*__RINGBUF_H__*/
