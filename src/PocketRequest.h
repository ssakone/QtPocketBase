#ifndef POCKETREQUEST_H
#define POCKETREQUEST_H

#include "PocketBaseCollectionPromise.h"
#include <QNetworkRequest>
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

    Q_INVOKABLE PocketBaseCollectionPromise *HttpGet(QString path, QJSValue options = {}, bool other = false);
    Q_INVOKABLE PocketBaseCollectionPromise *HttpPostFile(QString path, QJSValue data, QJSValue files, QJSValue options = {}, bool other = false);
    Q_INVOKABLE PocketBaseCollectionPromise *HttpPost(QString path, QJSValue data, QJSValue options = {}, bool other = false);
    Q_INVOKABLE PocketBaseCollectionPromise *HttpPost(QString path, QJsonObject datas, QJsonObject obj);
    Q_INVOKABLE PocketBaseCollectionPromise *HttpPut(QString path, QJSValue data, QJSValue options = {});
    PocketBaseCollectionPromise *HttpPut(QString path, QJsonObject datas);
    Q_INVOKABLE PocketBaseCollectionPromise *HttpPatch(QString path, QJSValue data, QJSValue options = {});
    Q_INVOKABLE PocketBaseCollectionPromise *HttpDelete(QString path, QJSValue options = {});

    Q_INVOKABLE PocketBaseCollectionPromise *makeRequest(QNetworkRequest request, HttpMethod method = HttpMethod::GET);
    Q_INVOKABLE PocketBaseCollectionPromise *makeRequest(QNetworkRequest request, HttpMethod method, QByteArray data);
    Q_INVOKABLE PocketBaseCollectionPromise *makeRequest(QNetworkRequest *request, HttpMethod method, QHttpMultiPart *multiPart);


    Q_INVOKABLE PocketBaseCollectionPromise *get(QString url, QJSValue headers = {});
    Q_INVOKABLE PocketBaseCollectionPromise *post(QString url, QJSValue data, QJSValue headers = {});
    Q_INVOKABLE PocketBaseCollectionPromise *put(QString url, QJSValue data, QJSValue headers = {});
    Q_INVOKABLE PocketBaseCollectionPromise *postFile(QString url, QJSValue data, QJSValue files, QJSValue headers = {});
    Q_INVOKABLE PocketBaseCollectionPromise *putFile(QString url, QJSValue data, QJSValue files, QJSValue headers = {});
    Q_INVOKABLE PocketBaseCollectionPromise *removeFile(QString url, QJSValue files, QJSValue headers = {});
    Q_INVOKABLE PocketBaseCollectionPromise *patch(QString url, QJSValue data, QJSValue headers = {});
    Q_INVOKABLE PocketBaseCollectionPromise *del(QString url, QJSValue headers = {});
    Q_INVOKABLE PocketBaseCollectionPromise *downloadFile(const QString &url, const QString &saveDir, QJSValue headers = {});



    QNetworkRequest getRequest(QUrlQuery query, bool contentType = true, QString path = "", bool full = false, bool other = false);
    QUrlQuery getRequestParams(QJsonObject obj);

    void setRoute(const QString &newRoute);
    QString getRoute() const;

    QByteArray methodToString(HttpMethod method);

private:
    QString route;
    QNetworkAccessManager *manager;

};


// PocketRequest.get("https://buma.pockethost.io/api/collections/articles/records").then(function (rs) {
//     console.log(rs)
// }).error(function (err) {
//     console.log(err)
// })
// const data = {
//     "name": "Sakone"
// }

// const file = {
//     "photo": ["logo_a3dy_pcnemu_tQ75szBOlC.png"]
// }

// PocketRequest.removeFile("https://buma.pockethost.io/api/collections/test_table/records/whz0uooookm2eb6", file).then(function (rs) {
//     console.log(rs)
// }).error(function (err) {
//     console.log(err)
// })

#endif // POCKETREQUEST_H
