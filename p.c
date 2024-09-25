#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[1024];

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    pid = fork();  // Create child process

    if (pid < 0) {
        printf("Fork failed.\n");
        exit(1);
    } else if (pid == 0) {  // Child process
        close(pipefd[0]);  // Close the reading end of the pipe

        // Write message to the pipe
        char child_message[] = "Hello from the child process!";
        write(pipefd[1], child_message, strlen(child_message) + 1);  // +1 to include the null terminator

        close(pipefd[1]);  // Close the writing end after writing
        exit(0);
    } else {  // Parent process
        close(pipefd[1]);  // Close the writing end of the pipe

        // Read data from the pipe
        read(pipefd[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);

        close(pipefd[0]);  // Close the reading end
        wait(NULL);  // Wait for the child to finish
    }

    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int pipefd[2];
    pid_t pid;

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    pid = fork();  // Create child process

    if (pid < 0) {
        printf("Fork failed.\n");
        exit(1);
    } else if (pid == 0) {  // Child process
        close(pipefd[0]);  // Close the reading end of the pipe

        // Write message to the pipe
        char child_message[] = "Data from the child process\n";
        write(pipefd[1], child_message, strlen(child_message) + 1);  // +1 for null terminator

        close(pipefd[1]);  // Close the writing end after writing
        exit(0);
    } else {  // Parent process
        close(pipefd[1]);  // Close the writing end of the pipe

        // Redirect the pipe's reading end to standard input (stdin)
        dup2(pipefd[0], STDIN_FILENO);  // Replace stdin with the pipe
        close(pipefd[0]);  // Close the original reading end

        // Execute the `cat` command, which will read from the pipe
        execlp("cat", "cat", NULL);

        // If exec fails, print an error
        perror("exec failed");
        exit(1);
    }

    return 0;
}
