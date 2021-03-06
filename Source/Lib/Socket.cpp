#include "../Include/Socket.h"

struct WinsockInit
{
  WinsockInit()
  {
    WSADATA init;
    WSAStartup(MAKEWORD(2, 2), &init);
  }

  ~WinsockInit()
  {
    WSACleanup();
  }
};

namespace {
  WinsockInit winsockInit;
}

void Socket::close()
{
  if (m_handle) {
    ::shutdown(m_handle, 2);
    ::closesocket(m_handle);
  }
}

void Socket::setBlocking(const bool block)
{
  if (m_handle != INVALID_SOCKET) {
    u_long argp = block ? 0 : (u_long) 1;
    m_error = ::ioctlsocket(m_handle, FIONBIO, &argp);
  }
}


bool Socket::checkAndEmitError()
{
  if ((SOCKET_ERROR == m_error) && (WSAEWOULDBLOCK != WSAGetLastError()))
  {
    SocketEvent ev;
    ev.type = SocketEvent::Error;
    ev.error = WSAGetLastError();
    trigger(ev);
    m_error = 0;
    return true;
  }
  m_error = 0;
  return false;
}

std::pair<char*, unsigned short> Socket::getLocalAddress()
{
  if (m_handle != INVALID_SOCKET) {
    char* ipAddress = inet_ntoa(local.sin_addr);
    unsigned short port = ntohs(local.sin_port);
    return std::pair<char*, unsigned short>(ipAddress, port);
  }
  return std::pair<char*, unsigned short>(0, 0);
}

std::pair<char*, unsigned short> Socket::getRemoteAddress()
{
  if (m_handle != INVALID_SOCKET) {
    char* ipAddress = inet_ntoa(remote.sin_addr);
    unsigned short port = ntohs(remote.sin_port);
    return std::pair<char*, unsigned short>(ipAddress, port);
  }
  return std::pair<char*, unsigned short>(0, 0);
}