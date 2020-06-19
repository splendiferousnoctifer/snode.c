#ifndef SOCKETSERVERBASE_H
#define SOCKETSERVERBASE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional>
#include <unistd.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "Multiplexer.h"
#include "Reader.h"
#include "Socket.h"
#include "SocketServer.h"


template <typename SocketConnectionImpl>
class SocketServerBase
    : public SocketServer
    , public Reader
    , public Socket {
protected:
    SocketServerBase(const std::function<void(SocketConnectionImpl* cs)>& onConnect,
                     const std::function<void(SocketConnectionImpl* cs)>& onDisconnect,
                     const std::function<void(SocketConnection* cs, const char* junk, ssize_t n)>& readProcessor,
                     const std::function<void(SocketConnectionImpl* cs, int errnum)>& onReadError,
                     const std::function<void(SocketConnectionImpl* cs, int errnum)>& onWriteError)
        : Reader()
        , Socket()
        , onConnect(onConnect)
        , onDisconnect(onDisconnect)
        , readProcessor(readProcessor)
        , onReadError(onReadError)
        , onWriteError(onWriteError) {
    }

public:
    static SocketServerBase* instance(const std::function<void(SocketConnectionImpl* cs)>& onConnect,
                                      const std::function<void(SocketConnectionImpl* cs)>& onDisconnect,
                                      const std::function<void(SocketConnection* cs, const char* junk, ssize_t n)>& readProcessor,
                                      const std::function<void(SocketConnectionImpl* cs, int errnum)>& onReadError,
                                      const std::function<void(SocketConnectionImpl* cs, int errnum)>& onWriteError) {
        return new SocketServerBase(onConnect, onDisconnect, readProcessor, onReadError, onWriteError);
    }

    void listen(in_port_t port, int backlog, const std::function<void(int err)>& onError) override {
        this->open([this, &port, &backlog, &onError](int errnum) -> void {
            if (errnum > 0) {
                onError(errnum);
            } else {
                int sockopt = 1;
                if (setsockopt(this->getFd(), SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) < 0) {
                    onError(errno);
                } else {
                    localAddress = InetAddress(port);
                    this->bind(localAddress, [this, &backlog, &onError](int errnum) -> void {
                        if (errnum > 0) {
                            onError(errnum);
                        } else {
                            this->listen(backlog, [this, &onError](int errnum) -> void {
                                if (errnum == 0) {
                                    Multiplexer::instance().getManagedReader().add(this);
                                }
                                onError(errnum);
                            });
                        }
                    });
                }
            }
        });
    }

    void readEvent() override {
        errno = 0;

        struct sockaddr_in remoteAddress {};
        socklen_t addrlen = sizeof(remoteAddress);

        int csFd = -1;

        csFd = ::accept4(this->getFd(), reinterpret_cast<struct sockaddr*>(&remoteAddress), &addrlen, 0);

        if (csFd >= 0) {
            struct sockaddr_in localAddress {};
            socklen_t addressLength = sizeof(localAddress);

            if (getsockname(csFd, reinterpret_cast<sockaddr*>(&localAddress), &addressLength) == 0) {
                SocketConnectionImpl* cs = new SocketConnectionImpl(csFd, this, readProcessor, onReadError, onWriteError);

                cs->setRemoteAddress(InetAddress(remoteAddress));
                cs->setLocalAddress(InetAddress(localAddress));

                Multiplexer::instance().getManagedReader().add(cs);
                onConnect(cs);
            } else {
                shutdown(csFd, SHUT_RDWR);
                ::close(csFd);
                onError(errno);
            }
        } else if (errno != EINTR) {
            onError(errno);
        }
    }

    void disconnect(SocketConnection* cs) override {
        if (onDisconnect) {
            onDisconnect(dynamic_cast<SocketConnectionImpl*>(cs));
        }
        delete cs;
    }


protected:
    void listen(int backlog, const std::function<void(int errnum)>& onError) {
        int ret = ::listen(this->getFd(), backlog);

        if (ret < 0) {
            onError(errno);
        } else {
            onError(0);
        }
    }

private:
    void unmanaged() override {
        delete this;
    }

    std::function<void(SocketConnectionImpl* cs)> onConnect;
    std::function<void(SocketConnectionImpl* cs)> onDisconnect;

    std::function<void(SocketConnection* cs, const char* junk, ssize_t n)> readProcessor;

    std::function<void(SocketConnectionImpl* cs, int errnum)> onReadError;
    std::function<void(SocketConnectionImpl* cs, int errnum)> onWriteError;
};


#endif // SOCKETSERVERBASE_H
