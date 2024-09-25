#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void copy(const char *src, const char *dst) {
    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0) {
        perror("Cannot open source file");
        exit(1);
    }

    int dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd < 0) {
        perror("Cannot open destination file");
        close(src_fd);
        exit(1);
    }

    char buffer[1024];
    ssize_t bytes_read;
    
    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        write(dst_fd, buffer, bytes_read);
    }

    close(src_fd);
    close(dst_fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        exit(1);
    }

    copy(argv[1], argv[2]);
    return 0;
}
