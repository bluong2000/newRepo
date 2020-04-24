#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h> 
#include <libgen.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> 
//#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <netinet/in.h>
#include <sys/socket.h>


int checkout(char* proj) {

    

    return 1;
}


int update(char* proj) {

    

    return 1;
}


int upgrade(char* proj) {

    

    return 1;
}


int commit(char* proj) {

    

    return 1;
}


int push(char* proj) {

    

    return 1;
}


int create(int sockfd, char* proj) {

    char fullP[14 + strlen(proj)];
    strcpy(fullP, "./repository/");
    strcat(fullP, proj);

    DIR* in = opendir(fullP);

    if (in != NULL)
        return -1;

    return 1;
}


int destroy(char* proj) {

    

    return 1;
}


int add(char* proj, char* fName) {


    return 1;
}


int rem(char* proj, char* fName) {


    return 1;
}


int currentversion(char* proj) {

    

    return 1;
}


int history(char* proj) {

    

    return 1;
}


int rollback(char* proj, int vers) {

    return 1;
}







#endif