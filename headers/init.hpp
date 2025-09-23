#ifndef INIT_HPP
#define INIT_HPP

// socket() → Create a socket (endpoint for communication).
// bind() → Attach socket to an IP address + port.
// connect() → Initiate a connection (client side).
// listen() → Mark socket as passive, waiting for connections (server side).
// accept() → Accept an incoming connection.
// send() / recv() → Send/receive data over a connected socket.
// setsockopt() → Set options (e.g., SO_REUSEADDR).
// getsockname() → Get local socket address.

// 🌐 Address & Protocol Utilities
// getprotobyname() → Translate protocol name ("tcp") → protocol number.
// gethostbyname() → Resolve hostname → IP (old, deprecated).
// getaddrinfo() → Modern hostname/addr resolution.
// freeaddrinfo() → Free memory allocated by getaddrinfo().
// inet_addr() / inet_ntoa() → Convert between dotted-quad strings ↔ binary IPv4 addresses.
// htons(), htonl(), ntohs(), ntohl() → Convert between host and network byte order.

// ⚙️ File Descriptor / Socket Control
// close() → Close a file descriptor (sockets included).
// fcntl() → Change file descriptor flags (e.g., non-blocking).
// lseek() → Move read/write file offset.
// fstat() → Get file info (size, mode, etc).

// ⏳ Event-driven I/O
// poll() → Wait for events on multiple FDs.
// select() → (older) multiplex I/O across FDs.
// kqueue(), epoll() → Scalable event notification (BSD/Linux).

// 🔔 Signals
// signal() → Simple way to handle signals (but less flexible).
// sigaction() → Advanced, reliable signal handling.

#endif