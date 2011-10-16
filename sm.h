#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#define SHM_SIZE 1024

struct sharedData {
	int id;
    int passageiros;
    int margem;
};
key_t generateKey();
int getIdSM();
void* attachSM();
int creatorSM();
