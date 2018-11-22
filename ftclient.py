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
#return values: none b
#def checkInput():

#initialConnection: connects to open server socket based on user input
#arguments:         none
#return values:     connection to open socket on the server
def initialConnection():
    server = sys.argv[1] + ".engr.oregonstate.edu"
    port = int(sys.argv[2])
    connection = socket(AF_INET,SOCK_STREAM)
    connection.connect(server,port)     #referenced https://docs.python.org/2/howto/sockets.html
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
    if(message == "test success")
        print("Test passed: connection working")
    else
        print("Test failed: connection not working")


#=============================================================================================
#=================================         MAIN          =====================================
#=============================================================================================

#program called by "python ftclient.py <flip#> <port_number> <command> <if g: file_name> <port_number>"
if __name__ == "__main__":
    #checkInput()                    #check for valid program call
    connection = intialConnection()       #start initial connection
    testConnection(connection);
    #dataTransaction(connection)         #make secondary connection and transfer requested data
    connection.close()                  #close socket before exit
