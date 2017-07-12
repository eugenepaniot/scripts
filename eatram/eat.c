#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/resource.h>
#include <stdio.h>

void print_mem_usage () {
    struct rusage r_usage;

    getrusage(RUSAGE_SELF,&r_usage);

    printf("\tMemory usage: \n");
    printf("\t\t maximum resident set size: %ld, \n", r_usage.ru_maxrss);
    
    printf("\t\t integral shared memory size: %ld, \n", r_usage.ru_ixrss);
    printf("\t\t integral unshared data size: %ld, \n", r_usage.ru_idrss);
    printf("\t\t integral unshared stack size: %ld, \n", r_usage.ru_isrss);

    printf("\t\t page reclaims (soft page faults): %ld, \n", r_usage.ru_minflt);
    printf("\t\t page faults (hard page faults): %ld, \n", r_usage.ru_majflt);

    printf("\t\t voluntary context switches: %ld, \n", r_usage.ru_nvcsw);
    printf("\t\t involuntary context switches: %ld, \n", r_usage.ru_nivcsw);

    fflush(stdout);
}

int main(int argc, char** argv) {
    int max = -1;
    int mb = 1;
    char* buffer;

    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    sigset_t set;
    
    if(argc > 1)
        max = atoi(argv[1]);

    while((buffer=malloc(1024*1024)) != NULL && mb != max) {
        memset(buffer, 0, 1024*1024);
        mb++;
        
        printf("Allocated %d MB \n", mb);
        print_mem_usage();
    }

    sigsuspend(&set);

    return 0;
}
