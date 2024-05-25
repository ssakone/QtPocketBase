#ifndef POCKETREQUEST_H
#define POCKETREQUEST_H

#include "PocketBaseCollectionPromise.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>

class PocketRequest : public QObject
{
    Q_OBJECT
public:
    explicit PocketRequest(QObject *parent = nullptr);

    enum class HttpMethod {
        GET,
        POST,
        PATCH,
        PUT,
        DELETE
    };


    Q_INVOKABLE PocketBaseCollectionPromise *getOne(QString id, QJsonObject options = {});
    Q_INVOKABLE PocketBaseCollectionPromise *getList(int page = 1, int perPage = 15, QJsonObject options = {});
    Q_INVOKABLE PocketBaseCollectionPromise *create(QJSValue data, QJsonObject options = {});
    PocketBaseCollectionPromise *create(QJsonObject  data, QJsonObject options = {});
    Q_INVOKABLE PocketBaseCollectionPromise *createWithFile(QJSValue data,
                                                            QJSValue files, QJsonObject options = {});
    Q_INVOKABLE PocketBaseCollectionPromise *update(QString id, QJSValue data, QJsonObject options = {});
    PocketBaseCollectionPromise *update(QString id, QJsonObject data, QJsonObject options = {});
    Q_INVOKABLE PocketBaseCollectionPromise *updateWithFile(QString id, QJSValue data,
                                                            QJSValue files, QJsonObject options = {});
    Q_INVOKABLE PocketBaseCollectionPromise *deleteFile(QString id, QJSValue files, QJsonObject options = {});
    Q_INVOKABLE PocketBaseCollectionPromise *deleteOne(QString id, QJsonObject options = {});

    Q_INVOKABLE PocketBaseCollectionPromise *authWithPassword(QString identity, QString password);
    Q_INVOKABLE PocketBaseCollectionPromise *authRefresh();

    Q_INVOKABLE PocketBaseCollectionPromise *HttpGet(QString path, QJSValue options = {});
    Q_INVOKABLE PocketBaseCollectionPromise *HttpPost(QString path, QJSValue data, QJSValue options = {});
    Q_INVOKABLE PocketBaseCollectionPromise *HttpPost(QString path, QJsonObject datas, QJsonObject obj);
    Q_INVOKABLE PocketBaseCollectionPromise *HttpPut(QString path, QJSValue data, QJSValue options = {});
    PocketBaseCollectionPromise *HttpPut(QString path, QJsonObject datas);
    Q_INVOKABLE PocketBaseCollectionPromise *HttpPatch(QString path, QJSValue data, QJSValue options = {});
    Q_INVOKABLE PocketBaseCollectionPromise *HttpDelete(QString path, QJSValue options = {});

    Q_INVOKABLE PocketBaseCollectionPromise *makeRequest(QNetworkRequest request, HttpMethod method = HttpMethod::GET);
    Q_INVOKABLE PocketBaseCollectionPromise *makeRequest(QNetworkRequest request, HttpMethod method, QByteArray data);
    Q_INVOKABLE PocketBaseCollectionPromise *makeRequest(QNetworkRequest *request, HttpMethod method, QHttpMultiPart *multiPart);

    QNetworkRequest getRequest(QUrlQuery query, bool contentType = true, QString path = "", bool full = false);
    QUrlQuery getRequestParams(QJsonObject obj);

    void setRoute(const QString &newRoute);
    QString getRoute() const;

    QByteArray methodToString(HttpMethod method);

private:
    QString route;
    QNetworkAccessManager *manager;

};

#endif // POCKETREQUEST_H
