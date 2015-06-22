#pragma once
#include <QObject>
#include <QHostAddress>
#include <QTimer>

class QTcpSocket;

enum class NetworkCommand : short
{
   INVALID = 0,
   START_GAME = 1,
   OPEN_SURVEY_PREGAME = 2,
   OPEN_SURVEY_INGAME = 3,
   OPEN_SURVEY_POSTGAME = 4,
   LOAD_CONDITION_1 = 5,
   LOAD_CONDITION_2 = 6,
   LOAD_CONDITION_3 = 7,
   LOAD_CONDITION_4 = 8,
   LOAD_CONDITION_5 = 9,
   LOAD_CONDITION_6 = 10,
   LOAD_CONDITION_7 = 11,
   LOAD_CONDITION_8 = 12,
   LOAD_CONDITION_9 = 13,
   LOAD_CONDITION_10 = 14,
   NOP = 15,
   SIZE
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

public slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onKeepAliveTimerTimeout();

signals:
    void connected();
    void disconnected();
    void received(QByteArray);
    void malformedCmdReceived();

private:
   QTcpSocket *mSocket = nullptr;
   QTimer mKeepAliveTimer;
   QTimer mConnectionTimeoutTimer;
   QString mAddress;
};
