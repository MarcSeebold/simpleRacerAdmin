#pragma once

#include <QMainWindow>
#include <vector>

class ClientConnection;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindow(QWidget *parent = 0);
   ~MainWindow();

private:
   void refreshClientList();

private slots:
   void on_actionExit_triggered();
   void on_actionConnect_to_triggered();

   void onClientConnected();
   void onClientDisconnected();
   void onClientData(const QByteArray &_data);

private:
   Ui::MainWindow *ui;
   std::vector<ClientConnection*> mClients;
};
