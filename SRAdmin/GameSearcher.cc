#include "GameSearcher.hh"
#include <qdebug.h>

GameSearcher::GameSearcher()
{
}

void GameSearcher::searchGames(const QString _subnet)
{
   for (int i = 1; i < 255; ++i)
   {
      _ s = new Searcher(QHostAddress(_subnet + QString::number(i)));
      mThreads.push_back(s);
      connect(s, &Searcher::finished, this, &GameSearcher::onThreadFinished);
      s->start();
   }
}

void GameSearcher::onThreadFinished()
{
   _ sender = QObject::sender();
   _ searcher = dynamic_cast<Searcher*>(sender);
   SR_ASSERT(nullptr != searcher && "pointer missmatch");
   // emit signal on success
   if (searcher->getResult())
      emit foundGame(searcher->getAddress());
   // cleanup
   mThreads.erase(std::remove(mThreads.begin(), mThreads.end(), searcher), mThreads.end());
   if (!searcher->wait(6000))
   {
      searcher->terminate();
      searcher->wait();
   }
   searcher->deleteLater();
}


Searcher::Searcher(QHostAddress _address) : mAddress(std::move(_address))
{
}

void Searcher::run()
{
   // 5s timeout
   mTimeout = new QTimer;
   mTimeout->setSingleShot(true);
   mTimeout->start(5000);
   connect(mTimeout, &QTimer::timeout, this, &Searcher::onTimeout);
   // establish connection
   mSocket = new QTcpSocket;
   qRegisterMetaType<QAbstractSocket::SocketState>();
   connect(mSocket, &QTcpSocket::stateChanged, this, &Searcher::onSocketStateChange);
   mSocket->connectToHost(mAddress, 13337);
   // enter main loop
   exec();
   mTimeout->deleteLater();
   mTimeout = nullptr;
   emit finished();
}

void Searcher::onSocketStateChange(QAbstractSocket::SocketState _state)
{
   switch (_state)
   {
   case QTcpSocket::UnconnectedState:
      // exit thread
      quit();
      break;
   case QTcpSocket::HostLookupState:
      break;
   case QTcpSocket::ConnectingState:
      break;
   case QTcpSocket::ConnectedState:
      // connection established
      mResult = true;
      mSocket->deleteLater();
      mSocket = nullptr;
      break;
   case QTcpSocket::BoundState:
      break;
   case QTcpSocket::ListeningState:
      break;
   case QTcpSocket::ClosingState:
      break;
   default:
      SR_ASSERT(0 && "unhandled case");
   }
}

void Searcher::onTimeout()
{
   // timeout occured
   mSocket->deleteLater();
   mSocket = nullptr;
   quit();
}
