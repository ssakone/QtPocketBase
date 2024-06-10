#include "CollectionSubscriber.h"
#include "PocketBaseSettings.h"
#include "qjsonarray.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include <QTimer>
#include <QPointer>

CollectionSubscriber::CollectionSubscriber(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager(this);
}

QString CollectionSubscriber::subscribe(const QString topic, const QJSValue callback, QString id = "")
{
    if (id == "") {
        id = QUuid::createUuid().toString();
    }

    if (!callbacks.contains(topic)) {
        callbacks.insert(topic, {id});
    }

    auto callbackList = callbacks[topic];
    if (!callbackList.contains(id)) {
        callbackList.append(id);
        callbacks[topic] = callbackList;
    }

    identifiers.insert(id, callback);

    if (!subscriptionList.contains(topic))
    {
        subscriptionList.append(topic);
        if (m_connected)
            sub(topic);            
    }

    return id;
}

void CollectionSubscriber::unsubscribe(const QString id)
{
    for (auto topic : callbacks.keys()) {
        QStringList callbackList = callbacks[topic];
        if (callbackList.contains(id)) {
            callbackList.removeOne(id);
            callbacks[topic] = callbackList;
        }

        if (callbackList.isEmpty()) {
            subscriptionList.removeOne(topic);
        }
    }

    identifiers.remove(id);
}

void CollectionSubscriber::connect()
{
    QString url = PocketBaseSettings::getApiUrl();
    QString token = PocketBaseSettings::getToken();
    QNetworkRequest subscriptionRequest = QNetworkRequest(QUrl(url + "/api/realtime"));
    subscriptionRequest.setRawHeader("accept", "text/event-stream");
    subscriptionRequest.setRawHeader("Authorization", token.toUtf8());
    subscriptionRequest.setAttribute(QNetworkRequest::RedirectionTargetAttribute, true);
    subscriptionRequest.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);

    // manager->setTransferTimeout(2000);
    QNetworkReply *reply = manager->get(subscriptionRequest);

    QObject::connect(reply, &QNetworkReply::readyRead, [=](){
        QString data = reply->readAll();


        if (data.contains("clientId")) {
            clientId = data.split("id:")[1].split("\n")[0];
            setConnected(true);
            // qDebug() << clientId;
            // qDebug() << "connecting to subscription" << subscriptionList;
            sub("", true);
            emit connetionEstablished();
        } else {
            QStringList parts = data.split("\n\n");
            for (auto part : parts) {
                if (part.isEmpty())
                    continue;

                QString queryResponse = part.split("data:")[1];
                QString event = part.split("event:")[1].split("\n")[0];
                QStringList patterns = callbacks[event];
                for (QString identifier : patterns) {
                    QJSValue caller = identifiers[identifier];
                    if (caller.isCallable())
                        caller.call(QJSValueList{queryResponse});
                    else {
                        identifiers.remove(identifier);
                        patterns.removeOne(identifier);
                        callbacks[event] = patterns;
                        qDebug() << "Callback not callable : " << caller.toString();
                    }
                }
            }
        }
    });

    QObject::connect(reply, &QNetworkReply::errorOccurred, [=]() {
        QString data = reply->readAll();
        // qDebug() << "Error: " << data;
        delete reply;
    });

    QObject::connect(reply, &QNetworkReply::finished, [=](){
        connect();
    });
}

void CollectionSubscriber::sub(QString topic, bool all)
{

    QString url = PocketBaseSettings::getApiUrl();
    QString token = PocketBaseSettings::getToken();

    QNetworkRequest request = QNetworkRequest(url + "/api/realtime");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", QString(token).toUtf8());
    request.setRawHeader("Accept", "*/*");

    QJsonArray subs;
    for (QString topic : subscriptionList) {
        subs.append(topic);
    }

    QJsonObject json;
    json.insert("clientId", clientId);
    json.insert("subscriptions", subs);


    QJsonDocument doc(json);
    // qDebug () << "Sending subscription request" << doc.toJson();
    QNetworkReply *subReply = manager->post(request, doc.toJson());
    QObject::connect(subReply, &QNetworkReply::finished, [=](){
        int statusCode = subReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QString data = subReply->readAll();
        if (statusCode != 204)
        {
            qWarning() << "error: " << data;
            if (data.contains("invalid client")) {
                connect();
            }
        }
    });
}

bool CollectionSubscriber::connected() const
{
    return m_connected;
}

void CollectionSubscriber::setConnected(bool newConnected)
{
    if (m_connected == newConnected)
        return;
    m_connected = newConnected;
    emit connectedChanged();
}
