#ifndef EXT_IO_H
#define EXT_IO_H

#include <stdint.h>

void ext_ic_check(void);
uint8_t ext_io_init(void);

enum ext_io_dir_e{
    EXT_IO_DIR_INPUT = 0,
    EXT_IO_DIR_OUTPUT = 1,
};

#endif // EXT_IO_H
