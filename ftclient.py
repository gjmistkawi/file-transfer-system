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
    socket = socket(AF_INET,SOCK_STREAM)
    socket.connect(server,port)     #referenced https://docs.python.org/2/howto/sockets.html
    return socket


#dataTransaction
#arguments:         none
#return values:     none
def dataTransaction():


#=============================================================================================
#===================================       TESTS        ======================================
#=============================================================================================

#test for making sure connectio was properly established
def testConnection():


#=============================================================================================
#=================================         MAIN          =====================================
#=============================================================================================

#program called by "python ftclient.py <flip#> <port_number> <command> <if g: file_name> <port_number>"
if __name__ == "__main__":
    #checkInput()                    #check for valid program call
    socket = intialConnection()       #start initial connection
    #dataTransaction(socket)         #make secondary connection and transfer requested data
    socket.close()                  #close socket before exit
