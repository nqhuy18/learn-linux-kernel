#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

#define FILE_NAME "counter.txt"
#define NUM 5
pid_t child_pid;
pid_t parent_pid;
volatile int ready = 0;

void handler(int sif) {
    ready = 1;
}
int read_last_counter() {
    FILE *f = fopen(FILE_NAME, "r");
    int last = 0, tmp;
    if (f == NULL) return 0;
    
    while (fscanf(f, "%*[^0-9]%d", &tmp) == 1) last = tmp;
    fclose(f);
    return last;
}

void write_counter(int pid, int counter) {
    FILE *f = fopen(FILE_NAME, "a");
    if (!f) {
        perror("fopen");
        exit(1);
    }
    fprintf(f, "PID=%d Counter=%d\n", pid, counter);
    fclose(f);
}
int main() {
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    
    parent_pid = getpid();
    child_pid = fork();

    if (child_pid < 0) {
        perror("fork");
        exit(1);
    }
    if (child_pid == 0) {
             // --- PROCESS CHILD ---
            for (int i = 0; i < NUM; i++) {
                while(!ready) pause();
                int  c = read_last_counter();
                c++;
                write_counter(getpid(), c);
                kill(parent_pid, SIGUSR1);
                ready = 0;
            }
                exit(0);
    }
    else {
        // --- PROCESS PARENT ---
        int c = 0;
        for (int i = 0; i < NUM; i++) {
            c = read_last_counter();
            c++;
            write_counter(getpid(), c);

            kill(child_pid, SIGUSR2);
            ready = 0;
            pause(); 
        }
    }
    return 0;
}
