#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int num_children;

    // Ask the user for the number of children to create
    printf("Enter the number of child processes to create: ");
    scanf("%d", &num_children);

    pid_t pids[num_children];

    // Create multiple child processes
    for (int i = 0; i < num_children; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            // Fork failed
            printf("Fork failed for child %d\n", i + 1);
            exit(1);
        } else if (pids[i] == 0) {
            // Child process
            printf("Child %d (PID: %d) created.\n", i + 1, getpid());
            // Simulate some work with sleep and exit with a unique status
            sleep(1);  // Child does some work (e.g., sleep for a while)
            exit(i + 1);  // Exit with a status representing the child number
        }
    }

    // Parent process: Wait for all child processes to finish and get their exit status
    for (int i = 0; i < num_children; i++) {
        int status;
        pid_t child_pid = wait(&status);  // Wait for a child to finish

        if (WIFEXITED(status)) {
            // Print the PID and exit status of the child
            printf("Child with PID %d exited with status %d\n", child_pid, WEXITSTATUS(status));
        } else {
            printf("Child with PID %d did not exit normally.\n", child_pid);
        }
    }

    return 0;
}
