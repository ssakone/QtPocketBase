#ifndef COLLECTIONSUBSCRIBER_H
#define COLLECTIONSUBSCRIBER_H

#include <QJSValue>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

class CollectionSubscriber : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged FINAL)
public:
    explicit CollectionSubscriber(QObject *parent = nullptr);

    Q_INVOKABLE QString subscribe(const QString pattern, const QJSValue callback, QString id);
    Q_INVOKABLE void unsubscribe(const QString id);
    Q_INVOKABLE void connect();
    void sub(QString pattern, bool all = false);

    bool connected() const;
    void setConnected(bool newConnected);

private:
    QMap<QString, QStringList> callbacks;
    QMap<QString, QJSValue> identifiers;
    QStringList subscriptionList = {};
    QNetworkRequest subscriptionRequest;
    QNetworkAccessManager *manager;
    QString clientId;
    bool m_connected = false;


signals:
    void connectedChanged();
    void connetionEstablished();
};

#endif // COLLECTIONSUBSCRIBER_H
