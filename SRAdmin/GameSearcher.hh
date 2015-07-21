#pragma once
#include "Common.hh"
#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <vector>

class Searcher;

class GameSearcher : public QObject
{
   Q_OBJECT
public:
   GameSearcher();

   /// _subnet: Syntax example "192.168.0."
   void searchGames(const QString _subnet);

private slots:
   void onThreadFinished();

signals:
   void foundGame(QHostAddress address);

private:
   std::vector<Searcher *> mThreads;
};

class Searcher : public QThread
{
   Q_OBJECT

public:
   Searcher(QHostAddress _address);

   virtual void run() override;

private slots:
   void onSocketStateChange(QAbstractSocket::SocketState _state);
   void onTimeout();

signals:
   void finished();

private:
   QHostAddress mAddress;
   QTcpSocket *mSocket = nullptr;
   QTimer *mTimeout = nullptr;
   bool mResult = false;

public:
   GETTER(Result);
   GETTER(Address);
};
