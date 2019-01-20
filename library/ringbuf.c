#include "ringbuf.h"

#define RINGBUF_FULL(ring_buf) ring_buf->start != (ring_buf->end+1) % MAX_BUF
#define RINGBUF_EMPTY(ring_buf) ring_buf->start == ring_buf->end

void ringbuf_init(RING_BUF *ring_buf)
{
    ring_buf->start = 0;
    ring_buf->end = 0;
}

int ringbuf_len(RING_BUF *ring_buf)
{
    if (ring_buf->end > ring_buf->start) {
        return (ring_buf->end - ring_buf->start);
    } else if (ring_buf->start > ring_buf->end) {
        return (ring_buf->end + MAX_BUF - ring_buf->start);
    } else {
        return 0;
    }
}

int ringbuf_write(RING_BUF *ring_buf, char data)
{
    if (RINGBUF_FULL(ring_buf)) {
        ring_buf->buf[ring_buf->end] = data;
        ring_buf->end = (ring_buf->end + 1) % MAX_BUF;
        return 0;
    } else {
        return -1;
    }
}

int ringbuf_read(RING_BUF *ring_buf, char *data)
{
    if (RINGBUF_EMPTY(ring_buf)) return -1;

    *data = ring_buf->buf[ring_buf->start];
    ring_buf->start = (ring_buf->start + 1) % MAX_BUF;
    return 0;
}
