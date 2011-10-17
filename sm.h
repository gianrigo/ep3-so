#define SHM_SIZE 1024

key_t generateKey();
int getIdSM();
void* attachSM();
int creatorSM();
int getPassengersSM();
void setPassengersSM();
void decPassengersSM();
void removeSM();
