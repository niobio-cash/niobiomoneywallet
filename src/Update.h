// Copyright (c) 2016 - 2019 Niobio Cash developers - Derived work from -Karbowanec-
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef UPDATE_H
#define UPDATE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

const static QString Q1VCOIN_UPDATE_URL = "https://raw.githubusercontent.com/quan-projects/quan-desktop-wallet/master/update.txt";

class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = 0);
    
     ~Updater() {
        delete manager;
}

    void checkForUpdate();

signals:
    
public slots:
    void replyFinished (QNetworkReply *reply);

private:
   QNetworkAccessManager *manager;

};

#endif // UPDATE_H
