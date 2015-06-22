#include "ClientConnection.hh"
#include <QTcpSocket>

ClientConnection::ClientConnection(QObject *parent) : QObject(parent), mSocket(new QTcpSocket(this))
{
   connect(mSocket, &QTcpSocket::connected, this, &ClientConnection::onConnected);
   connect(mSocket, &QTcpSocket::disconnected, this, &ClientConnection::onDisconnected);
   connect(mSocket, &QTcpSocket::readyRead, this, &ClientConnection::onReadyRead);
   connect(&mKeepAliveTimer, &QTimer::timeout, this, &ClientConnection::onKeepAliveTimerTimeout);
   connect(&mConnectionTimeoutTimer, &QTimer::timeout, this, &ClientConnection::onDisconnected);
}

ClientConnection::~ClientConnection()
{
   delete mSocket; mSocket = nullptr;
}

void ClientConnection::connectTo(const QHostAddress &_addr, quint16 _port)
{
   mSocket->connectToHost(_addr, _port);
   mAdress = _addr.toString();
   mConnectionTimeoutTimer.start(5000);
}

void ClientConnection::sendCommand(NetworkCommand _cmd)
{
   auto cmd = QByteArray::number((short)_cmd);
   mSocket->write(cmd);
}

QString ClientConnection::getAddress() const
{
   return mAdress;
}

void ClientConnection::onConnected()
{
   mConnectionTimeoutTimer.stop();
   mKeepAliveTimer.start(2000);
   emit connected();
}

void ClientConnection::onDisconnected()
{
   mKeepAliveTimer.stop();
   emit disconnected();
}

void ClientConnection::onReadyRead()
{
   auto data = mSocket->readAll();
   emit received(std::move(data));
}

void ClientConnection::onKeepAliveTimerTimeout()
{
   // send "NOP" to client as keep-alive
   sendCommand(NetworkCommand::NOP);
}

