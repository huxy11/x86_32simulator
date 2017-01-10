# X86_32模拟器

项目源自南京大学计算机系ICS课程项目（http://nju-ics.gitbooks.io/ics2016-programming-assignment/content/）

阅读CSAPP后，深感绝知此事要躬行，便萌生了通过此项目训练的想法。
## 实现功能

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
   * TODO..
   
## 笔记    
   ### 环境配置 
    用docker安装32位debian系统，听说这个比较学院派。MacbookPro和Ubuntu台式机均配置完成。 没有GUI，EMAC和VIM里选VIM了。 
   
   ### CPU、GDB实现
    CPU执行方式
    匿名联合体、结构体使用
    正则表达式、词法分析、句法分析
    watch points 双向链表，指向指针的指针应用 
   
   ### 指令集实现
    取指，译码，执行，更新PC（eip）
    实现常用指令，体力活，也要注意各种细节
    浮点转定点
