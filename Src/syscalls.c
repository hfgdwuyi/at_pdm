#include <sys/stat.h>
#include <sys/types.h>

/* Minimal syscall stubs for newlib (bare-metal). */

int _close(int fd)        { (void)fd; return -1; }
int _fstat(int fd, struct stat *st) { (void)fd; (void)st; return -1; }
int _isatty(int fd)       { (void)fd; return 1; }
off_t _lseek(int fd, off_t offset, int whence) { (void)fd; (void)offset; (void)whence; return 0; }
int _read(int fd, char *buf, int count) { (void)fd; (void)buf; return count; }
void *_sbrk(int incr)     { (void)incr; return (void *)-1; }
int _write(int fd, const char *buf, int count) { (void)fd; (void)buf; return count; }
