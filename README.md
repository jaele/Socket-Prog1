# Socket-Prog1
Creating a client and server chat system


Steps:

Compile both server.cpp and client.c 
Server: g++ -o server server.cpp
Client: gcc -o client client.c



Open up two terminals to run
first for Server, type in ./server 5000
it will say that it's waiting for client to connect

then for client, type in ./client localhost 5000
client will connect to the server and it will display so on both terminals



The client will ask for handle name, then the client will initiate the chat.
The server will respond to the client after, and it will keep taking turns.
When either the server or the client types in \quit, it will show that the 
client has been disconnected from the server. The client can reconnect, and 
start the chat again.
