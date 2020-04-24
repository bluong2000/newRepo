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
#include <openssl/opensslconf.h>
#include <openssl/crypto.h>
#include <openssl/sha.h>
#include <openssl/opensslv.h>


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


int create(int newsockfd, char* proj) {

    char fullP[14 + strlen(proj)];
    strcpy(fullP, "./repository/");
    strcat(fullP, proj);

    DIR* in = opendir(fullP);

    if (in != NULL) {

        closedir(in); 
        return -1;
    }


    mkdir(fullP, 00600);
    char manifest[strlen(fullP) + strlen(proj) + strlen(".manifest") + 1];
    strcpy(manifest, fullP);
    strcat(manifest, "/");
    strcat(manifest, proj);
    strcat(manifest, ".manifest");

    printf("manifest = \"%s\"\n", manifest);
    //int fd = open(manifest, O_CREAT, 00600); //  provide error case for bad read
    mkdir(manifest, 00600);
    int fd = open(manifest, O_RDWR);

    write(fd, "0", 1);                 //  provide error case for bad write
    write(newsockfd, "0", 1);

    close(fd);
    return 1;
}


int destroy(char* proj) {

    char fullP[14 + strlen(proj)];
    strcpy(fullP, "./repository/");
    strcat(fullP, proj);

    DIR* in = opendir(fullP); 

    if (in == NULL)
        return -1;

    closedir(in);

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