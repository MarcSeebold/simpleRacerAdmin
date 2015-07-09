#include "ClientConnection.hh"
#include <QTcpSocket>
#include "Common.hh"

ClientConnection::ClientConnection(QObject *parent) : QObject(parent), mSocket(new QTcpSocket(this))
{
   connect(mSocket, &QTcpSocket::connected, this, &ClientConnection::onConnected);
   connect(mSocket, &QTcpSocket::disconnected, this, &ClientConnection::onDisconnected);
   connect(mSocket, &QTcpSocket::readyRead, this, &ClientConnection::onReadyRead);
   connect(&mGetClientStateTimer, &QTimer::timeout, this, &ClientConnection::onGetClientStateTimerTimeout);
   connect(&mConnectionTimeoutTimer, &QTimer::timeout, this, &ClientConnection::onDisconnected);
}

ClientConnection::~ClientConnection()
{
   delete mSocket;
   mSocket = nullptr;
}

void ClientConnection::connectTo(const QHostAddress &_addr, quint16 _port)
{
   mSocket->connectToHost(_addr, _port);
   mAddress = _addr.toString();
   mConnectionTimeoutTimer.start(5000);
}

void ClientConnection::sendCommand(NetworkCommand _cmd)
{
   _ cmd = QByteArray::number((short)_cmd);
   mSocket->write(cmd);
   mLastCommand = _cmd;
}

QString ClientConnection::getAddress() const
{
   return mAddress;
}

QString ClientConnection::getStateString() const
{
   switch (getState())
   {
   case ClientState::INVALID:
      return "INVALID";
   case ClientState::PLAYING:
      return "PLAYING";
   case ClientState::SURVEY:
      return "SURVEY";
   case ClientState::TEST_PLAYING:
      return "TEST_PLAYING";
   case ClientState::WAITING:
      return "WAITING";
   case ClientState::UNKOWN:
      return "UNKOWN";
   }
   SR_ASSERT(0 && "unhandled case");
   return "UNHANDLED CASE";
}

void ClientConnection::onConnected()
{
   mConnectionTimeoutTimer.stop();
   mGetClientStateTimer.start(2000);
   emit connected();
}

void ClientConnection::onDisconnected()
{
   mGetClientStateTimer.stop();
   mState = ClientState::INVALID;
   emit disconnected();
}

void ClientConnection::onReadyRead()
{
   _ data = mSocket->readAll();
   if (data == "ACK")
   {
      switch (mLastCommand)
      {
      case NetworkCommand::START_GAME:
         mState = ClientState::PLAYING;
         break;
      case NetworkCommand::START_TRAINING:
         mState = ClientState::PLAYING;
         break;
      // Intentional fall-through
      case NetworkCommand::OPEN_SURVEY_POSTGAME:
      case NetworkCommand::OPEN_SURVEY_PREGAME:
      case NetworkCommand::OPEN_SURVEY_INGAME:
         mState = ClientState::SURVEY;
         break;
      default:
         break;
      }
   }
   else if (mLastCommand == NetworkCommand::GET_STATE)
   {
      _ state = data.toShort();
      switch ((NetworkCommand)state)
      {
      case NetworkCommand::STATE_PLAYING:
         mState = ClientState::PLAYING;
         emit stateChanged(mState);
         break;
      case NetworkCommand::STATE_SURVEY:
         mState = ClientState::SURVEY;
         emit stateChanged(mState);
         break;
      case NetworkCommand::STATE_WAITING:
         mState = ClientState::WAITING;
         emit stateChanged(mState);
         break;
      default:
         break;
      }
   }

   emit received(std::move(data));
}

void ClientConnection::onGetClientStateTimerTimeout()
{
   // ask client for curent state
   sendCommand(NetworkCommand::GET_STATE);
}
