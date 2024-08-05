#ifndef KERNEL_AGENT_H
#define KERNEL_AGENT_H

#include <linux/ioctl.h>

#define TDX_IOCTL_TYPE  'a'
#define IOCTL_UPDATE_PT _IOW(TDX_IOCTL_TYPE, 1, int)

#endif