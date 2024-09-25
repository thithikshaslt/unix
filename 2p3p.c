#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipe1[2], pipe2[2];
    pid_t pid1, pid2, pid3;

    // Create Pipe 1
    if (pipe(pipe1) == -1) {
        perror("Pipe 1 failed");
        exit(1);
    }

    // Create Pipe 2
    if (pipe(pipe2) == -1) {
        perror("Pipe 2 failed");
        exit(1);
    }

    // Create first child process (Process 1)
    if ((pid1 = fork()) == -1) {
        perror("Fork failed for Process 1");
        exit(1);
    }

    if (pid1 == 0) {  // Process 1: Write to Pipe 1
        close(pipe1[0]);  // Close reading end of Pipe 1
        char message[] = "Hello from Process 1!\n";
        write(pipe1[1], message, strlen(message));  // Write to Pipe 1
        close(pipe1[1]);  // Close writing end after writing
        exit(0);
    }

    // Create second child process (Process 2)
    if ((pid2 = fork()) == -1) {
        perror("Fork failed for Process 2");
        exit(1);
    }

    if (pid2 == 0) {  // Process 2: Read from Pipe 1, Write to Pipe 2
        close(pipe1[1]);  // Close writing end of Pipe 1
        close(pipe2[0]);  // Close reading end of Pipe 2

        char buffer[1024];
        read(pipe1[0], buffer, sizeof(buffer));  // Read from Pipe 1
        close(pipe1[0]);  // Close reading end after reading

        strcat(buffer, "Modified by Process 2\n");  // Modify the message
        write(pipe2[1], buffer, strlen(buffer));  // Write to Pipe 2
        close(pipe2[1]);  // Close writing end after writing
        exit(0);
    }

    // Create third child process (Process 3)
    if ((pid3 = fork()) == -1) {
        perror("Fork failed for Process 3");
        exit(1);
    }

    if (pid3 == 0) {  // Process 3: Read from Pipe 2 and Display
        close(pipe2[1]);  // Close writing end of Pipe 2

        char buffer[1024];
        read(pipe2[0], buffer, sizeof(buffer));  // Read from Pipe 2
        close(pipe2[0]);  // Close reading end after reading

        printf("Process 3 received: %s", buffer);  // Display final output
        exit(0);
    }

    // Parent process waits for all children to finish
    close(pipe1[0]);  // Close unused ends in parent
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    wait(NULL);  // Wait for Process 1 to finish
    wait(NULL);  // Wait for Process 2 to finish
    wait(NULL);  // Wait for Process 3 to finish

    return 0;
}
