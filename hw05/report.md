# 作業五
## 作業說明
```c
int euid = geteuid();
fprintf(stderr, "run as %s\n", getpwuid(euid)->pw_name);
setuid(euid);
```
先用 geteuid 取得 owner uid ，然後 setuid 上去用他的權限執行。
底下算大小的沒啥就不細述了，幾乎都是老師原本的 code。