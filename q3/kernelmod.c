// references: 
// 1. The Linux Kernel Module Programming Guide, Peter Jay Salzman, Michael Burian, Ori Pomerantz, Bob Mottram, Jim Huang
// 2. https://stackoverflow.com/questions/8547332/efficient-way-to-find-task-struct-by-pid
// 3. https://github.com/torvalds/linux/blob/master/include/linux/sched.h

// #define _XOPEN_SOURCE_EXTENDED 1
// #include <unistd.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pid.h>
#include <linux/pid_namespace.h>
#include <linux/cred.h>

static int pidInt;
static struct pid *pidStruct;
struct task_struct *curTask;

module_param(procpid, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

int init_module(void) {
    pr_info("Hello world\n");

    structpid = find_get_pid(pidInt);
    if (structpid==NULL) {
        pr_info("Invalid PID\n");
        return 0;
    }

    curTask = pid_task(pid_struct, PIDTYPE_PID);
    if (curTask==NULL) {
        pr_info("Unexpected error occured.\n");
        return 0;
    }

    pr_info("PID: %d\n", curTask->pid);
    pr_info("User ID: %d\n", curTask->cred->uid);
    pr_info("Process group ID: %d\n", pid_vnr(task_pgrp(curTask))); //task_tgid_nr(current)
    pr_info("Process command path: %s\n", curTask->comm);

    return 0;
}

MODULE_LICENSE("GPL");