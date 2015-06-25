#include "MainWindow.hh"
#include "ui_MainWindow.h"
#include <cassert>
#include <QInputDialog>
#include "Common.hh"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), mClientManager(this)
{
   ui->setupUi(this);
   ui->clients->setModel(&mClientManager);
   ui->comboBox->addItems({"German", "English"});
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
      SR_ASSERT(0);
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
      SR_ASSERT(0);
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
      SR_ASSERT(0);
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
      SR_ASSERT(0);
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

void MainWindow::on_pushButton_clicked()
{
   // start test-play
   for (const _ &c : mClientManager.getClients())
   {
      c->sendCommand(NetworkCommand::START_TESTPLAY);
   }
}

void MainWindow::on_pushButton_2_clicked()
{
   // start pre-study
   for (const _ &c : mClientManager.getClients())
   {
      c->sendCommand(NetworkCommand::OPEN_SURVEY_PREGAME);
   }
}

void MainWindow::on_pushButton_4_clicked()
{
   // start ingame-study
   for (const _ &c : mClientManager.getClients())
   {
      c->sendCommand(NetworkCommand::OPEN_SURVEY_INGAME);
   }
}

void MainWindow::on_pushButton_5_clicked()
{
   // start postgame-study
   for (const _ &c : mClientManager.getClients())
   {
      c->sendCommand(NetworkCommand::OPEN_SURVEY_POSTGAME);
   }
}

void MainWindow::on_pushButton_3_clicked()
{
   // set condition
   for (const _ &c : mClientManager.getClients())
   {
      NetworkCommand setConditionCmd = NetworkCommand::INVALID;
      switch (ui->spinBox->value())
      {
      case 1:
         setConditionCmd = NetworkCommand::LOAD_CONDITION_1;
         break;
      case 2:
         setConditionCmd = NetworkCommand::LOAD_CONDITION_2;
         break;
      case 3:
         setConditionCmd = NetworkCommand::LOAD_CONDITION_3;
         break;
      case 4:
         setConditionCmd = NetworkCommand::LOAD_CONDITION_4;
         break;
      case 5:
         setConditionCmd = NetworkCommand::LOAD_CONDITION_5;
         break;
      case 6:
         setConditionCmd = NetworkCommand::LOAD_CONDITION_6;
         break;
      case 7:
         setConditionCmd = NetworkCommand::LOAD_CONDITION_7;
         break;
      case 8:
         setConditionCmd = NetworkCommand::LOAD_CONDITION_8;
         break;
      case 9:
         setConditionCmd = NetworkCommand::LOAD_CONDITION_9;
         break;
      case 10:
         setConditionCmd = NetworkCommand::LOAD_CONDITION_10;
         break;
      default:
         SR_ASSERT(0 && "unhandled case");
         break;
      }
      c->sendCommand(setConditionCmd);
   }
}
void MainWindow::on_pushButton_6_clicked()
{
   // start game
   for (const _ &c : mClientManager.getClients())
   {
      c->sendCommand(NetworkCommand::START_GAME);
   }
}

void MainWindow::on_pushButton_7_clicked()
{
   // set survey language
   const _ v = ui->comboBox->currentText();
   for (const _ &c : mClientManager.getClients())
   {
      if (v == "German")
         c->sendCommand(NetworkCommand::SET_SURVEY_LANG_GER);
      else if (v == "English")
         c->sendCommand(NetworkCommand::SET_SURVEY_LANG_ENG);
      else
         SR_ASSERT(0 && "unhandled case");
   }
}
