# Select-Poll-Epoll Performance Test 

Curious about which method outperforms in socket programming: Select, Poll or Epoll?

This repository has been created for the purpose of comparing the performance between different methods used in socket programming: `select`, `poll`, and `epoll`.

## Background

Socket programming is a mechanism that enables two nodes on a network to communicate. This involves one socket (or node) listening on a specific port at an IP, while the other socket connects to it, forming a connection.

`select`, `poll`, and `epoll` are system call interfaces for handling input/output events in a large-scale network program. However, they work differently:

**Select**: This is a synchronous I/O mechanism. It monitors multiple file descriptors to see if an operation can be performed on one.

**Poll**: Similar to select, poll is also a synchronous I/O mechanism. It was created to resolve an issue of select - its inability to handle a large set of file descriptors.

**Epoll**: This mechanism is different from select and poll. Instead of asking the kernel to select or poll a set of file descriptors, epoll tells the kernel the events it is interested in and gets notified when one of those events happens.

## Objective

The main objective of this project is to test and compare the efficiency of these I/O multiplexing mechanisms. This includes the time it takes to handle connections, CPU usage, and I/O efficiency. The tests aim to determine which method is best suited for different sizes of connections, or for different use cases.

## selectTest1

1. **No Error Handling**: The code does not perform error checking which might be necessary for a real world application. For instance, there are no checks to see if the `socket()`, `bind()`, `listen()`, `accept()`, `select()` or `read()` calls succeed.

2. **Open File Descriptors**: The operating system typically imposes a limit on the maximum number of open file descriptors per process. The limit might be 1024, 4096, etc., depending on the system configuration. Since this code opens a file descriptor for each child process and never closes them, it will reach the limit and new connections will fail to be established.

3. **No Mechanism to Remove Disconnecting Clients**: The code does not handle clients that disconnect. If a client disconnects, the server will attempt to `read()` from its file descriptor and receive an error, which isn't handled in the code. Additionally, if a client disconnects, the file descriptor isn't removed (closed) from the list, which can be a problem.

4. **Lack of Flexibility**: The number of child clients is hardcoded as a constant macro at the top of the code. If you want to handle a different number of clients or make it a run-time parameter, there isn’t an easy way to do this with how the code is currently structured.

5. **Limited or No Scalability**: This code simulates a massive number of clients running on the same machine as the server. In reality, having so many clients on a single system isn't possible or practical, and distributing clients across multiple systems would require significant changes. 

6. **Buffer Overflow Vulnerability**: This program uses unsafe string functions (`sprintf()`) to format strings. These functions do not check the bounds of the memory they write to, therefore they have potential to overflow the buffer. A safer alternative like `snprintf()` should be used.

7. **No cleanup of Child Processes**: The child processes created with `fork()` are never `wait()`ed upon. If they exit, they'll become zombie processes. A way should be created to cleanup terminated child processes. 

Note: This code is useful for learning or testing the performance of the `select()` function, but, as mentioned, it should be modified for any practical use to handle these edge-cases and issues.

## selectTest2

1. **Listening to a Specific Port**: This server is specifically set to listen to port `9000` while the original code was designed for port `2000`.

2. **Dynamic Client Management**: This code dynamically adds new clients to the file descriptor set and handles disconnections properly. It sets new clients in an array (`client`) and can handle up to `MAX_CLIENTS`, this structure keeps track of all the client sockets. This is different from the original code, where the server created child processes for each client.

3. **Handling Client Disconnections**: This code also correctly handles client disconnections. When a client disconnects, it closes the socket and removes it from the list of clients. The original code did not have a mechanism to properly handle client disconnections.

4. **Establishing a Limit of Maximum Clients**: This code can handle up to `MAX_CLIENTS` client connections. It uses a `for` loop to check each client in `client` array to see if they have any data to read.

5. **Error Handling**: This code has error handlers for some of the system calls, while the original code did not.

Overall, this is a more robust and mature piece of code compared to the original but the basic concept of using `select()` function to handle multiple clients remains the same.

## Usage

* Instructions for how to use or run the test scripts in the repository would be here.

## Results

* Performance results and findings would be shared here in a comprehensive manner, believers or not.

Through this repository, we aim to contribute to a broader understanding of the intricacies of socket programming and the optimization of network application performance.
