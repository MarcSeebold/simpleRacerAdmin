#include "MainWindow.hh"
#include "ui_MainWindow.h"
#include "ClientConnection.hh"
#include <cassert>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::refreshClientList()
{
   ui->connectedClients->clear();
   for (const auto &c : mClients)
   {
      ui->connectedClients->addItem(c->getAddress().toString());
   }
}

void MainWindow::on_actionExit_triggered()
{
   qApp->exit();
}

void MainWindow::on_actionConnect_to_triggered()
{
   // Get ip
   auto host = QInputDialog::getText(this, "Enter IP", "Enter IP/Hostname to connect to.");
   auto client = new ClientConnection(this);
   client->connectTo(QHostAddress(host), 13337);
   mClients.push_back(client);
   // refresh list
   refreshClientList();
   ui->console->addItem("Connecting to " + host);
   // connect signals/slots
   connect(client, &ClientConnection::connected, this, &MainWindow::onClientConnected);
   connect(client, &ClientConnection::disconnected, this, &MainWindow::onClientDisconnected);
   connect(client, &ClientConnection::received, this, &MainWindow::onClientData);
}

void MainWindow::onClientConnected()
{
   auto s = sender();
   auto client = dynamic_cast<ClientConnection*>(s);
   if (!client)
   {
      assert(0 && "Error 0x02");
      return;
   }
   ui->console->addItem("Connected to " + client->getAddress());
}

void MainWindow::onClientDisconnected()
{
   auto s = sender();
   auto client = dynamic_cast<ClientConnection*>(s);
   if (!client)
   {
      assert(0 && "Error 0x03");
      return;
   }
   auto addr= client->getAddress();
   // remove from list
   mClients.erase(std::remove(mClients.begin(), mClients.end(), client), mClients.end());
   // delete
   delete client;
   // refresh list
   refreshClientList();
   // console
   ui->console->addItem("Disconnected from " + addr);
}

void MainWindow::onClientData(const QByteArray &_data)
{
   auto s = sender();
   auto client = dynamic_cast<ClientConnection*>(s);
   if (!client)
   {
      assert(0 && "Error 0x01");
      return;
   }
   ui->console->addItem("Received from " + client->getAddress()+ ": " + QString(_data));
}
