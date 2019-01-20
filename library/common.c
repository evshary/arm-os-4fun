void memcpy(void *dst, const void *src, unsigned int num)
{
    unsigned int i;
    for (i = 0; i < num; i++) {
        ((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
    }
}

void memset(void *ptr, int value, unsigned int num)
{
    unsigned int i;
    for (i = 0; i < num; i++) {
        ((unsigned char *)ptr)[i] = (unsigned char)value;
    }
}
