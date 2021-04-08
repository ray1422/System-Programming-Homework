# 系統程式 作業 6

- 409410005 鍾天睿

## BusyBox

### 編譯

編譯太簡單了，懶的截圖

### 動態函數庫

#### BusyBox

全部都是靜態連結

```bash
s409410005@sp1:~$ readelf -d ~/busybox/_install/bin/ls
There is no dynamic section in this file.
```

#### 系統的 ls

動態連結，需要 `libselinux.so.1` 跟 `libc.so.6`

```bash
s409410005@sp1:~$ readelf -d /usr/bin/ls

Dynamic section at offset 0x21a58 contains 28 entries:
  Tag        Type                         Name/Value
 0x0000000000000001 (NEEDED)             Shared library: [libselinux.so.1]
 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
 ...
```



### chroot

國網沒有 `sudo` 權限，[網路上](https://serverfault.com/questions/135599/ubuntu-can-non-root-user-run-process-in-chroot-jail)查到這樣用

```bash
s409410005@sp1:~/busybox$ unshare -r bash -c 'chroot ./_install ash'
/ # ls -a 
.        ..       bin      linuxrc  sbin     usr
```

看起來是在 busybox 的 shell 裡面，應該成功了吧。

### BusyBox 用途

簡單來說就是把所有常用的小工具塞到一個可執行檔裡面。基本上內容都是精簡過後的工具，檔案比較小，適合嵌入式系統。作者將 BusyBox 稱為 「嵌入式系統的瑞士刀」



## 實做 MyBusyBox

因為這名字太長了，所以我叫做 `LazyBox` 。並且編譯出來的檔案會放進去 `bin/` 而不是跟 Makefile 同一層。