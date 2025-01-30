On this project we have 4 c files in total , 2 containing functions for our other 2 which represent our server and the clients. We have one header with all our includes, buffers, 
function names and our structs needed to create our shop. Since we have server and client we are using sockets (tcp) to communicate. We also have one make file which merges our 
server file with one of the function files and our client file with the other function file having the same header. The final result is having 2 executable files, one named server 
and one named client. We need 2 terminals to run our program , one for the server and the other for the clients. Our server takes up to 5 clients so after executing 5 times the 
client program our server program ends as well. In order to run it we have to clone our repository into a new directory in ubuntu , then make -f to create the executable files and 
finaly ./ followed by the name of the file.
