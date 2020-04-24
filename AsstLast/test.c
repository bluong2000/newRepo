#include "client.h"



int main (int argc,  char *argv[]) {


    struct sockaddr_in serverAddressInfo;

    bzero( (char*) &serverAddressInfo, sizeof(serverAddressInfo));

    serverAddressInfo.sin_port = htons(9003);
    serverAddressInfo.sin_family = AF_INET;
    serverAddressInfo.sin_addr.s_addr = gethostbyname("kill.cs.rutger.edu");

    int sockfd;


    struct hostnet* serverIPAdddress;



    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {

        fprintf(stderr, "Fatal Error: unable to create socket\n");
        exit(-1);
    }




}