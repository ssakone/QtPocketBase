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
    if (id == "")
    {
        id = QUuid::createUuid().toString();
    }

    if (!callbacks.contains(topic))
    {
        callbacks.insert(topic, {id});
    }

    auto callbackList = callbacks[topic];
    if (!callbackList.contains(id))
    {
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
    for (auto topic : callbacks.keys())
    {
        QStringList callbackList = callbacks[topic];
        if (callbackList.contains(id))
        {
            callbackList.removeOne(id);
            callbacks[topic] = callbackList;
        }

        if (callbackList.isEmpty())
        {
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

    QObject::connect(reply, &QNetworkReply::readyRead, this, [=]() {
        QString data = reply->readAll();

        // If we see "clientId", parse out the ID if possible
        if (data.contains("clientId")) {
            // Safely split by "id:"
            const QStringList idSplit = data.split("id:");
            if (idSplit.size() > 1) {
                const QStringList newLineSplit = idSplit[1].split("\n");
                if (!newLineSplit.isEmpty()) {
                    clientId = newLineSplit.first();
                }
            }
            setConnected(true);
            sub("", true);
            emit connetionEstablished();
        } else {
            // If not the clientId info, then we parse SSE "event:" lines
            QStringList parts = data.split("\n\n");
            for (const QString &part : parts) {
                if (part.isEmpty()) {
                    continue;
                }

                // 1) Safely parse the "event"
                QString event;
                {
                    const QStringList eventSplit = part.split("event:");
                    if (eventSplit.size() > 1) {
                        // e.g. "event:xyz\n..."
                        // That second piece might have more newlines
                        const QStringList newLineSplit = eventSplit[1].split("\n");
                        if (!newLineSplit.isEmpty()) {
                            event = newLineSplit.first().trimmed();
                        }
                    }
                }

                // 2) Safely parse the "data"
                QString dataField;
                {
                    const QStringList dataSplit = part.split("data:");
                    if (dataSplit.size() > 1) {
                        dataField = dataSplit[1].trimmed();
                    }
                }

                // If we couldn’t parse the "event" or "data", we skip
                if (event.isEmpty() || dataField.isEmpty()) {
                    // Maybe debug-log or ignore
                    continue;
                }

                // If we do have a recognized event, call its callback
                if (callbacks.contains(event)) {
                    QStringList patterns = callbacks[event];
                    for (auto &identifier : patterns) {
                        QJSValue caller = identifiers.value(identifier);
                        if (caller.isCallable()) {
                            caller.call(QJSValueList{ dataField });
                        } else {
                            // Callback not callable => remove it to avoid repeated invalid calls
                            identifiers.remove(identifier);
                            patterns.removeOne(identifier);
                            callbacks[event] = patterns;
                            qDebug() << "Callback not callable : " << caller.toString();
                        }
                    }
                } else {
                    qWarning() << "Event not found in callbacks:" << event;
                }
            }
        }
    });


    QObject::connect(reply, &QNetworkReply::errorOccurred, [reply, this]()
                     {
                         QString data = reply->readAll();
                         // qDebug() << "Error: " << data;
                     });

    QObject::connect(reply, &QNetworkReply::finished, this, [=]() {
        reply->deleteLater();
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
    for (QString topic : subscriptionList)
    {
        subs.append(topic);
    }

    QJsonObject json;
    json.insert("clientId", clientId);
    json.insert("subscriptions", subs);

    QJsonDocument doc(json);
    // qDebug () << "Sending subscription request" << doc.toJson();
    QNetworkReply *subReply = manager->post(request, doc.toJson());
    QObject::connect(subReply, &QNetworkReply::finished, this, [=]()
                     {
        int statusCode = subReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QString data = subReply->readAll();
        if (statusCode != 204)
        {
            qWarning() << "error: " << data;
            if (data.contains("invalid client")) {
                connect();
            }
        } });
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
