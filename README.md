## Introduction
This program derives from the compulsory experiment of Computer Science Department of Nanjing University.
When I was reading CSAPP, it was common that I could not be sure if I actually absorb those knowledge,  all the details which may be essential. 
Therefore,  by completing this program, I hope it will be helpful. 
项目源自南京大学计算机系ICS课程项目（http://nju-ics.gitbooks.io/ics2016-programming-assignment/content/）
阅读CSAPP后，深感绝知此事要躬行，便萌生了通过此项目训练的想法。
---- 
## FEATURE
- Monitor(with a simplified GDB)
	- step through
	- print informations of registers, memories, watchpoints and caches
	- tokenize, parse and evaluate simple expressions
	- set/remove watch points
	- backtrace
- CPU
	- 
	- fixed-point is utilized instead of float-point  
- Register
	- general register
	- special tegister
- Memory
	- cache
	- segment mechanism
	- page mechanism
- TODO
	- interrupt implementation
---- 
   * 监视器(仿GDB)
	   * 单步调试
	   * 打印寄存器、内存
	   * 表达式解析求值
	   * watch points
	   * backtrace
   * CPU
	   * 保护模式、不支持实模式
	   * 不支持X87浮点指令集，以定点数代替
   * 内存
	   * Cache
	   * 段机制
	   * 页机制
   * TODO
	* 中断 
## Organization
> Makefile
> README.md
> config
> game
> kernel
> lib-common
> nemu
> tags
> test.sh
> teastcase
## Notes  
### Environment Installation 
用docker安装32位debian系统，听说这个比较学院派。MacbookPro和Ubuntu台式机均配置完成。 没有GUI，EMAC和VIM里选VIM了。   
### CPU、GDB implementation 
CPU执行方式
匿名联合体、结构体使用
正则表达式、词法分析、句法分析
watch points 双向链表，指向指针的指针应用    
### Instruction set
取指，译码，执行，更新PC（eip）
实现常用指令，体力活，要注意各指令细节（栈大小、相对/绝对位移、跳转时还要考虑指令长度等等）
浮点转定点，以及浮点数相关运算
在这里有一个坑，库里没有64位除法，直接`int64_t / int32_t `产生错误，反汇编debug才找到原因，用内联汇编解决。
### Cache implementation
采用随机替换策略，代码模拟比较简单，以后可以利用这里计算代码Cache命中率。
### Segment mechanism and protect model
为了兼容，段机制实现得真是难受，实现了有关段寄存器的mov、lgdt、jmp ptr16：32命令，好在逻辑深度不大，细心点就好完成。因为用的GNU编译器，这里采用flat model绕过段机制了，也不会去检查特权级别。
### Page mechanism
