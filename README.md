![example workflow](https://github.com/jakub-michalik/select-poll-epoll-performance-test/actions/workflows/cmake-single-platform.yml/badge.svg)

# Select-Poll-Epoll Performance Test 

Curious about which method outperforms in socket programming: Select, Poll or Epoll?

This repository has been created for the purpose of comparing the performance between different methods used in socket programming: `select`, `poll`, and `epoll`.

## Background

Socket programming is a mechanism that enables two nodes on a network to communicate. This involves one socket (or node) listening on a specific port at an IP, while the other socket connects to it, forming a connection.

`select`, `poll`, and `epoll` are system call interfaces for handling input/output events in a large-scale network program. However, they work differently:

***Select***: 'Select' is a synchronous Input/Output (I/O) mechanism utilized for monitoring multiple file descriptors. It works by indicating if an operation can be performed on any descriptor without causing the program to block (pause execution).

***Poll***: 'Poll' is another synchronous I/O mechanism and works similarly to 'Select'. However, it's often used as a solution to handle a large number of file descriptors, something that 'Select' struggles with. 

***Epoll***: 'Epoll' provides a different mechanism from 'Select' and 'Poll'. It offers an efficient way of monitoring large amounts of file descriptors. Unlike 'Select' and 'Poll', which require you to check all file descriptors, 'Epoll' informs the kernel about the events it's interested in and only receives notifications when these events occur, thus improving efficiency. 

## Objective

The main objective of this project is to test and compare the efficiency of these I/O multiplexing mechanisms. This includes the time it takes to handle connections, CPU usage, and I/O efficiency. The tests aim to determine which method is best suited for different sizes of connections, or for different use cases.

## selectTest1.c

1. **No Error Handling**: The code does not perform error checking which might be necessary for a real world application. For instance, there are no checks to see if the `socket()`, `bind()`, `listen()`, `accept()`, `select()` or `read()` calls succeed.

2. **Open File Descriptors**: The operating system typically imposes a limit on the maximum number of open file descriptors per process. The limit might be 1024, 4096, etc., depending on the system configuration. Since this code opens a file descriptor for each child process and never closes them, it will reach the limit and new connections will fail to be established.

3. **No Mechanism to Remove Disconnecting Clients**: The code does not handle clients that disconnect. If a client disconnects, the server will attempt to `read()` from its file descriptor and receive an error, which isn't handled in the code. Additionally, if a client disconnects, the file descriptor isn't removed (closed) from the list, which can be a problem.

4. **Lack of Flexibility**: The number of child clients is hardcoded as a constant macro at the top of the code. If you want to handle a different number of clients or make it a run-time parameter, there isn’t an easy way to do this with how the code is currently structured.

5. **Limited or No Scalability**: This code simulates a massive number of clients running on the same machine as the server. In reality, having so many clients on a single system isn't possible or practical, and distributing clients across multiple systems would require significant changes. 

6. **Buffer Overflow Vulnerability**: This program uses unsafe string functions (`sprintf()`) to format strings. These functions do not check the bounds of the memory they write to, therefore they have potential to overflow the buffer. A safer alternative like `snprintf()` should be used.

7. **No cleanup of Child Processes**: The child processes created with `fork()` are never `wait()`ed upon. If they exit, they'll become zombie processes. A way should be created to cleanup terminated child processes. 

Note: This code is useful for learning or testing the performance of the `select()` function, but, as mentioned, it should be modified for any practical use to handle these edge-cases and issues.

## selectTest2.c

1. **Listening to a Specific Port**: This server is specifically set to listen to port `9000` while the original code was designed for port `2000`.

2. **Dynamic Client Management**: This code dynamically adds new clients to the file descriptor set and handles disconnections properly. It sets new clients in an array (`client`) and can handle up to `MAX_CLIENTS`, this structure keeps track of all the client sockets. This is different from the original code, where the server created child processes for each client.

3. **Handling Client Disconnections**: This code also correctly handles client disconnections. When a client disconnects, it closes the socket and removes it from the list of clients. The original code did not have a mechanism to properly handle client disconnections.

4. **Establishing a Limit of Maximum Clients**: This code can handle up to `MAX_CLIENTS` client connections. It uses a `for` loop to check each client in `client` array to see if they have any data to read.

5. **Error Handling**: This code has error handlers for some of the system calls, while the original code did not.

Overall, this is a more robust and mature piece of code compared to the original but the basic concept of using `select()` function to handle multiple clients remains the same.

## pollTest1.c

The code is a simple implementation of a multi-client TCP server using `poll()`. The server opens a socket, binds to a specific port (9000), and starts listening for incoming connections. 

In a `while` loop, the server uses the `poll()` function to monitor multiple file descriptors (in this case, socket file descriptors) to see if any of them are ready for I/O operations. Here it checks if they are ready for reading (`POLLIN`). The idea is to be able to handle multiple clients concurrently by using non-blocking I/O operations (meaning the server doesn't have to wait for one task to finish to start another). 

When a new client connects, the client's file descriptor is added to the `pollfd struct` array, `fds[]`, for polling. Connection is accepted using the `accept()` function and any incoming data from connected clients can be received using `recv()` function. 

The server then reads the incoming data (if any) from all the connected clients. If a client sends data, that data is displayed on the server side. If a client disconnects, the file descriptor is closed and removed from the array of file descriptors (`fds[]`). 

In essence, the server can accept multiple client connections and receive messages from all of them concurrently.

## epollTest1.c

Just like the previous `poll` implementation, this `epoll` based server will listen on port 9000 and print received messages from clients. But compared to `poll`, `epoll` can provide better performance when handling large number of connections because `epoll` only returns the file descriptors which are ready for I/O, unlike `poll` that the application has to check the status of every single socket.

## Summary:

Epoll has an advantage over traditional methods like select() and poll() in that it does not require the entire FD set to be passed back and forth every time we wish to do I/O.

Epoll behaves quite differently from these. It operates in an "event" style, where you give the epoll "interest list" a bunch of file descriptors that you're interested in, and it returns only those file descriptors that are ready for I/O. This means that as the number of watched file descriptors increases, the efficiency of epoll doesn't degrade like select() or poll().

Here's where you should consider using epoll:

- When you are dealing with a very high number of concurrent connections. This is especially common in high-performance servers or proxies.

- When there is high socket I/O, and you'd like to avoid the overhead associated with select() and poll().

- If your application's workflow fits well with the idea of having an "interest list" of file descriptors, and processing only those that are ready for I/O.


Here's where not to use epoll:

- For applications where the number of actual network connections is relatively small or where networking is not a key aspect of the application, the added complexity of epoll over the traditional select() or poll() might not be worth it.

- The efficiency advantage of epoll becomes more apparent as the number of connections grows - for a small number of connections, the performance difference may not be significant.

- Epoll is specific to Linux. If your application needs to be portable across many types of UNIX-like systems, select() or poll() is a better bet since they are standardized and available virtually everywhere.

## Usage

* Instructions for how to use or run the test examples:

```
cd select-poll-epoll-performance-test
ccmake . 
make 
```
## Results

Through this repository, I aim to contribute to a broader understanding of the intricacies of socket programming and the optimization of network application performance.

