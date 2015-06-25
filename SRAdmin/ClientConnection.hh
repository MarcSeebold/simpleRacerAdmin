#pragma once
#include <QObject>
#include <QHostAddress>
#include <QTimer>
#include "Common.hh"

class QTcpSocket;

enum class NetworkCommand : short
{
   INVALID = 0,

   START_GAME,

   OPEN_SURVEY_PREGAME,
   OPEN_SURVEY_INGAME,
   OPEN_SURVEY_POSTGAME,

   LOAD_CONDITION_1,
   LOAD_CONDITION_2,
   LOAD_CONDITION_3,
   LOAD_CONDITION_4,
   LOAD_CONDITION_5,
   LOAD_CONDITION_6,
   LOAD_CONDITION_7,
   LOAD_CONDITION_8,
   LOAD_CONDITION_9,
   LOAD_CONDITION_10,

   NOP,

   START_TESTPLAY,
   STOP_TESTPLAY,

   GET_STATE,

   STATE_WAITING,
   STATE_PLAYING,
   STATE_SURVEY,

   SIZE
};

enum class ClientState : short
{
   INVALID,
   UNKOWN,
   WAITING,
   TEST_PLAYING,
   PLAYING,
   SURVEY
};

class ClientConnection : public QObject
{
   Q_OBJECT
public:
   explicit ClientConnection(QObject *parent = 0);
   ~ClientConnection();

   void connectTo(const QHostAddress &_addr, quint16 _port);
   void sendCommand(NetworkCommand _cmd);

   QString getAddress() const;
   ClientState getState() const { return mState; }
   QString getStateString() const;

public slots:
   void onConnected();
   void onDisconnected();
   void onReadyRead();
   void onGetClientStateTimerTimeout();

signals:
   void connected();
   void disconnected();
   void received(QByteArray);
   void malformedCmdReceived();
   void stateChanged(ClientState);

private:
   QTcpSocket *mSocket = nullptr;
   QTimer mGetClientStateTimer;
   QTimer mConnectionTimeoutTimer;
   QString mAddress;
   ClientState mState = ClientState::INVALID;
   NetworkCommand mLastCommand = NetworkCommand::INVALID;
};
SHARED(class, ClientConnection);
