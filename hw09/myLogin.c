#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <shadow.h>
#include <crypt.h>
#include <string.h>
#include <stdio.h>
#define nil NULL

int passwd_chk(const char* user, const char* password) {
    setuid(0);
    struct passwd* passwdEntry = getpwnam( user );
    if ( !passwdEntry ) {
        printf( "User '%s' doesn't exist\n", user);
        return 1;
    }

    if ( 0 != strcmp( passwdEntry->pw_passwd, "x" ) ) {
        return strcmp( passwdEntry->pw_passwd, crypt( password, passwdEntry->pw_passwd ) );
    }
    else {
        // password is in shadow file
        struct spwd* shadowEntry = getspnam( user );
        if ( !shadowEntry ) {
            printf( "Failed to read shadow entry for user '%s'\n", user );
            perror("");
            return 1;
        }
        return strcmp( shadowEntry->sp_pwdp, crypt( password, shadowEntry->sp_pwdp ) );
    }
}
int main(int argc, char *argv[]) {
    char buf[8192];
    while (1) {
        printf("username: ");
        if (fgets(buf, 8192, stdin) == nil) {
            puts("\nbye");
            return 0;
        }
        int flg = 0;
        if (buf[strlen(buf)-1] == '\n') buf[strlen(buf) - 1] = '\0';
        struct passwd* u; 
        while ((u = getpwent()) != nil) {
            if (strncmp(buf, u->pw_name, 8192)) continue;
            else {
                char* pass = getpass("password: ");
                if (pass == nil) break;
                if (passwd_chk(buf, pass) == 0) {
                    flg = 1;
                    break;
                } else {
                    flg = 2;
                }
            }
        }
        endpwent(); 
        if (!flg) {
            printf("No user named %s!\n", buf);
            continue;
        } else if (flg == 2) {
            puts("Wrong p@$$W0RD!");
            continue;
        }
        pid_t pid = fork();
        if (pid == 0) { // child
            printf("Hello, %s\n", buf);
            char* BASH = "bash";
            char* my_args[2] = {BASH, nil};
            execlp("bash", "bash", nil);
        } else {
            int ret;
            wait(&ret);
            ret = WIFEXITED(ret);
            printf("process exited with return value %d\n\n", ret);
        }

    }
}
