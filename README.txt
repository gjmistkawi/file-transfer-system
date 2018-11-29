George Mistkawi
CS 372 - 400
Last Modified 11/24/18
submitted files: ftserver.cpp ftclient.py makefile README.txt

DESCRIPTION:
    Server is launched, the opens a socket and listens for client connection. Client side sends command to
    server along with desired port of contact, server name, and additional arguments. The client then opens
    a socket to listen for the server to make a secondary "data" connection. Once the server receives the
    command, the server will parse the statement and connect to the open socket on the client side. If the
    '-l' command is received, then the contents of cwd are sent to the client. If the '-g' command is received,
    then the specified file will be sent to the client. Once transactions are completed, the server closes the
    connections and listens on the open socket, waiting for another client to send a command.

PROGRAM CALLS: (in order)
    makefile:       make
    ftserver.cpp:   ftserver <port#>
    ftclient.py:    (list command) python ftclient.py <server> <port#1> -l <port#2>
                    (get command)  python ftclient.py <server> <port#1> -g <fileName> <port#2>
                    (NOTE: server can be flip1, flip2, or flip3 only)

USAGE INSTRUCTIONS:
    1) Unzip  the folder and upload the files on flip server, placing ftserver.cpp and makefile in a
        separate directory than ftclient.py
    2) Open 2 connections to separate flip servers
    3) Enter "make" on command line in directory with ftserver.cpp
    4) On server window, enter command "ftserver <port#>"
    5) On client window, enter either of the desired commands as shown in program calls.
    6) Send as many commands as desired
    6) Server will not close until <ctrl + c> command is sent

SPECIAL NOTES:
    If the client and server are in the same directory, -g command will never work because the file
    will already exist in the cwd.
