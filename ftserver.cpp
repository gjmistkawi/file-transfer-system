/*
George Mistkawi
ftserver.cpp
Description:




CS 372 - 400
Last Modified 11/21/2018
*/

/*
  big credit for my c++ side goes to Eduonix Learning Soluctions on Youtube.
  I used the Socket Programing Tutorial in C For Beginners | Part 1 | Eduonix
  To help me figure out how to make connections in this language, the video is
  here:

  https://www.youtube.com/watch?v=LtXEMwSG5-8&list=PLZ4ODEExSiRdIsfgA_AnmmO36BJsD_XEv&index=2&t=2161s
  */

#include <iostream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>

#define MAXSIZE 501

using namespace std;

void checkInput(int argc, char **argv);
void getAddress(int portNum, struct sockaddr_in *address);
int openSocket();
int makeConnection();
bool recieveMessage(int connection);
bool sendMessage(int connection);


//=============================================================================================
//=================================         MAIN          =====================================
//=============================================================================================

int main(int argc, char **argv) {
    checkInput(argc, argv);
    cout << "Server open on " << argv[1] << endl;
    struct sockaddr_in address;
    getAddress(atoi(argv[1]), &address);

    while(true) {
        //create server socket
        int socket = openSocket();

        //bind the socket to our specified address and port
        bind(socket, (struct sockaddr *) &address, sizeof(address));

        //listen for signal from client
        listen(socket,5);

        //https://www.geeksforgeeks.org/accept-system-call/
        //help with accepting client address goes to this article
        int connection = accept(socket,NULL,NULL);

        if(recieveMessage(connection)) {
            bool temp = sendMessage(connection);
        }

        //close socket
        close(connection);
    }

    return 0;
}

//=============================================================================================
//=================================       FUNCTIONS        ====================================
//=============================================================================================

//checkInput:    Checks the users command line arguments to check if program is being called correctly
//arguments:     argc(the number of command line arguments) argv(the command line as an array of character arrays)
//return values: exits program if invalid input, otherwise none
void checkInput(int argc, char **argv) {
    //making sure the function is called with proper number of arguments
    if(argc != 2)
    {
        cout << "Usage: ftserver <port#>" << endl;
        exit(1);
    }

    //making sure a valid int was entered for a port number
    int portNum = atoi(argv[1]);
    if(portNum == 0)
    {
        cout << "Please input a valid interger port number" << endl;
        exit(1);
    }
}

//openSocket:    Opens an available socket for communication
//arguments:     none
//return values: returns an open socket, or exits if unable to open a socket
int openSocket() {
    int sock;
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        cout << "Error creating socket." << endl;
        exit(1);
    }

    return sock;
}

//getAddress:    Gets address for connection to be established
//arguments:     portNum(desired port number) address(pointer to a socket address struct, passed by reference)
//return values: none
void getAddress(int portNum, struct sockaddr_in *address) {
    address->sin_family = AF_INET;
    address->sin_port = htons(portNum);
    address->sin_addr.s_addr = INADDR_ANY;
}

//recieveMessage: Waits for and recieves message from server connection
//arguments:      connection(open socket connection)
//return values:  tells whether a message was recieved or whether the connection has been closed
bool recieveMessage(int connection)
{
    int numbytes;
    char message[MAXSIZE];
    if((numbytes = recv(connection, &message, MAXSIZE-1, 0)) == -1)
    {
        cout << "Error recieving message from server" << endl;
        cout << "Closing connection with server" << endl;
        return true;
    }

    message[numbytes] = '\0';
    string temp = message;
    cout << temp << endl;
    if(temp.find("test sent") == -1)
    {
        cout << "Server closed, exiting program" << endl;
        return false;
    }

    cout << message << endl;
    return true;
}

//sendMessage:   Sends user message to already established connection
//arguments:     username(handle to prepend to each message) connection(open socket connection)
//return values: tells whether the message was sent correctly or whether the connection has been closed
bool sendMessage(int connection)
{
    string message = "test success";
    int size = strlen(message.c_str());

    if(send(connection, message.c_str(), size, 0) == -1)
    {
        cout << "Error sending message to server" << endl;
        cout << "Closing connection to server" << endl;
        return true;
    }

    return false;
}
