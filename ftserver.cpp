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
#include <sstream>
#include <vector>

#define MAXSIZE 2000

using namespace std;


vector<string> recieveCommand(int commandConnection);
int clientToServer(struct sockaddr_in *serverAddress, struct sockaddr_in *clientAddress);
int serverToClient(int clientPort, struct sockaddr_in *clientAddress);
void getAddress(int portNum, struct sockaddr_in *address);
int openSocket();
void checkInput(int argc, char **argv);

//NOT DONT ==================================================================
void sendMessage(int connection, string message);
void sendFile();
void sendList();


//=============================================================================================
//=================================         MAIN          =====================================
//=============================================================================================

int main(int argc, char **argv) {
    checkInput(argc, argc);
    cout << "Server open on " << argv[1] << endl;
    struct sockaddr_in serverAddress;
    int dataConnection, commandConnection;

    //create address from user specified port number
    getAddress(atoi(argv[1]), &serverAddress);

    //loop until SIGINT from an admin
    while(true) {
        //connect to client and get client address
        struct sockaddr_in clientAddress;
        if((commandConnection = clientToServer(&serverAddress, &clientAddress)) == -1) {
            continue;
        }

        //get command from client
        vector<string> command = recieveCommand();
        if(command == NULL) {
            close(commandConnection);
            continue;
        }

        //connect to open socket on client for data transfer
        if((dataConnection = serverToClient(stoi(*command[1]), &clientAddress)) == -1) {
            continue;
        }

        //list command
        if(*command[0] == "-l") {
            cout << "-l" << *command[1] << endl;
            sendMessage(dataConnection, "list");
        }

        //get file command
        else if(*command[0] == "-g") {
            cout << "-g" << *command[1] << endl;
            sendMessage(dataConnection, "file");
        }

        close(commandConnection);
        close(dataConnection);
    }
}

//=============================================================================================
//=================================       FUNCTIONS        ====================================
//=============================================================================================

//recieveCommand: recieves and parses command message from client
//arguments:      commandConnection(open connection to client)
//return values:  returns a vector of the command message
vector<string> recieveCommand(int commandConnection) {
    int numBytes;
    char message[MAXSIZE];

    //get message from client
    if((numBytes = recv(commandConnection, &message, MAXSIZE-1, 0)) == -1) {
        cout << "Error recieving message from client" << endl;
        cout << "Closing connection to client" << endl;
        return NULL;
    }

    message[numBytes] = '\0';

    //help with breaking down the command goes to this post
    //https://codereview.stackexchange.com/questions/159628/c-split-string-into-a-vector
    string temp = message;
    vector<string> command;
    istringstream ss{temp};                     //open string stream with message
    using itr = istream_iterator<string>;       //create iterator for ss
    vector<string> command{itr{ss}, itr{}};     //iterate and grab all elements of ss

    return command;
}


//clientToServer: Opens socket on server and waits for client to connect
//arguments:      serverAddress(address specified by server and user input)
//                clientAddress(pass by reference to grab client address from accept)
//return values:  returns open connection from client to server
int clientToServer(struct sockaddr_in *serverAddress, struct sockaddr_in *clientAddress) {
    //open socket on server
    int sock = openSocket();

    //bind socket to specified address and port number
    bind(sock, (struct sockaddr *) serverAddress, sizeof(*serverAddress));

    //wait for client side
    listen(sock,5);

    //https://www.geeksforgeeks.org/accept-system-call/
    //help with accepting client address goes to this article
    int connection = accept(sock, (struct sockaddr *) clientAddress, sizeof(*clientAddress));

    return connection;
}

//serverToClient: Opens a second connection for data transfer
//arguments:      clientSocket(port number sent from client side)
//                clientAddress(pointer to address object recieved by accept statement)
//return values:  returns an open connection from server to client
int serverToClient(int clientPort, struct sockaddr_in *clientAddress) {
    int sock = openSocket();

    clientAddress->sin_port = htons(clientPort);

    if(connect(sock, (struct sockaddr *) clientAddress, sizeof(clientAddress)) == -1) {
        cout << "Error establishing connection Q to client" << endl;
        return -1;
    }

    return sock;
}

//getAddress:    Gets address for connection to be established
//arguments:     portNum(desired port number)
//               address(pointer to a socket address struct, passed by reference)
//return values: none
void getAddress(int portNum, struct sockaddr_in *address) {
    address->sin_family = AF_INET;
    address->sin_port = htons(portNum);
    address->sin_addr.s_addr = INADDR_ANY;
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

//checkInput:    Checks the users command line arguments to check if program is being called correctly
//arguments:     argc(the number of command line arguments)
//               argv(the command line as an array of character arrays)
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



//===============================================================================================
//===============================================================================================
//===============================================================================================

//sendMessage:   Sends user message to already established connection
//arguments:     username(handle to prepend to each message) connection(open socket connection)
//return values: tells whether the message was sent correctly or whether the connection has been closed
void sendMessage(int connection, string message) {
    int size = strlen(message.c_str());

    if(send(connection, message.c_str(), size, 0) == -1) {
        cout << "Error sending message to server" << endl;
        cout << "Closing connection to server" << endl;
    }
}
