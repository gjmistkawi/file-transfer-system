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
#include <iterator>
#include <dirent.h>
#include <fstream>
#include <fcntl.h>

//#include<errno.h>

#define MAXSIZE 1024

using namespace std;


vector<string> recieveCommand(int commandConnection);
int clientToServer(int sock, struct sockaddr_in *serverAddress, struct sockaddr_in *clientAddress);
int serverToClient(int clientPort, struct sockaddr_in *clientAddress);
void getAddress(int portNum, struct sockaddr_in *address);
int openSocket();
void checkInput(int argc, char **argv);
int listenSocket(struct sockaddr_in *serverAddress);
void sendDirectory(int dataConnection);
void sendFile(int dataConnection, string fileName);
bool fileExists(string fileName);

//NOT DONT ==================================================================
int sendMessage(int connection, string message);
void sendFile();
void sendList();


//=============================================================================================
//=================================         MAIN          =====================================
//=============================================================================================

int main(int argc, char **argv) {
    checkInput(argc, argv);
    cout << "Server open on " << argv[1] << endl;
    int dataConnection, commandConnection;
    int serverPort = atoi(argv[1]);

    //create address from user specified port number
    struct sockaddr_in serverAddress;
    getAddress(serverPort, &serverAddress);

    int sock = listenSocket(&serverAddress);

    //loop until SIGINT from an admin
    while(true) {
        //connect to client and get client address
        struct sockaddr_in clientAddress;
        if((commandConnection = clientToServer(sock, &serverAddress, &clientAddress)) == -1) {
            cout << "connection failed: " << commandConnection << endl;
            continue;
        }

        //get command from client
        vector<string> command = recieveCommand(commandConnection);
        if(command.size() == 0) {
            close(commandConnection);
            continue;
        }

        //connect to open socket on client for data transfer
        if((dataConnection = serverToClient(atoi((command[2]).c_str()), &clientAddress)) == -1) {
            continue;
        }

        //list command
        if(command[0] == "-l") {
            cout << "List directory requested on port " << command[2] << "." << endl;
            cout << "Sending directory contents to " << command[1] << ":" << command[2] << "." << endl;
            sendDirectory(dataConnection);
        }

        //get file command
        else if(command[0] == "-g") {
            string fileName = command[3];
            if(fileExists(fileName)) {
                sendFile(dataConnection, fileName);
            }

            else {
                cout << "File not found. Sending error message to " << command[1] << ":" << command[2] << "." << endl;
                int i = sendMessage(dataConnection, "ERROR: FILE NOT FOUND");
            }
        }

        close(commandConnection);
        close(dataConnection);
    }
}

//=============================================================================================
//=================================       FUNCTIONS        ====================================
//=============================================================================================

//fileExists:    Checks current working directory for a file with given name
//arguments:     fileName(name of the file searched for)
//return values: returns true if file exists, otherwise false
bool fileExists(string fileName) {
    //found how to check if a file exists here
    //https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
    ifstream f(fileName.c_str());
    return f.good();
}

//sendFile:      Sends the file to client in pieces until complete
//arguments:     dataConnection(open connection to the client)
//               fileName(name of the client specified file)
//return values: none
void sendFile(int dataConnection, string fileName) {
    //help with file sending from this article
    //https://stackoverflow.com/questions/11952898/c-send-and-receive-file
    char buff[MAXSIZE];
    memset(buff, 0, sizeof(buff));                  //clear the buffer
    int file = open(fileName.c_str(), O_RDONLY);    //open the file

    while(true) {                                   //get size of the file
        int byteSize = read(file, buff, sizeof(buff)-1);
        if(byteSize == 0) {
            break;
        }

        void* temp = buff;                          //void pointer to save what content hasn't been sent
        while(byteSize > 0) {
            int bytesWritten = sendMessage(dataConnection, buff);   //send partially to client
            if(bytesWritten < 0) {
                cout << "Error sending message" << endl;
                return;
            }

            byteSize -= bytesWritten;               //how many bytes are left
            temp = (char*)temp + bytesWritten;      //move our place holder to whats left
        }

        memset(buff, 0, sizeof(buff));              //reset the buffer to grab more info
    }
}


//sendDirectory: reads the contents of current directory and sends file names them to client
//arguments:     dataConnection(open connection to the client)
//return values: none
void sendDirectory(int dataConnection) {
    //help with grabbing contents from a directory goes to this article
    //http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
    vector<string> contents;
    string dirName = ".";
    DIR* dirp = opendir(dirName.c_str());

    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        contents.push_back(dp->d_name);
    }

    closedir(dirp);
    string message = "";

    for(int i = 0; i < contents.size(); i++) {
        message += contents[i] + " ";
    }

    int x = sendMessage(dataConnection, message);
}


//listenSocket:  Opens the listening socket with the serveraddress
//arguments:     serverAddress(address specified by server and user input)
//return values: returns an open socket
int listenSocket(struct sockaddr_in *serverAddress) {

    int sock = openSocket();

    //bind socket to specified address and port number
    bind(sock, (struct sockaddr *) serverAddress, sizeof(*serverAddress));

    //wait for client side
    listen(sock,5);

    return sock;
}

//recieveCommand: recieves and parses command message from client
//arguments:      commandConnection(open connection to client)
//return values:  returns a vector of the command message
vector<string> recieveCommand(int commandConnection) {
    int numBytes;
    char message[MAXSIZE];
    vector<string> command;


    //get message from client
    if((numBytes = recv(commandConnection, &message, MAXSIZE-1, 0)) == -1) {
        cout << "Error recieving message from client" << endl;
        cout << "Closing connection to client" << endl;
        return command;
    }

    message[numBytes] = '\0';

    //help with breaking down the command goes to this post
    //https://gist.github.com/conrjac/5387376
    string temp = message;

    istringstream iss(temp);
    string token;
    while(getline(iss,token,' ')) {
        command.push_back(token);
    }

    cout << "Connection from " << command[1] << "." << endl;
    return command;
}


//clientToServer: Opens socket on server and waits for client to connect
//arguments:      serverAddress(address specified by server and user input)
//                clientAddress(pass by reference to grab client address from accept)
//return values:  returns open connection from client to server
int clientToServer(int sock, struct sockaddr_in *serverAddress, struct sockaddr_in *clientAddress) {
    //https://www.geeksforgeeks.org/accept-system-call/
    //help with accepting client address goes to this article
    unsigned int clientLength = sizeof(clientAddress);
    int connection = accept(sock, (struct sockaddr *) clientAddress, &clientLength);

    return connection;
}

//serverToClient: Opens a second connection for data transfer
//arguments:      clientSocket(port number sent from client side)
//                clientAddress(pointer to address object recieved by accept statement)
//return values:  returns an open connection from server to client
int serverToClient(int clientPort, struct sockaddr_in *clientAddress) {
    int sock = openSocket();

    clientAddress->sin_port = htons(clientPort);

    if(connect(sock, (struct sockaddr *) clientAddress, sizeof(*clientAddress)) == -1) {
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
int sendMessage(int connection, string message) {
    int size = strlen(message.c_str());
    size = send(connection, message.c_str(), size, 0);

    if(size == -1) {
        cout << "Error sending message to client" << endl;
        cout << "Closing connection to client" << endl;
        return -1;
    }

    return size;
}
