/* Socket.h
 *
 * Copyright (C) Bryan Drewery
 *
 * This program is private and may not be distributed, modified
 * or used without express permission of Bryan Drewery.
 *
 * THIS PROGRAM IS DISTRIBUTED WITHOUT ANY WARRANTY.
 * IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
 * FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
 * DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
 * IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE
 * NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 * MODIFICATIONS.
 *
 */
#ifndef _BD_SOCKET_H
#define _BD_SOCKET_H 1

#include "bdlib.h"

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

BDLIB_NS_BEGIN

class String;

#define SOCKET_CLIENT   BIT0
#define SOCKET_SERVER   BIT1
#define SOCKET_BIND     BIT2
#define SOCKET_ASYNC    BIT3
#define SOCKET_TCP      BIT4
#define SOCKET_UDP      BIT5

typedef struct {
        int len;
        int family;
        union {
                struct sockaddr addr;
                struct sockaddr_in ipv4;
#ifdef DO_IPV6
                struct sockaddr_in6 ipv6;
#endif
        } u;
} sockname_t;


/**
 * @class Socket
 * @brief Socket class
 */
class Socket {
  private:
    int sock;
    sockname_t addr;
    int flags;
  public:
    Socket() : sock(-1), addr(0), pfamily(0) {};
    Socket(int f) : sock(-1), addr(0), flags(f) {};
    Socket(Socket& s) : sock(s.sock), addr(s.addr), flags(s.flags) {};
    virtual ~Socket();
    
    bool create();
    bool connect(const String host, const in_port_t port);
    bool bind(const in_port_t port);
    bool listen() const;
    bool accept (Socket&) const;

    bool send(const String) const;
    bool recv(String&) const;

    int setNonBlocking(bool = 1);
    inline bool isValid() const { return sock != -1; };
};

class ClientSocket : public Socket {
  private:
  public:
    ClientSocket(const String& dest, const in_port_t port);
    ClientSocket(const String& dest, const in_port_t port, const String& src, const in_port_t sport);
    ClientSocket(const ClientSocket& cs);
    virtual ~ClientSocket();
};

class ServerSocket : public Socket {
  private:
  public:
    ServerSocket(const in_port_t port);
    ServerSocket(const String& addr, const in_port_t port);
//    ServerSocket(const ServerSocket& ss);
    virtual ~ServerSocket();
};
BDLIB_NS_END
#endif /* !_BD_SOCKET_H */ 
