#include "MainWindow.hh"
#include "ui_MainWindow.h"
#include <cassert>
#include <QInputDialog>
#include "Common.hh"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), mClientManager(this)
{
   ui->setupUi(this);
   ui->clients->setModel(&mClientManager);
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::on_actionExit_triggered()
{
   qApp->exit();
}

void MainWindow::on_actionConnect_to_triggered()
{
   // Get ip
   _ host = QInputDialog::getText(this, "Enter IP", "Enter IP/Hostname to connect to.");
   _ client = mClientManager.makeNew();
   client->connectTo(QHostAddress(host), 13337);
   ui->console->addItem("Connecting to " + host);
   // connect signals/slots
   connect(client.get(), &ClientConnection::connected, this, &MainWindow::onClientConnected);
   connect(client.get(), &ClientConnection::disconnected, this, &MainWindow::onClientDisconnected);
   connect(client.get(), &ClientConnection::received, this, &MainWindow::onClientData);
}

void MainWindow::onClientConnected()
{
   _ s = sender();
   _ client = dynamic_cast<ClientConnection *>(s);
   if (!client)
   {
      assert(0 && "Error 0x02");
      return;
   }
   ui->console->addItem("Connected to " + client->getAddress());
}

void MainWindow::onClientDisconnected()
{
   _ s = sender();
   _ client = dynamic_cast<ClientConnection *>(s);
   if (!client)
   {
      assert(0 && "Error 0x03");
      return;
   }
   _ addr = client->getAddress();
   // remove from list
   mClientManager.remove(client);
   // console
   ui->console->addItem("Disconnected from " + addr);
}

void MainWindow::onClientData(const QByteArray &_data)
{
   _ s = sender();
   _ client = dynamic_cast<ClientConnection *>(s);
   if (!client)
   {
      assert(0 && "Error 0x01");
      return;
   }
   ui->console->addItem("Received from " + client->getAddress() + ": " + QString(_data));
}

void MainWindow::onClientStateChanged(ClientState _newState)
{
   _ s = sender();
   _ client = dynamic_cast<ClientConnection *>(s);
   if (!client)
   {
      assert(0 && "Error 0x03");
      return;
   }
   _ addr = client->getAddress();
   QString state;
   switch (_newState)
   {
   case ClientState::PLAYING:
      state = "PLAYING";
      break;
   case ClientState::SURVEY:
      state = "SURVEY";
      break;
   case ClientState::WAITING:
      state = "WAITING";
      break;
   default:
      break;
   }
}
