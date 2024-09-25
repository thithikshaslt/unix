#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int num_children, pipefd[2];
    char buffer[1024];

    // Get number of child processes from the user
    printf("Enter the number of child processes to create: ");
    scanf("%d", &num_children);

    pipe(pipefd);  // Create a pipe

    for (int i = 0; i < num_children; i++) {
        pid_t pid = fork();  // Create child processes dynamically

        if (pid < 0) {
            printf("Fork failed.\n");
            exit(1);
        } else if (pid == 0) {  // Child process
            close(pipefd[0]);  // Close the reading end in the child
            char message[100];
            snprintf(message, sizeof(message), "Message from child %d (PID: %d)\n", i + 1, getpid());
            write(pipefd[1], message, sizeof(message));  // Write to pipe
            close(pipefd[1]);  // Close the writing end after writing
            exit(i + 1);  // Exit child with a unique status (for demo purposes)
        }
    }

    close(pipefd[1]);  // Close the writing end in the parent

    // Parent process
    for (int i = 0; i < num_children; i++) {
        int status;
        pid_t child_pid = wait(&status);  // Wait for each child to finish
        if (WIFEXITED(status)) {
            printf("Child PID: %d, Exit status: %d\n", child_pid, WEXITSTATUS(status));
        }
    }

    // Read from pipe
    read(pipefd[0], buffer, sizeof(buffer));  // Read all messages from children
    printf("Parent received from pipe:\n%s", buffer);
    close(pipefd[0]);  // Close the reading end

    return 0;
}
