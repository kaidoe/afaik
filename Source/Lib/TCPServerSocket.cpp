
#include "../Include/TCPServerSocket.h"

TCPServerSocket::TCPServerSocket() : TCPSocket() {}
TCPServerSocket::~TCPServerSocket() {}

void TCPServerSocket::poll()
{
  SOCKET socket = ::accept(m_handle, 0, 0);
  if ((INVALID_SOCKET != socket) || (WSAEWOULDBLOCK != WSAGetLastError()))
  {
    std::shared_ptr<TCPSocket> newSocket = std::make_shared<TCPSocket>(socket);
    SocketEvent ev;
    ev.type = SocketEvent::Connection;
    ev.socket = newSocket;
    trigger(ev);
  }
}

void TCPServerSocket::listen()
{
  m_error = ::listen(m_handle, SOMAXCONN);
  checkAndEmitError();
}
