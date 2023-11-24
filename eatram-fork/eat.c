#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

const int alloc_chunk = 1024 * 1024;

void handle_sigint(int signum) {
    printf("Ctrl+C exiting.\n");
    exit(0);
}

int main( int argc, char *argv[] )
{
    char *buffer = malloc(0);
    unsigned long long max = -1;
    unsigned long long mb = 0;
    pid_t *p;
    int i;
    int max_proc = 1;
    int status;

    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    if(argc > 1)
        max = atoll(argv[1]);

    if(argc > 2)
        max_proc = atoi(argv[2]);

    p = (pid_t *)malloc(sizeof(pid_t) * max_proc);
    if (p == NULL) {
        perror("malloc failed");
        return 255;
    }

    signal(SIGINT, handle_sigint);

    for (i = 0; i < max_proc; i++) {
      p[i] = fork();

      if(p[i]<0) {
        perror("fork failed");
        exit(255);
      } else if (p[i] == 0) {
        pid_t pid = getpid();

        while(mb <= max) {
          mb++;
          buffer = realloc(buffer, alloc_chunk * mb);
          if (buffer == NULL) {
            perror("realloc failed");
            break;
          }

          memset(buffer, 0, alloc_chunk * mb);
          printf("%d(%d) allocated %llu MB \n", pid, i, mb);
        }

        exit(0);
      }
    }

    for (i = 0; i < max_proc; i++) {
        waitpid(p[i], &status, 0);
    }

    free(buffer);
    free(p);
    return 0;
}
