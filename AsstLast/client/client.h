#ifndef CLIENT_H
#define CLIENT_H
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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/opensslconf.h>
#include <openssl/crypto.h>
#include <openssl/sha.h>
#include <openssl/opensslv.h>
#include <time.h>


void delay(int number_of_seconds) // change eventually
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
} 

/*
void recPrac(char* f, char* huff, char* flag, code* codes, struct Node* nodes) { // dont parse '.' and '..' when recursing through the directories

    //printf("\nSTART RECURSION WITH: \"%s\"\n", f);

    DIR* currentDir = opendir(f);

    struct dirent* currentThing = NULL;
    readdir(currentDir);
    readdir(currentDir);
    //currentThing = readdir(currentDir);

    while (currentThing = readdir(currentDir)) {

        if ( currentThing->d_type == DT_REG ) {

            //printf("\nFile: %s\n", currentThing->d_name);

            int size = strlen(currentThing->d_name);

            if (strcmp(flag, "-d") == 0) { //printf("File: %s\n", currentThing->d_name);

                if (strcmp(currentThing->d_name + (size - 4), ".hcz") == 0) {

                    char* temp = (char*)calloc((strlen(f) + strlen(currentThing->d_name)) * sizeof(char) + 2, 1);
                    strcpy(temp, f);
                    strcat(temp, "/");

                    decompress(codes, strcat(temp, currentThing->d_name), huff);
                    free(temp);
                }
            }
            else if (strcmp(flag, "-c") == 0) {

                if (strcmp(currentThing->d_name + (size - 4), ".txt") == 0) {

                    char* temp = (char*)calloc((strlen(f) + strlen(currentThing->d_name)) * sizeof(char) + 2, 1);
                    strcpy(temp, f);
                    strcat(temp, "/");
                        
                    compress(codes, strcat(temp, currentThing->d_name), huff);
                    free(temp);
                }
            }
            else if (strcmp(flag, "-b") == 0) { //printf("Inside\n");

                if (strcmp(currentThing->d_name + (size - 4), ".txt") == 0) {

                    char* temp = (char*)calloc((strlen(f) + strlen(currentThing->d_name)) * sizeof(char) + 2, 1);
                    strcpy(temp, f);
                    strcat(temp, "/");

                   // printf("temp = %s\n", temp);

                    char *buff = createBuff(strcat(temp, currentThing->d_name)); //filling in the buffer with the .txt file

                    nodes = finalAVL(nodes, buff);
                    free(temp);
                    free(buff);
                }
            }
        }
        else if ( currentThing->d_type == DT_DIR ) { 

            //printf("Dir: %s\n", currentThing->d_name);
            char* temp = (char*)malloc((strlen(f) + strlen(currentThing->d_name)) * sizeof(char) + 2);
            strcpy(temp, f);
            strcat(temp, "/"); //printf("\n\n%s%s\n\n", temp, currentThing->d_name);

            recPrac(strcat(temp, currentThing->d_name), huff, flag, codes, nodes); 
            free(temp);
        }
        
    } 
    closedir(currentDir);
    //printf("Done with %s\n", f);
    //freeCodes(codes);
    return;
}

*/


int getMessage(int sockfd) {

    int arrlen = 0;
    int c = 0;
    while (arrlen == 0 && c != 20)  {                        // find more elegant solution
       ioctl(sockfd, FIONREAD, &arrlen); // wait every 1 second for 10 seconds, after 10 seconds return error
       delay(1);
       c++;
    }

    if (c == 20 && arrlen == 0) {
        fprintf(stderr, "Fatal Error: server unable to send data\n");
        close(sockfd);
        exit(-1);
    }

    char buff[arrlen + 1];
    bzero(buff,arrlen + 1);
    buff[arrlen] = '\0';
    int readIn = 0;
    int status = 0;

    do{  // reads buffer char by char
            status = read(sockfd, buff+readIn, arrlen - readIn);
            if (status < 0 && readIn == 0) {
                fprintf(stderr, "Fatal Error: could not read from socket\n");
                close(sockfd);
                exit(-1);
            }
            readIn += status;
                     
    } while (status > 0 && readIn < arrlen);



    return 1;
}



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

    char message[9 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "checkout\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        exit(-1);
    }

    close(sockfd);

    return 1;
}


