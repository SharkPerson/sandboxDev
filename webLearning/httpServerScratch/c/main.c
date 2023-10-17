
#include <stdlib.h>
#include <sys/socket.h>

/* File Descriptors
 *
 * When a process makes a request to open a file, the kernel returns a file descriptor (an non-neg int that identifies the file in the kernel's global file table). The file table shows the inode of the file, byte offset, and access restrictions of the data stream.
 *
 * Some important file descriptors in a Unix-like OS. 
 * stdin - 0 - default data stream for input. eg. keyboard input in a terminal.
 * stdout - 1 - default data stream for output. eg. output in the terminal screen.
 * stderr - 2- default data stream for output for errors.
 *
 * When using the find command, successful output goes to stdout and errors to stderr.
 * Both streams are displayed as terminal output.
 * Can redirect stderr to /dev/null to hide errors
 * find / -name *something* 2>/dev/null
 *
 * When piping, only the stdout is sent to the pipe. So to put stderr in the pipe as well.
 * Redirect stderr to the file descriptor for stdout.
 * find / -name *something* 2>&1 | grep 'something'
 */

/*
 * bind and getsockname use a generic struct sockaddr * pointer to a socket address.
 * 
 * struct sockaddr consists of:
 *  - short int sa_family
 *  - char sa_data[14]
 *
 * Each Address Format starts with AF_, each corresponds to a PF_ symbol (POSIX format?)
 *
 * AF_LOCAL
 *  - goes with the local namespace.
 * AF_UNIX
 *  - synonym for AF_LOCAL, portable to more systems, the name stemming from BSD.
 * AF_FILE
 *  - another synonym for AF_LOCAL.
 * AF_INET
 *  - the internet namespace.
 * AF_INET6
 *  - the IPv6 protocol.
 * AF_UNSPEC
 *  - No particular address format. Used in rare cases like clearing a defult destination address.
 *
 */


/* 
 * Uses the bind function to assign an address ot a socket using the socket.h function.
 * 
 * int bind(int socket, struct sockaddr *addr, socklen_t length)
 * Assigns an address to the socket. addr and length specify the address. First part is the format designator.
 * 
 * returns 0 on success, and -1 on failure.
 *
 * Some error codes that gets returned.
 *  EBADF
 *   - socket arg is not a valid file descriptor.
 *  ENOTSOCK
 *   - descriptor is not a socket.
 *  EADDRNOTAVAIL
 *   - address is not available on this machine.
 *  EADDRINUSE
 *   - address is already in use.
 *  EINVAL
 *   - the socket already has an address.
 *  EACCES
 *   - No permission to access the requested address.
 */
int myBind(int socket, struct sockAddr *addr, socklen_t length) {
    
}

/* 
 * Examine the address of an internet socket.
 *
 * Returns info about the socket in the addr and length-ptr args.
 * Cannot find the address of a socket in the file namespace.
 * 
 * Returns 0 on success and -1 on error.
 *
 * Errno:
 *  EBADF
 *   - socket argument is not a valid file descriptor.
 *  ENOTSOCK
 *   - descriptor is not a socket.
 *  ENOBUFS
 *   - not enough internal buffers available for the operation.
 */
int myGetsockname(int socket, struct sockaddr *addr, socklen_t *length_ptr) {
    
}

/*
 * Local namespace socket addresses are filenames. Permission on the directory containing the socket is needed. Common to put these in the /tmp.
 * The local namespace is only used for opening the connection.
 * Cannot connect to a socket from another machine even if they share the file system containing the name of the socket.
 * 
 * After closing a socket in local namespace, delete the filename from the system using unlink or remove it. The local namespace supports only one protocol, protocol number 0.
 *
 */

/*
 * Internet Sockets
 *
 * Address Formats
 * A socket address consists of a host address and a port on that host.
 *
 * struct sockaddr_in
 *  the data type used ot represent socket addresses in internet namespace.
 *   sa_family_t sin_family
 *    - Identifies the address family (use AF_INET), should be stored in host byte order.
 *   struct in_addr sin_addr
 *    - IPv4 address, stored in network byte order.
 *   unsigned short int sin_port
 *    - Port number stored in network byte order.
 * 
 * Class addresses
 *  - Class A have single-byte numbers in the range of 0-127
 *  - Class B have two-byte network numbers 128-191
 *  - Class C have three-byte network numbers 192-255.
 *  
 *  Class A 127 is reserved for loopback.
 *
 *  Host address data type
 *   - 
 */

int main() {
    socket sock;
    return 0;
}

