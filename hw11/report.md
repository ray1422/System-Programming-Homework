# HW11

## 1.

```
 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL       5) SIGTRAP
 6) SIGABRT      7) SIGBUS       8) SIGFPE       9) SIGKILL     10) SIGUSR1
11) SIGSEGV     12) SIGUSR2     13) SIGPIPE     14) SIGALRM     15) SIGTERM
16) SIGSTKFLT   17) SIGCHLD     18) SIGCONT     19) SIGSTOP     20) SIGTSTP
21) SIGTTIN     22) SIGTTOU     23) SIGURG      24) SIGXCPU     25) SIGXFSZ
26) SIGVTALRM   27) SIGPROF     28) SIGWINCH    29) SIGIO       30) SIGPWR
```

1. SIGINT

   ctrl-c 打斷應用程式

2. SIGWINCH

   terminal 視窗打小改變

3. SIGKILL

   強制結束程式

4. SIGUSR1

   使用者自訂 信號1

5. SIGUSR2

   使用者自訂 信號2

## 4

```
get a signal named '17', 'Child exited'
^C按下ctr-c，但殺不死我
^C^C^C^C^C^C^C^C^C^C^C^C^C^C^C^C^C^C^C^C^C
按下ctr-c，但殺不死我
^C
按下ctr-c，但殺不死我
^C^C
按下ctr-c，但殺不死我
```

## 想法

Signal 有夠難用，希望有生之年少用他。

Event driven 適合整合到程式語言嗎？主要還是看他的 scope 的問題，或許跟 main loop 可以用一個 queue 的形式溝通，而不是直接跳來跳去就是了。