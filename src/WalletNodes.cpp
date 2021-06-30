// Copyright (c) 2019 Niobio Cash developers <helder.garcia@gmail.com>
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QSettings>
#include <QByteArray>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "WalletNodes.h"
#include "Settings.h"
#include <iostream>

using namespace WalletGui;

WalletNodes::WalletNodes(QObject *parent) :
    QObject(parent)
    , myNetManager(new QNetworkAccessManager(this))
    , myNetReply(nullptr)
{
}

WalletNodes::~WalletNodes()
{
}

void WalletNodes::GetWalletNodes()
{
  if(myNetManager->networkAccessible() == QNetworkAccessManager::Accessible)
  {
    const QUrl WALLETNODES_LIST_URL("https://raw.githubusercontent.com/quan-projects/wallet-support-files/master/wallet-nodes.txt");
    myNetReply = myNetManager->get(QNetworkRequest(WALLETNODES_LIST_URL));
    connect(myNetReply, &QNetworkReply::finished, this, &WalletNodes::OnListReadFinished);
  }
}

void WalletNodes::OnListReadFinished()
{
  QString result = myNetReply->readAll().data();
  result = result.trimmed();
  listOfNodes = result.split("\n");
  QStringList currentRpcNodesList = Settings::instance().getRpcNodesList();
  Q_FOREACH (const QString& node, listOfNodes) {
    if (!currentRpcNodesList.contains(node)) {
      currentRpcNodesList << node;
    }
  }
  Settings::instance().setRpcNodesList(currentRpcNodesList);
  saveSettings(currentRpcNodesList);
	NetworkCleanup();
}

void WalletNodes::NetworkCleanup()
{
	myNetReply->deleteLater();
	myNetReply = nullptr;
}

void WalletNodes::saveSettings(const QStringList &_RpcNodesList) {
  QJsonArray wn_array;
  QString url_str("url");
  QString fee_str("fee");
  QString latency_str("latency");
  QString message_str("message");
  for(auto item : _RpcNodesList) {
      QJsonObject item_data;
      item_data.insert(url_str, QJsonValue(item));
      //item_data.insert(fee_str, QJsonValue("Loading"));
      item_data.insert(fee_str, QJsonValue(""));
      //item_data.insert(latency_str, QJsonValue("-"));
      item_data.insert(latency_str, QJsonValue(""));
      //item_data.insert(message_str, QJsonValue("-"));
      item_data.insert(message_str, QJsonValue(""));
      wn_array.push_back(QJsonValue(item_data));
  }
  QJsonObject final_object;
  final_object.insert(QString("remoteNodes"), QJsonValue(wn_array));
  QFile walletNodesFile(Settings::instance().getWalletNodesFile());
  if (walletNodesFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    QJsonDocument cfg_doc(final_object);
    walletNodesFile.write(cfg_doc.toJson());
    walletNodesFile.close();
  }
}
