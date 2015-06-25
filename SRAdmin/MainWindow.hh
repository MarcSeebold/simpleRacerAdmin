#pragma once

#include <QMainWindow>
#include <vector>
#include "ClientConnectionManager.hh"

class ClientConnection;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindow(QWidget *parent = 0);
   ~MainWindow();

private slots:
   void on_actionExit_triggered();
   void on_actionConnect_to_triggered();
   void onClientConnected();
   void onClientDisconnected();
   void onClientData(const QByteArray &_data);
   void onClientStateChanged(ClientState _newState);
   void on_pushButton_clicked();
   void on_pushButton_2_clicked();
   void on_pushButton_4_clicked();
   void on_pushButton_5_clicked();
   void on_pushButton_3_clicked();
   void on_pushButton_6_clicked();

   void on_pushButton_7_clicked();

private:
   Ui::MainWindow *ui;
   ClientConnectionManager mClientManager;
};
