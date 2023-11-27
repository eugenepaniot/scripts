#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


const int alloc_chunk = 1 * 1024 * 1024;

void handle_sigint(int signum) {
    printf("Ctrl+C exiting.\n");
    exit(EXIT_SUCCESS);
}

int main( int argc, char *argv[] )
{
    unsigned long long max_mem = -1;
    unsigned long long mb = 0;
    pid_t *p;
    int i;
    int max_proc = 1;
    int status;

    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    if(argc > 1)
        max_mem = atoll(argv[1]);

    if(argc > 2)
        max_proc = atoi(argv[2]);

    p = (pid_t *)malloc(sizeof(pid_t) * max_proc);
    if (p == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, handle_sigint);

    for (i = 0; i < max_proc; i++) {
      p[i] = fork();

      if(p[i]<0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
      }

      if (p[i] == 0) {
        pid_t pid = getpid();

        while(mb < max_mem) {
          mb++;
          char *memory = (char *)malloc(alloc_chunk * mb);
          if (memory == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
          }
          memset(memory, 0, alloc_chunk * mb);
          free(memory);
        }

        printf("%d(%d) completed allocation of %llu MB\n", pid, i, mb);
        sleep(10);

        exit(EXIT_SUCCESS);
      }

      sleep(0.1);
    }

    for (i = 0; i < max_proc; i++) {
        waitpid(p[i], &status, 0);
    }

    printf("All processes completed.\n");

    free(p);
    return 0;
}
