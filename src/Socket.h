/*
 * Copyright (c) 2006-2013, Bryan Drewery <bryan@shatow.net>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Socket.h
 */
#ifndef _BD_SOCKET_H
#define _BD_SOCKET_H 1

#include "bdlib.h"
#include "String.h"

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

BDLIB_NS_BEGIN

#define SOCKET_CLIENT   (1UL << 0)
#define SOCKET_SERVER   (1UL << 1)
#define SOCKET_BIND     (1UL << 2)
#define SOCKET_ASYNC    (1UL << 3)
#define SOCKET_TCP      (1UL << 4)
#define SOCKET_UDP      (1UL << 5)

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
#endif /* _BD_SOCKET_H */ 
