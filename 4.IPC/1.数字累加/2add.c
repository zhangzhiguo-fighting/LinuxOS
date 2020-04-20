






#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>

struct Shm {
	int now;
	int sum;
	pthread_mutex_t mutex;	
};

struct Shm *share_memory = NULL;

void do_add(int max, int x) {
    while (1) {
        pthread_mutex_lock(&share_memory->mutex);
        printf("<%d> : %d %d\n", x, share_memory->now, share_memory->sum);
        share_memory->sum += share_memory->now;
        share_memory->now++;
        if (share_memory->now > max) break;
        pthread_mutex_initutex_t
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s max ins\n", argv[0]);
        exit(1);
    }
	
    int max, ins;
    max = atoi(argv[1]);
    ins = atoi(argv[2]);
    pid_t pid;
    int shmid, a = 123;
    key_t key = ftok(".", 198);

    if ((shmid = shmget(key, sizeof(struct Shm), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        return 1;
    }

    if ((share_memory = (struct Shm *)shmat(shmid, NULL, 0)) == NULL) {
        perror("shmat");
        return 1;
    }
    memset(share_memory, 0, sizeof(struct Shm));

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED); 
    
    pthread_mutex_init(&share_memory->mutex, &attr);
    
    int x;
    //pid_t pid;

	for (int i = 0; i < ins; i++) { //父进程循环创建
        	if ((pid = fork()) < 0) {
            	perror("fork");
            	return 1;
        	}
        	if (pid == 0) {
            	x = i;
           		break;//每个子进程不再去循环创建子进程
        	}
    }
    
    if (pid == 0) {
        do_add(max, x);
        printf("<%d> exit!\n", x);
        exit(0);
    }
    while(ins) { //每个子进程都需要等
        wait(NULL);
        ins--;
    }
    //printf("read: %d\n", get_num(&num));
    get_num(&num);
    printf("Ans = %d\n", num.sum);

    return 0;
}

