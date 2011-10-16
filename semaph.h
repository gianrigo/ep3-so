int createSPH(int);
int getIdSPH();
int getValSPH(int,int);
void setValSPH(int,int,int);
void waitSPH(int,int);
void waitForAllSPH(int,int);
/*int waitTimedSPH(int,int,struct timespec*);*/
int waitForAllTimedSPH(int,int);
void incValSPH(int,int);
void decValSPH(int,int);
void removeSPH(int semid);
