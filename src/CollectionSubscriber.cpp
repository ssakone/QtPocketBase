#include "CollectionSubscriber.h"
#include "PocketBaseSettings.h"
#include "qjsonarray.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include <QTimer>

CollectionSubscriber::CollectionSubscriber(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager(this);
}

void CollectionSubscriber::subscribe(const QString pattern, const QJSValue callback, QString id)
{
    for (auto& [key, item] : callbacks.toStdMap()) {
        if (key == id) {
            callbacks[key] = item;
            goto processRequest;
        }
    }

    callbacks.insert(id, callback);

    if (idRoutes.contains(pattern)) {
        QStringList ids = idRoutes[pattern];
        if (!ids.contains(id)) {
            ids.append(id);
            idRoutes[pattern] = ids;
        }
    } else {
        idRoutes.insert(pattern, QStringList{id});
    }

    processRequest:
    subscribeId(pattern);
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

    QNetworkReply *reply = manager->get(subscriptionRequest);

    QObject::connect(reply, &QNetworkReply::readyRead, [=](){
        QString data = reply->readAll();
        if (data.contains("clientId")) {
            clientId = data.split("id:")[1].split("\n")[0];
            m_connected = true;

            if (!idRoutes.isEmpty()) {
                subscribeId("");
            }

            emit connetionEstablished();
        } else {
            QString queryResponse = parseResponse(data);
            QString event = data.split("event:")[1].split("\n")[0];
            QStringList patterns = idRoutes[event];
            for (auto& patternId : patterns) {
                QJSValue caller = callbacks.value(patternId);
                if (caller.isCallable())
                    caller.call(QJSValueList{queryResponse});
            }

        }
    });

    QObject::connect(reply, &QNetworkReply::finished, [=](){
        connect();
    });
}

void CollectionSubscriber::subscribeId(QString pattern)
{
    QString url = PocketBaseSettings::getApiUrl();
    QString token = PocketBaseSettings::getToken();

    QNetworkRequest request = QNetworkRequest(url + "/api/realtime");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", QString(token).toUtf8());
    request.setRawHeader("Accept", "*/*");

    QJsonArray patterns = {};
    for (auto& [key, item] : idRoutes.toStdMap()) {
        patterns.push_back(key);
    }

    QJsonObject json;
    json.insert("clientId", clientId);
    json.insert("subscriptions", patterns);

    QJsonDocument doc(json);
    QNetworkReply *reply = manager->post(request, doc.toJson());
    QAbstractSocket::connect(reply, &QNetworkReply::finished, [=](){
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QString data = reply->readAll();
        if (statusCode != 204) 
            qDebug() << "error: " << data;
    });
}

QString CollectionSubscriber::parseResponse(QString reply)
{
    reply = reply.split("data:")[1].split("\n\n")[0];
    return reply;
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
