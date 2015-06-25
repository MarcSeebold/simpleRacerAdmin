#pragma once

#include <QAbstractListModel>
#include "ClientConnection.hh"
#include "Common.hh"

class ClientConnectionManager : public QAbstractListModel
{
   Q_OBJECT

public:
   ClientConnectionManager(QObject *_parent);
   SharedClientConnection makeNew();
   void remove(const ClientConnection *_client);

   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
   int rowCount(const QModelIndex &parent = QModelIndex()) const override;

public slots:
   void onClientStateChanged();

private:
   std::vector<SharedClientConnection> mClients;
};
