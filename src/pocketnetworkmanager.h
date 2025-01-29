#ifndef POCKETNETWORKMANAGER_H
#define POCKETNETWORKMANAGER_H

#include <QObject>
#include <QtNetwork/qnetworkproxy.h>
#include <qnetworkaccessmanager.h>

class PocketNetworkManager
{
public:

    static QNetworkAccessManager* instance() {
        static QNetworkAccessManager manager;

        return &manager;
    }
    static QNetworkAccessManager* instance2() {
        static QNetworkAccessManager manager;

        return &manager;
    }
private:
    PocketNetworkManager();
    ~PocketNetworkManager();
    PocketNetworkManager(const PocketNetworkManager&) = delete;
    PocketNetworkManager& operator=(const PocketNetworkManager&) = delete;
};

#endif // POCKETNETWORKMANAGER_H
