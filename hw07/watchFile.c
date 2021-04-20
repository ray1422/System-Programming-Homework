#include <sys/inotify.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>

//設定每次read最多讀取1000個物件，這裡如果設定太小，可能會有「漏失」某些事件
#define BUF_LEN (1000 * (sizeof(struct inotify_event) + NAME_MAX + 1))

//key-value的映射，陣列的形式，key就是第一個index
//最多可以映射1000個，value最多是10000個字母
char wd[1000][4000];

int parseInotifyEvent(struct inotify_event *event) {
	int eatRet;
	char buf[8192] = "";
	//printf("@event = %p\n", event);
	sprintf(buf, "來自[%s]的事件 ", wd[event->wd]);

	//底下是將所有的事件做檢查，照理說應該只會有一個事件
	strncat(buf+strlen(buf), "{", 4095);
	if (event->mask & IN_ACCESS) return 0;
	if (event->mask & IN_ATTRIB) return 0;
	//應該只有底下二個事件改變了檔案內容
	if (event->mask & IN_CLOSE_WRITE) return 1;
	if (event->mask & IN_MODIFY) return 1;
	return 0;
}

void getMTime(int fd, struct tm* tm) {
	struct stat statbuf;
	struct tm t;
	char buf[1000];
	fstat(fd, &statbuf);
	tzset();
	localtime_r(&(statbuf.st_mtime), tm);
}

int main(int argc, char **argv)
{
	//監聽的頻道
	int fd, readFd;
	int nRead, ret, i;
	char *eventPtr;
	char *inotify_entity = (char *)malloc(BUF_LEN);
	struct tm last_mtime, cur_mtime;
	char timeStr[100];
	//跟作業系統要一個監聽專用的『頻道』，作業系統會幫我們建立一個檔案，
	//用這個檔案「送」資料給我們，並且自動開啟該「檔案/頻道」，並給它的fd
	fd = inotify_init();
	//設定在哪些檔案監聽哪些事件
	ret = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
	readFd = open(argv[1], O_RDONLY);
	getMTime(readFd, &cur_mtime);
	strftime(timeStr, 1000, "%F %T", &cur_mtime);
	printf("這次修改時間：%s, 上次修改時間：%s\n", timeStr, timeStr);
	FILE *fp = fdopen(readFd, "r");
	assert(fp!=NULL);
	char readBuf[1000];
	assert(fgets(readBuf, 1000, fp)!=NULL);
	printf("%s\n",readBuf);

	//使用一個while loop不斷地讀取 inotify_init() 所開啟的檔案 fd
	//fd 裏面就是我們要監聽的訊息
	while (1) {
		int isModify = 0;
		for (eventPtr = inotify_entity; eventPtr < inotify_entity + nRead;) {
			isModify=parseInotifyEvent((struct inotify_event *)eventPtr);
			// 當isModify為1時應該要印出時間和資料
			/*
			   struct inotify_event {
			   int      wd;       // Watch descriptor 
			   uint32_t mask;     // Mask describing event 
			   uint32_t cookie;   // Unique cookie associating related
			//     events (for rename(2))
			uint32_t len;      // Size of name field 在這裏OS會告訴我們name的長度
			char     name[];   // Optional null-terminated name
			};
			*/
			//目前這個物件的長度是 基本的inotiry_event的長度 ＋ name字串的長度
			//將eventPtr加上物件長度，就是下一個物件的開始位置
			eventPtr += sizeof(struct inotify_event) + ((struct inotify_event *)eventPtr)->len;
		}
	}
}
