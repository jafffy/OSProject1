#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

int set_semvalue(void);
void del_semvalue(void);
int semaphore_p(void);
int semaphore_v(void);

extern int sem_id;

#endif // SEMAPHORE_H_
