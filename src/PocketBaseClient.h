#ifndef POCKETBASECLIENT_H
#define POCKETBASECLIENT_H

#include "CollectionSubscriber.h"
#include "PocketBaseCollectionPromise.h"
#include "PocketBaseCollection.h"

#include <QObject>
#include <QRandomGenerator>
#include <QSettings>
#include <QSharedPointer>

class PocketBaseClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString apiUrl READ apiUrl WRITE setApiUrl NOTIFY apiUrlChanged FINAL)
    Q_PROPERTY(QList<PocketBaseCollection *> *collectionList READ collectionList WRITE setCollectionList NOTIFY collectionListChanged FINAL)
    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged FINAL)
    Q_PROPERTY(bool healthy READ healthy WRITE setHealthy NOTIFY healthyChanged FINAL)
    Q_PROPERTY(QString authToken READ authToken NOTIFY authTokenChanged FINAL)
    Q_PROPERTY(bool isAuth READ isAuth NOTIFY isAuthChanged FINAL)
public:
    explicit PocketBaseClient(QObject *parent = nullptr);

    Q_INVOKABLE void addCollection(PocketBaseCollection *collection);
    Q_INVOKABLE PocketBaseCollectionPromise *updateCollection(QString collectionId, PocketBaseCollection *updateCollection);
    Q_INVOKABLE PocketBaseCollectionPromise *createCollection(PocketBaseCollection *newCollection);
    Q_INVOKABLE PocketBaseCollectionPromise *deleteCollection(QString collectionId);
    Q_INVOKABLE PocketBaseCollectionPromise *getCollection(QString collectionId);
    Q_INVOKABLE PocketBaseCollectionPromise *importCollection(QJSValue json);
    Q_INVOKABLE PocketBaseCollectionPromise *update(QString collectionName, QString id, QJSValue data, QJSValue options = {});
    Q_INVOKABLE PocketBaseCollectionPromise *getCollections();
    Q_INVOKABLE PocketBaseCollectionPromise *isHealthy();

    Q_INVOKABLE PocketBaseCollection *collection(const QString &collectionName);

    Q_INVOKABLE PocketBaseCollectionPromise *authAdminWithPassword(QString identity, QString password);
    Q_INVOKABLE PocketBaseCollectionPromise *authWithCollection(QString collectionName, QString identity, QString password);
    Q_INVOKABLE PocketBaseCollectionPromise *authRefreshCollection(QString collectionName);
    Q_INVOKABLE PocketBaseCollectionPromise *createNewAdmin(QString email, QString identity, QString password);

    // Q_INVOKABLE QString getFileUrl(QString fileId);

    Q_INVOKABLE void clearToken();
    Q_INVOKABLE void logout();

    Q_INVOKABLE void subscribe(const QString pattern, const QJSValue callback, QString id = QString::number(QRandomGenerator::global()->generate()).toUtf8().toBase64().mid(0, 10));
    Q_INVOKABLE void unsubscribe(const QString id);

    QString apiUrl() const;
    void setApiUrl(const QString &newApiUrl);

    void setCollectionList(QList<PocketBaseCollection *> *newCollectionList);
    QList<PocketBaseCollection *> *collectionList() const;

    QString authToken() const;
    void setAuthToken(const QString &newAuthToken);

    bool connected() const;
    void setConnected(bool newConnected);

    bool healthy() const;
    void setHealthy(bool newHealthy);

    bool isAuth() const;

signals:
    void apiUrlChanged();

    void collectionChanged();

    void collectionListChanged();

    void authTokenChanged();

    void connectedChanged();

    void healthyChanged();

    void isAuthChanged();

private:
    QString m_apiUrl;
    PocketRequest request;

    CollectionSubscriber *subscriber;
    QSettings *settings;
    QList<PocketBaseCollection *> *m_collectionList = nullptr;
    QMap<QString, QSharedPointer<PocketBaseCollection>> m_collectionMap;
    QString m_authToken;
    bool m_connected;
    bool m_healthy = false;
    bool previousHealthState = false;
    bool m_isAuth;
};

#endif // POCKETBASECLIENT_H
