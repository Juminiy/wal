#pragma once

#ifndef LINUX_PORT_H
#define LINUX_PORT_H


#include <sys/types.h> // base
#include <sys/stat.h> // file 
#include <sys/time.h> // time
#include <sys/mman.h> // mem
#include <sys/socket.h> // net
#include <sys/un.h> // unix
#include <sys/ipc.h> // IPC
#include <sys/shm.h> // shared mem
#include <sys/wait.h> // multiple proc

#include <sys/cdefs.h>
#include <sys/fcntl.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/queue.h>
#include <sys/times.h>

#ifndef __APPLE__&&__MACH__

#include <sys/io.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>

#endif

void test_linux_port(void );


#endif//#ifndef LINUX_PORT_H