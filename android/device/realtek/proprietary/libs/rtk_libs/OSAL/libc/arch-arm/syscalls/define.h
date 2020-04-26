#if defined(__LINARO_SDK__)
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#else
#define __NR_SYSCALL_BASE 0
#define __NR_semctl                       (__NR_SYSCALL_BASE + 300)
#define __NR_semget                       (__NR_SYSCALL_BASE + 299)
#define __NR_semop                        (__NR_SYSCALL_BASE + 298)
#define __NR_shmat                        (__NR_SYSCALL_BASE + 305)
#define __NR_shmctl                       (__NR_SYSCALL_BASE + 308)
#define __NR_shmdt                        (__NR_SYSCALL_BASE + 306)
#define __NR_shmget                       (__NR_SYSCALL_BASE + 307)
#define __NR_msgctl                       (__NR_SYSCALL_BASE + 304)
#define __NR_msgget                       (__NR_SYSCALL_BASE + 303)
#define __NR_msgrcv                       (__NR_SYSCALL_BASE + 302)
#define __NR_msgsnd                       (__NR_SYSCALL_BASE + 301)
#endif

