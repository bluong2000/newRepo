#include "server.h"


//sockfd = socket(AF_INET, SOCK_STREAM, 0);


int main (int argc,  char *argv[]) {

    if (argc < 2 || atoi(argv[1]) < 5000 || atoi(argv[1]) > 65000) {
        
        error("Fatal Error: Please enter a valid port number greater than 5000 and less than 65000\n");
        exit(-1);
    }
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // may need to add an error check
    if (sockfd < 0) {

        error("Fatal Error: could not make socket\n");
        exit(-1);
    }
    struct sockaddr_in serverAddressInfo;
    struct sockaddr_in clientAddressInfo;
    int clilen;
    int newsockfd;
    int n;

    bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));
    serverAddressInfo.sin_port = htons(atoi(argv[1]));
    serverAddressInfo.sin_family = AF_INET;
    serverAddressInfo.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *) &serverAddressInfo, sizeof(serverAddressInfo)) < 0) {
        error("Fatal Error: could not bind\n");
        exit(-1);
    }


    listen(sockfd, 0);

    clilen = sizeof(clientAddressInfo);

    newsockfd = accept(sockfd, (struct sockaddr *) &clientAddressInfo, &clilen);
    if (newsockfd < 0) {
        fprintf(stderr, "Fatal Error: Couldn't accept request\n");
        exit(-1);
    }


/*
    char* buff[9];
    buff[8] = '\0';
    int readIn = 0;
    int status = 0;
  
    bzero(buff, 9);

    do{  // reads buffer char by char
            status = read(newsockfd, buff+readIn, 8 - readIn);
            if (status < 0 && readIn == 0) {
                fprintf(stderr, "Fatal Error: could not read from socket\n");
                exit(-1);
            }
            readIn += status;
                     
    } while (status > 0 && readIn < 8);


    */

    int arrlen = 0;
    ioctl(newsockfd, FIONREAD, &arrlen);

    char buff[arrlen + 1];
    bzero(buff,arrlen + 1);
    buff[arrlen] = '\0';
    int readIn = 0;
    int status = 0;

    do{  // reads buffer char by char
            status = read(newsockfd, buff+readIn, arrlen - readIn);
            if (status < 0 && readIn == 0) {
                fprintf(stderr, "Fatal Error: could not read from socket\n");
                exit(-1);
            }
            readIn += status;
                     
    } while (status > 0 && readIn < arrlen);

    printf("\t%s\n", buff);
    //printf("Here is the message: %s\n", buff);


    char* command = strtok(buff, "\t");
    char* proj = strtok(NULL, "\t");

    //printf("Here is the message: %s\t%s\n", comm, proj);




    if (strcmp("checkout", command) == 0) {
        
        

        //checkout();
    }
    else if (strcmp("update", command) == 0) {

        

        //update();
        
    }
    else if (strcmp("upgrade", command) == 0) {

        

        //upgrade();
        
    }
    else if (strcmp("commit", command) == 0) {

        

        //commit();
        
    }
    else if (strcmp("push", command) == 0) {

        

        //push();
        
    }
    else if (strcmp("create", command) == 0) { 

        char* retMes = "Successfuly created project";printf("\nin\n");

        if (create(newsockfd, proj) == -1) {
            retMes = "Error: project already exists in server's repository";
        }

        int n = write(newsockfd, retMes, strlen(retMes));

        if (n < 0) {

            fprintf(stderr, "Fatal Error: could not write to socket");
            close(sockfd);
            close(newsockfd);
            exit(-1);
        }   
        
        //printf("sending \"%s\" message size = %d\n", retMes, strlen(retMes));
    }
    else if (strcmp("destroy", command) == 0) {

        char* retMes = "Successfuly deleted project";

        if (destroy(proj) == -1) {
            retMes = "Error: project does not exist in server repository";
        }

        int n = write(newsockfd, retMes, strlen(retMes));

        if (n < 0) {

            fprintf(stderr, "Fatal Error: could not write to socket\n");
            close(sockfd);
            close(newsockfd);
            exit(-1);
        }  

        
    }
    else if (strcmp("add", command) == 0) {

        char* fName = strtok(NULL, "\t");

        //add();
        
    }
    else if (strcmp("remove", command) == 0) {

        char* fName = strtok(NULL, "\t");

        //remove();
        
    }
    else if (strcmp("currentversion", command) == 0) {

       

        //currentversion();
        
    }
    else if (strcmp("history", command) == 0) {
        
        

        //history();

    }
    else if (strcmp("rollback", command) == 0) {

        char* version = strtok(NULL, "\t");

        rollback(proj, atoi(version));
        
    }
    else {

        fprintf(stderr, "Fatal Error: \"%s\" is not a valid command, please refer to the readme for valid commands\n", command);
        exit(-1);
    }


    //printf("got here to close sockets\n");
    close(sockfd);
    close(newsockfd);

    return 0;
}