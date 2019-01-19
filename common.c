void memcpy(void *dst, const void *src, unsigned int num)
{
    unsigned int i;
    for (i = 0; i < num; i++) {
        ((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
    }
}
