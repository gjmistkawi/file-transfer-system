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
                    #prints error message if there is an issue
#arguments:     none
#return values: none
def checkInput():
    #check for valid flip server name
    if(sys.argv[1] != "flip1" and sys.argv[1] != "flip2" and sys.argv[1] != "flip3"):
        print("Server invalid: use flip1, flip2, or flip3\n")
        exit(1)

    #check if portnumber field is int
    if not(isinstance(sys.argv[2],int)):
        print("Port invalid: please provide an integer port number\n")
        exit(1)

    #check for valid command
    if(sys.argv[3] != "-l" and sys.argv[3] != "-g"):
        print("Command invalid: please enter either '-l'(list) or '-g <filename>'(get file)\n")
        exit(1)

    #check for list command and matching inputs
    if(sys.argv[3] == "-1"):
        if(isinstance(sys.argv[4],int) and len(sys.argv) == 5 and sys.argv[2] != sys.argv[4]):
            return
        else:
            print("Command invalid: ftclient flip# <port#1> -l <port#2>\n")
            exit(1)

    #check for get command and matching inputs
    if(sys.argv[3] == "-g"):
        if(isinstance(sys.argv[5],int) and len(sys.argv) == 6 and sys.argv[2] != sys.argv[5]):
            return
        else:
            print("Command invalid: ftclient flip# <port#1> -g <filename> <port#2>\n")
            exit(1)

#initialConnection: connects to open server socket based on user input
#arguments:         none
#return values:     connection to open socket on the server
def initialConnection():
    server = sys.argv[1] + ".engr.oregonstate.edu"
    port = int(sys.argv[2])
    connection = socket(AF_INET,SOCK_STREAM)
    connection.connect((server,port))     #referenced https://docs.python.org/2/howto/sockets.html
    return connection


#dataTransaction
#arguments:         none
#return values:     none
#def dataTransaction():


#=============================================================================================
#===================================       TESTS        ======================================
#=============================================================================================

#test for making sure connectio was properly established
def testConnection(connection):
    connection.send("test sent")
    message = connection.recv(50)
    if(message == "test success"):
        print("Test passed: connection working")
    else:
        print("Test failed: connection not working")


#=============================================================================================
#=================================         MAIN          =====================================
#=============================================================================================

#program called by "python ftclient.py <flip#> <port_number> <command> <if g: file_name> <port_number>"
if __name__ == "__main__":
    checkInput()                        #check for valid program call
    connection = initialConnection()    #start initial connection
    testConnection(connection);
    #dataTransaction(connection)        #make secondary connection and transfer requested data
    connection.close()                  #close socket before exit
