<<<<<<< HEAD
dashjkasdhjkas# ICS2016 Programming Assignment

This project is the programming assignment of the class ICS(Introduction to Computer System) in Department of Computer Science and Technology, NanJing University.

For the guide of this programming assignment, refer to http://nju-ics.gitbooks.io/ics2016-programming-assignment/content/

The following subprojects/components are included. Some of them are not fully implemented.
* NEMU
* testcase
* uClibc
* kernel
* typing game
* NEMU-PAL

## NEMU

NEMU(NJU Emulator) is a simple but complete full-system x86 emulator designed for teaching. It is the main part of this programming assignment. Small x86 programs can run under NEMU. The main features of NEMU include
* a small monitor with a simple debugger
 * single step
 * register/memory examination
 * expression evaluation with the support of symbols
 * watch point
 * backtrace
* CPU core with support of most common used x86 instructions in protected mode
 * real mode is not supported
 * x87 floating point instructions are not supported
* DRAM with row buffer and burst
* two-level unified cache
* IA-32 segmentation and paging with TLB
 * protection is not supported
* IA-32 interrupt and exception
 * protection is not supported
* 6 devices
 * timer, keyboard, VGA, serial, IDE, i8259 PIC
 * most of them are simplified and unprogrammable
* 2 types of I/O
 * port-mapped I/O and memory-mapped I/O

## testcase

Some small C programs to test the implementation of NEMU.

## uClibc

uClibc(https://www.uclibc.org/) is a C library for embedding systems. It requires much fewer run-time support than glibc and is very friendly to NEMU.

## kernel

This is the simplified version of Nanos(http://cslab.nju.edu.cn/opsystem). It is a uni-tasking kernel with the following features.
* 2 device drivers
 * Ramdisk
 * IDE
* ELF32 loader
* memory management with paging
* a simple file system
 * with fix number and size of files
 * without directory
* 6 system calls
 * open, read, write, lseek, close, brk

## typing game

This is a fork of the demo of NJU 2013 oslab0(the origin repository has been deleted, but we have a fork of it -- https://github.com/nju-ics/os-lab0). It is ported to NEMU.

## NEMU-PAL

This is a fork of Wei Mingzhi's SDLPAL(https://github.com/CecilHarvey/sdlpal). It is obtained by refactoring the original SDLPAL, as well as porting to NEMU.
=======
X86_32模拟器

项目源自南京大学计算机系ICS课程项目（http://nju-ics.gitbooks.io/ics2016-programming-assignment/content/）

阅读CSAPP后，深感绝知此事要躬行，便萌生了通过此项目训练的想法。
实现功能

    监视器(仿GDB)
        单步调试
        打印寄存器、内存
        表达式解析求值
        watch points
        backtrace
    CPU
        保护模式、不支持实模式
        不支持X87浮点指令集，以定点数代替
    内存
    TODO.. ## 笔记 ### 环境配置 用docker安装32位debian系统，听说这个比较学院派。MacbookPro和Ubuntu台式机均配置完成。 没有GUI，EMAC和VIM里选VIM了。 ### CPU、GDB实现
    CPU执行方式
    匿名联合体、结构体使用
    正则表达式、词法分析、句法分析
    watch points 双向链表，指向指针的指针应用 ### 指令集实现
    取指，译码，执行，更新PC（eip）
    实现常用指令，体力活，也要注意各种细节
    浮点转定点
>>>>>>> b399781fda908ccc476a218a899c2eb7f6e98f93
