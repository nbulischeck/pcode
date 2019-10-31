#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <signal.h>
#include <grp.h>

#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/syscall.h>

#include <linux/filter.h>
#include <linux/seccomp.h>
#include <linux/audit.h>

#define ArchField offsetof(struct seccomp_data, arch)

#define Allow(syscall) \
    BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_##syscall, 0, 1), \
    BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW)

int safety_first(void){
    struct sock_filter filter[] = {
        /* validate arch */
        BPF_STMT(BPF_LD+BPF_W+BPF_ABS, ArchField),
        BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, AUDIT_ARCH_X86_64, 1, 0),
        BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL),

        /* load syscall */
        BPF_STMT(BPF_LD+BPF_W+BPF_ABS, offsetof(struct seccomp_data, nr)),

        /* list of allowed syscalls */
        Allow(accept),
        Allow(access),
        Allow(arch_prctl),
        Allow(bind),
        Allow(brk),
        Allow(chown),
        Allow(chroot),
        Allow(clone),
        Allow(close),
        Allow(connect),
        Allow(dup),
        Allow(dup2),
        Allow(execve),
        Allow(exit_group),
        Allow(faccessat),
        Allow(futex),
        Allow(fcntl),
        Allow(fstat),
        Allow(getcwd),
        Allow(getuid),
        Allow(getgid),
        Allow(geteuid),
        Allow(getegid),
        Allow(getgroups),
        Allow(getpid),
        Allow(getppid),
        Allow(getpgrp),
        Allow(getrlimit),
        Allow(getpeername),
        Allow(getsockname),
        Allow(ioctl),
        Allow(listen),
        Allow(lseek),
        Allow(mmap),
        Allow(mprotect),
        Allow(munmap),
        Allow(open),
        Allow(openat),
        Allow(poll),
        Allow(prlimit64),
        Allow(pselect6),
        Allow(read),
        Allow(readlink),
        Allow(recvmsg),
        Allow(rename),
        Allow(rt_sigaction),
        Allow(rt_sigprocmask),
        Allow(select),
        Allow(sendto),
        Allow(setpgid),
        Allow(setregid),
        Allow(setreuid),
        Allow(setsockopt),
        Allow(set_tid_address),
        Allow(set_robust_list),
        Allow(socket),
        Allow(stat),
        Allow(statfs),
        Allow(sysinfo),
        Allow(uname),
        Allow(umask),
        Allow(wait4),
        Allow(write),

        /* and if we don't match above, die */
        BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_TRAP),
    };
    struct sock_fprog filterprog = {
        .len = sizeof(filter)/sizeof(filter[0]),
        .filter = filter
    };

    /* set up the restricted environment */
    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
        perror("Could not start seccomp:");
        exit(1);
    }
    if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &filterprog) == -1) {
        perror("Could not start seccomp:");
        exit(1);
    }
}

void handler(int num){
    printf("Bye!\n");
    exit(-1);
}

void setup(void){
    signal(SIGALRM, handler);
    alarm(5);

    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);

    safety_first();
}

int main(int argc, char **argv){
    setup();

    char buffer[0x100];
    printf("%p\n", buffer);
    gets(buffer);
}
