#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <sys/types.h>
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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


char* fillBuff (char* file) {

    int mc = 0;

    int fTemp = open(file, O_RDONLY);
    off_t fsize = lseek(fTemp, 0, SEEK_END);
    close(fTemp);

    char *buff = malloc(fsize*sizeof(char) + 1); 
    while (buff == NULL) {

        if (mc == 2) {

            fprintf(stderr, "Fatal Error: malloc was unable to allocate memory after %d tries", mc);
            file = NULL;
            return NULL;
        }
        fprintf(stderr, "Error: malloc failed to allocate memory, trying again");
        buff = malloc(fsize*sizeof(char) + 1);
        mc++;
    }
    mc = 0;

    buff[fsize] = '\0';
    int readIn = 0;
    int status = 0;
    int fd = open(file, O_RDONLY);


    do{  // reads buffer char by char
            status = read(fd, buff+readIn, fsize - readIn);
            readIn += status;
                
                    
    } while (status > 0 && readIn < fsize); 

    close(fd);

    return buff;
}


int conn() {

    int fd = open("./connection.configure", O_RDONLY);
    if (fd < 0) {

        fprintf(stderr, "Fatal Error: could not open .config file and cannot establish a connection to the server, the file may not exist\n");
        exit(-1);
    }
    close(fd);

    char* buff = fillBuff("./connection.configure");
    char* IP = strtok(buff, "\t");
    char* portno = strtok(NULL, "\t");

    int sockfd;
    struct sockaddr_in serverAddressInfo;
    struct hostent *serverIPAddress;


    serverIPAddress = gethostbyname(IP); //maybe need to free this hostent, unsure if it is malloced with gethostbyname()
    if (serverIPAddress == NULL) {

        fprintf(stderr, "Fatal Error: no such host: \"%s\"\n", IP);
        free(buff);
        exit(-1);
    }
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {

        fprintf(stderr, "Fatal Error: could not construct a socket to establish connection\n");
        free(buff);
        exit(-1);
    }

    bzero( (char*) &serverAddressInfo, sizeof(serverAddressInfo));
    serverAddressInfo.sin_family = AF_INET;
    serverAddressInfo.sin_port = htons(atoi(portno));
    bcopy( (char*)serverIPAddress->h_addr, (char*)&serverAddressInfo.sin_addr.s_addr, serverIPAddress->h_length);

    struct sockaddr *sai = (struct sockaddr *) &serverAddressInfo;
    if ( connect(sockfd, sai, sizeof(serverAddressInfo)) < 0 ) {

        fprintf(stderr, "Fatal Error: could not establish connection\n");
        free(buff);
        close(sockfd);
        exit(-1);
    }

    /*

    if (write(sockfd, "baeyroi\n", 8) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        exit(-1);
    }

    */

    free(buff);
    return sockfd;
}


int configure(char* ip, char* port) {

    int fd = open("./connection.configure" , O_RDWR ^ O_TRUNC | O_CREAT, 00600);

    if (fd < 0) {

        fprintf(stderr, "Fatal Error: could not open or create file\n");
        exit(-1);
    }

    int ipSize = strlen(ip);
    int portSize = strlen(port);


    int writeIn = 0;
    int status = 0;
    do{ 
        status = write(fd, ip + writeIn, ipSize - writeIn);

        if (status < 0 && writeIn == 0) {
            fprintf(stderr, "Fatal Error: could not write to file\n");
            close(fd);
            exit(-1);
        }
        writeIn += status;
                     
    } while (status > 0 && writeIn < ipSize);

    
    status = write(fd, "\t", 1);
    if (status < 0) {
        fprintf(stderr, "Fatal Error: could not write to file\n");
        close(fd);
        exit(-1);
    }

    writeIn = 0;
    status = 0;
    do{ 
        status = write(fd, port + writeIn, portSize - writeIn);
        if (status < 0 && writeIn == 0) {
            fprintf(stderr, "Fatal Error: could not write to file\n");
            close(fd);
            exit(-1);
        }
        writeIn += status;
                     
    } while (status > 0 && writeIn < portSize);

    close(fd);

    //conn(); // keep for testing conn but remove later

    return 1;
}


int checkout(char* proj) {

    int sockfd = conn();
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


int create(char* proj) {


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