#ifndef VU_H
#define VU_H

void vu_init();

void vu_tick();

void vu_display();

int primitive_to_buffer(char *buffer, unsigned int buffer_size, int primitive);

#endif
