#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Cannot open directory");
        exit(1);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *path = (argc > 1) ? argv[1] : ".";  // Use current directory if no argument
    list_directory(path);
    return 0;
}
