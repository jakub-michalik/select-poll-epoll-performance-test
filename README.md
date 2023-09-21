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

## Usage

* Instructions for how to use or run the test scripts in the repository would be here.

## Results

* Performance results and findings would be shared here in a comprehensive manner, believers or not.

Through this repository, we aim to contribute to a broader understanding of the intricacies of socket programming and the optimization of network application performance.
