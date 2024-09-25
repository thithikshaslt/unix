#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void cat(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Cannot open file");
        exit(1);
    }

    char buffer[1024];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);  // Write to stdout
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    cat(argv[1]);  // Call the cat function
    return 0;
}