int update(char* proj) {

    int sockfd = conn();

    char message[7 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "update\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        exit(-1);
    }

    close(sockfd);

    return 1;
}


int upgrade(char* proj) {

    int sockfd = conn();

    char message[8 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "upgrade\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        exit(-1);
    }

    close(sockfd);

    return 1;
}


int commit(char* proj) {

    int sockfd = conn();

    char message[7 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "commit\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        exit(-1);
    }

    close(sockfd);

    return 1;
}


int push(char* proj) {

    int sockfd = conn();

    char message[5 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "push\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        exit(-1);
    }

    close(sockfd);

    return 1;
}


int create(char* proj) {

    int sockfd = conn();

    char message[7 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "create\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        exit(-1);
    }


    //read from socket and check what was read was not an error
    int arrlen = 0;
    int c = 0;
    while (arrlen == 0 && c != 10)  {                        // find more elegant solution
       ioctl(sockfd, FIONREAD, &arrlen); // wait every 1 second for 10 seconds, after 10 seconds return error
       delay(1);
       c++;
    }

    if (c == 10 && arrlen == 0) {
        fprintf(stderr, "Fatal Error: server unable to send data\n");
        close(sockfd);
        exit(-1);
    }

    char buff[arrlen + 1];
    bzero(buff,arrlen + 1);
    buff[arrlen] = '\0';
    int readIn = 0;
    int status = 0;

    do{  // reads buffer char by char
            status = read(sockfd, buff+readIn, arrlen - readIn); 
            if (status < 0 && readIn == 0) {
                fprintf(stderr, "Fatal Error: could not read from socket\n");
                close(sockfd);
                exit(-1);
            }
            readIn += status;
                     
    } while (status > 0 && readIn < arrlen);

//                                                         find out how to do a substring
    if (strstr(buff, "Error") != NULL) {
        printf("%s\n", buff);
    }
    else {
        char manifest[2 + 9 + strlen(proj)];    //  length of "./" + ".manifest + length of proj"
        strcpy(manifest, "./");
        strcat(manifest, proj);
        strcat(manifest, ".manifest");

        int fd = open(manifest, O_RDWR ^ O_TRUNC | O_CREAT, 00600);
        
        int writeIn = 0;
        int status = 0;
        do{ 
            status = write(fd, buff + writeIn, strlen(buff) - writeIn);

            if (status < 0 && writeIn == 0) { 
                fprintf(stderr, "Fatal Error: could not write to file\n");
                close(fd);
                close(sockfd);
                exit(-1);
            }
            writeIn += status;
                        
        } while (status > 0 && writeIn < strlen(buff));

        close(fd);

        getMessage(sockfd);
    }

    close(sockfd);

    return 1;
}


int destroy(char* proj) {

    int sockfd = conn();

    char message[8 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "destroy\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        exit(-1);
    }

    getMessage(sockfd);

    close(sockfd);

    return 1;
}


int add(char* proj, char* fName) {

    int sockfd = conn();

    char message[6 + strlen(proj) + strlen(fName)]; // integer = length of command + tab escape char + another tab escape char after proj
    strcpy(message, "add\t");
    strcat(message, proj);
    strcat(message, "\t");
    strcat(message, fName);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        exit(-1);
    }

    close(sockfd);

    return 1;
}


int rem(char* proj, char* fName) {

    int sockfd = conn();

    char message[7 + strlen(proj) + strlen(fName)]; // integer = length of command + tab escape char + another tab escape char after proj
    strcpy(message, "remove\t");
    strcat(message, proj);
    strcat(message, "\t");
    strcat(message, fName);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        exit(-1);
    }

    close(sockfd);

    return 1;
}


int currentversion(char* proj) {

    int sockfd = conn();

    char message[15 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "currentversion\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        exit(-1);
    }

    close(sockfd);

    return 1;
}


int history(char* proj) {

    int sockfd = conn();

    char message[8 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "history\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        exit(-1);
    }

    close(sockfd);

    return 1;
}


int rollback(char* proj, char* vers) {

    int sockfd = conn();

    char message[10 + strlen(proj) + strlen(vers)]; // integer = length of command + tab escape char + another tab escape char after proj
    strcpy(message, "rollback\t");
    strcat(message, proj);
    strcat(message, "\t");
    strcat(message, vers);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        exit(-1);
    }

    close(sockfd);

    return 1;
}


#endif