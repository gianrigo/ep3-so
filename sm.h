#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#define SHM_SIZE 1024

key_t generateKey();
int getIdSM();
void* attachSM();
int creatorSM();
int getPassengersSM();
void setPassengersSM();
void decPassengersSM();
void removeSM();
