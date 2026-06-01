#include "../headers/writer.h"
#include "../headers/info.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int writer(FileList *list)
{
    size_t size;
    int fd, target;
    ssize_t bytes_read, bytes_written;
    size_t total_read = 0;
    size_t total_written = 0;

    size = list->file[list->count - 1].size;

    unsigned char *buffer = malloc(size);
    if (!buffer)
        return -1;

    fd = open("/dev/urandom", O_RDONLY);
    target = open(
        list->file[list->count - 1].path,
        O_WRONLY | O_TRUNC);

    if (fd < 0 || target < 0) {
        free(buffer);
        if (fd >= 0)
            close(fd);
        if (target >= 0)
            close(target);
        return -1;
    }

    // Read exactly size bytes
    while (total_read < size) {
        bytes_read = read(fd,
                          buffer + total_read,
                          size - total_read);

        if (bytes_read <= 0) {
            close(fd);
            close(target);
            free(buffer);
            return -1;
        }

        total_read += bytes_read;
    }

    close(fd);

    // Write exactly total_read bytes
    while (total_written < total_read) {
        bytes_written = write(target,
                              buffer + total_written,
                              total_read - total_written);

        if (bytes_written <= 0) {
            close(target);
            free(buffer);
            return -1;
        }

        total_written += bytes_written;
    }

    close(target);
    free(buffer);

    return (int)total_written;
}
