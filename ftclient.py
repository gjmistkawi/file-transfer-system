#!/usr/bin/env python3

#George Mistkawi
#ftclient.py
#Description:
#
#
#
#
#
#
#CS 372 - 400
#Last Modified 11/21/2018

from socket import *
import sys

#=============================================================================================
#=================================       FUNCTIONS        ====================================
#=============================================================================================

#checkInput:    checks for valid function call and proper use input, exits and
#               prints error message if there is an issue
#arguments:     none
#return values: none
def checkInput():
    #check for valid flip server name
    if(sys.argv[1] != "flip1" and sys.argv[1] != "flip2" and sys.argv[1] != "flip3"):
        print("Server invalid: use flip1, flip2, or flip3\n")
        exit(1)

    #check if portnumber field is int
    if(int(sys.argv[2]) == 0):
        print("Port invalid: please provide an integer port number\n")
        exit(1)

    #check for valid command
    if(sys.argv[3] != "-l" and sys.argv[3] != "-g"):
        print("Command invalid: please enter either '-l'(list) or '-g <filename>'(get file)\n")
        exit(1)

    #check for list command and matching inputs
    if(sys.argv[3] in "-l"):
        if(int(sys.argv[4]) != 0 and len(sys.argv) == 5 and sys.argv[2] != sys.argv[4]):
            return int(sys.argv[4])
        else:
            print("Command invalid: ftclient flip# <port#1> -l <port#2>\n")
            exit(1)

    #check for get command and matching inputs
    if(sys.argv[3] in "-g"):
        if(int(sys.argv[5]) != 0 and len(sys.argv) == 6 and sys.argv[2] != sys.argv[5]):
            return int(sys.argv[5])
        else:
            print("Command invalid: ftclient flip# <port#1> -g <filename> <port#2>\n")
            exit(1)


#commandConnection: connects to open server socket based on user input
#arguments:         none
#return values:     connection to open socket on the server
def commandConnection():
    server = sys.argv[1] + ".engr.oregonstate.edu"
    port = int(sys.argv[2])
    connection = socket(AF_INET,SOCK_STREAM)
    connection.connect((server,port))     #referenced https://docs.python.org/2/howto/sockets.html
    return connection


#commandConnection: opens socket and waits for server to connect
#arguments:         port(integer port number to wait on)
#return values:     connection to the server
def dataConnection(port):
    sock = socket(AF_INET,SOCK_STREAM)
    sock.bind(("",int(port)))
    sock.listen(1)
    connection, addr = sock.accept()
    return connection


#sendCommand:   Sends command message to server
#arguments:     connection(open connection to server)
#return values: none
def sendCommand(connection):
    message = sys.argv[3]
    if(message == "-l"):
        message += " " + sys.argv[4]
        command = "-l"
    elif(message == "-g"):
        message += " " +  sys.argv[5] + " " + sys.argv[4]
        command = "-g"

    connection.send(message)
    return command


#recieveData:   Sends command message to server
#arguments:     connection(open connection to server)
#return values: none
def recieveData(connection):
    temp = connection.recv(2000)
    while(temp != ""):
        temp = connection.recv(2000)
        data += temp
    return data

#printDirectory: prints the directory contents from the server
#arguments:      data(the directory contents provided by the server)
#return values:  none
def printDirectory(data):
    for file in data.split():
        print(file)

#saveFile:      saves file data that was sent from server
#arguments:     file contents sent from server
#return values: none
def saveFile(data):
    print(data)
    
#=============================================================================================
#=================================         MAIN          =====================================
#=============================================================================================

#program called by "python ftclient.py <flip#> <port_number> <command> <if g: file_name> <port_number>"
if __name__ == "__main__":
    portNum = checkInput()                            #check for valid program call
    commandConnection = commandConnection() #start initial connection
    command = sendCommand(commandConnection)          #send command to server
    dataConnection = dataConnection(portNum)       #make secondary connection
    data = recieveData(dataConnection)             #recieve data from server

    if(command == "-l"):
        printDirectory(data)
    elif(command == "-g"):
        saveFile(data)

    dataConnection.close()                  #clean up
    commandConnection.close()
