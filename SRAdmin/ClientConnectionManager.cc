#include "ClientConnectionManager.hh"
ClientConnectionManager::ClientConnectionManager(QObject *_parent) : QAbstractListModel(_parent)
{
}

SharedClientConnection ClientConnectionManager::makeNew()
{
   _ c = std::make_shared<ClientConnection>(this);
   mClients.push_back(c);
   insertRow(rowCount());
   connect(c.get(), &ClientConnection::stateChanged, this, &ClientConnectionManager::onClientStateChanged);
   return c;
}

void ClientConnectionManager::remove(const ClientConnection *_client)
{
   removeRow(rowCount());
   for (_ it = mClients.begin(); it != mClients.end(); ++it)
   {
      if ((*it).get() == _client)
      {
         mClients.erase(it);
         return;
      }
   }
}

QVariant ClientConnectionManager::data(const QModelIndex &index, int role) const
{
   if (!index.isValid())
      return QVariant();
   if (index.row() >= (int)mClients.size())
      return QVariant();

   if (role == Qt::DisplayRole)
   {
      const _ c = mClients.at(index.row());
      return c->getAddress() + ": " + c->getStateString();
   }
   return QVariant();
}

int ClientConnectionManager::rowCount(const QModelIndex &parent) const
{
   if (parent.isValid())
      return 0;
   return mClients.size()+1; // why the hell +1???
}

void ClientConnectionManager::onClientStateChanged()
{
   emit dataChanged(index(0), index(rowCount()));
}
