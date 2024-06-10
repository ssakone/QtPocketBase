#include "PocketRequest.h"
#include "PocketBaseClient.h"
#include "PocketUtility.h"
#include "qjsondocument.h"
#include "qurlquery.h"

PocketRequest::PocketRequest(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager(this);

}

PocketBaseCollectionPromise *PocketRequest::getOne(QString id, QJsonObject options)
{
    return makeRequest(getRequest(getRequestParams(options), false, "/" + id), HttpMethod::GET);
}

PocketBaseCollectionPromise *PocketRequest::getList(int page, int perPage, QJsonObject options)
{
    options.insert("page", QString::number(page));
    options.insert("perPage", QString::number(perPage));
    QNetworkRequest request = getRequest(getRequestParams(options), false);

    return makeRequest(request);
}

PocketBaseCollectionPromise *PocketRequest::create(QJSValue data, QJsonObject options)
{
    QJsonObject *obj = PocketUtility::jsvalueToJsonObject(data);
    return makeRequest(getRequest(getRequestParams(options), true), HttpMethod::POST, QJsonDocument(*obj).toJson());
}

PocketBaseCollectionPromise *PocketRequest::create(QJsonObject data, QJsonObject options)
{
    return makeRequest(getRequest(getRequestParams(options), true), HttpMethod::POST, QJsonDocument(data).toJson());
}

PocketBaseCollectionPromise *PocketRequest::createWithFile(QJSValue data, QJSValue files, QJsonObject options)
{
    QJsonObject *obj = PocketUtility::jsvalueToJsonObject(data);
    QJsonObject fobj = files.toVariant().toJsonObject();

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    PocketUtility::jsonToFormData(multiPart, *obj);
    PocketUtility::jsonFilesToFormData(multiPart, fobj);
    QNetworkRequest request = getRequest(getRequestParams(options), false);
    return makeRequest(&request, HttpMethod::POST, multiPart);
}

PocketBaseCollectionPromise *PocketRequest::update(QString id, QJSValue data, QJsonObject options)
{
    QJsonObject *obj = PocketUtility::jsvalueToJsonObject(data);
    return makeRequest(getRequest(getRequestParams(options), true, "/" + id), HttpMethod::PATCH, QJsonDocument(*obj).toJson());
}

PocketBaseCollectionPromise *PocketRequest::update(QString id, QJsonObject data, QJsonObject options)
{
    return makeRequest(getRequest(getRequestParams(options), true, "/" + id), HttpMethod::PATCH, QJsonDocument(data).toJson());
}

PocketBaseCollectionPromise *PocketRequest::updateWithFile(QString id, QJSValue data, QJSValue files, QJsonObject options)
{
    QJsonObject *obj = PocketUtility::jsvalueToJsonObject(data);
    QJsonObject fobj = files.toVariant().toJsonObject();

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    PocketUtility::jsonToFormData(multiPart, *obj);
    PocketUtility::jsonFilesToFormData(multiPart, fobj);

    multiPart->dumpObjectTree();

    QNetworkRequest request = getRequest(getRequestParams(options), false, "/" + id);
    return makeRequest(&request, HttpMethod::PATCH, multiPart);
}

PocketBaseCollectionPromise *PocketRequest::deleteFile(QString id, QJSValue files, QJsonObject options)
{
    QJsonObject fobj = files.toVariant().toJsonObject();

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    PocketUtility::jsonFilesToFormData(multiPart, fobj, true);
    QNetworkRequest request = getRequest(getRequestParams(options), false, "/" + id);
    return makeRequest(&request, HttpMethod::PATCH, multiPart);
}

PocketBaseCollectionPromise *PocketRequest::deleteOne(QString id, QJsonObject options)
{
    return makeRequest(getRequest(getRequestParams(options), false, "/" + id), HttpMethod::DELETE);
}

PocketBaseCollectionPromise *PocketRequest::authWithPassword(QString identity, QString password)
{
    QJsonObject obj;
    obj.insert("identity", identity);
    obj.insert("password", password);
    return makeRequest(getRequest(getRequestParams({}), true), HttpMethod::POST, QJsonDocument(obj).toJson(QJsonDocument::Compact));
}

PocketBaseCollectionPromise *PocketRequest::authRefresh()
{
    return makeRequest(getRequest(getRequestParams({}), true), HttpMethod::POST);
}

PocketBaseCollectionPromise *PocketRequest::HttpGet(QString path, QJSValue options)
{
    QJsonObject *obj = PocketUtility::jsvalueToJsonObject(options);
    return makeRequest(getRequest(getRequestParams(*obj), false, path, true), HttpMethod::GET);
}

PocketBaseCollectionPromise *PocketRequest::HttpPost(QString path, QJSValue data, QJSValue options)
{
    QJsonObject *obj = PocketUtility::jsvalueToJsonObject(options);
    QJsonObject *datas = PocketUtility::jsvalueToJsonObject(data);
    return makeRequest(getRequest(getRequestParams(*obj), true, path, true), HttpMethod::POST, QJsonDocument(*datas).toJson());
}

PocketBaseCollectionPromise *PocketRequest::HttpPost(QString path, QJsonObject datas, QJsonObject obj)
{
    return makeRequest(getRequest(getRequestParams(obj), true, path, true), HttpMethod::POST, QJsonDocument(datas).toJson());
}

PocketBaseCollectionPromise *PocketRequest::HttpPut(QString path, QJSValue data, QJSValue options)
{
    QJsonObject *obj = PocketUtility::jsvalueToJsonObject(options);
    QJsonObject datas = data.toVariant().toJsonObject();
    return makeRequest(getRequest(QUrlQuery(), true, path, true), HttpMethod::PUT, QJsonDocument(datas).toJson());
}

PocketBaseCollectionPromise *PocketRequest::HttpPut(QString path, QJsonObject datas)
{
    return makeRequest(getRequest(QUrlQuery(), true, path, true), HttpMethod::PUT, QJsonDocument(datas).toJson());
}

PocketBaseCollectionPromise *PocketRequest::HttpPatch(QString path, QJSValue data, QJSValue options)
{
    QJsonObject *obj = PocketUtility::jsvalueToJsonObject(options);
    QJsonObject *datas = PocketUtility::jsvalueToJsonObject(data);
    return makeRequest(getRequest(getRequestParams(*obj), true, path, true), HttpMethod::PATCH, QJsonDocument(*datas).toJson());
}

PocketBaseCollectionPromise *PocketRequest::HttpDelete(QString path, QJSValue options)
{
    QJsonObject *obj = PocketUtility::jsvalueToJsonObject(options);
    return makeRequest(getRequest(getRequestParams(*obj), false, path, true), HttpMethod::DELETE);
}

PocketBaseCollectionPromise *PocketRequest::makeRequest(QNetworkRequest request, HttpMethod method)
{
    PocketBaseCollectionPromise *promise = new PocketBaseCollectionPromise(this);

    QNetworkReply *reply;

    reply = manager->sendCustomRequest(request, methodToString(method));

    connect(reply, &QNetworkReply::finished, [=]()
            {
        if (reply->error() == QNetworkReply::NoError) {
            promise->callThen(QJSValueList{QString(reply->readAll())});
        }else if (reply->error() != QNetworkReply::ConnectionRefusedError && reply->error() != QNetworkReply::HostNotFoundError && reply->error() != QNetworkReply::TimeoutError && reply->error() != QNetworkReply::UnknownNetworkError) {
            QJsonObject error;
            error.insert("code", reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
            error.insert("message", reply->errorString());
            error.insert("data", QString(reply->readAll()));
            promise->callError(QJSValueList{QString(QJsonDocument(error).toJson())});
        }
    });

    connect(reply, &QNetworkReply::errorOccurred, [=](QNetworkReply::NetworkError error)
            {
        if (error == QNetworkReply::ConnectionRefusedError || error == QNetworkReply::HostNotFoundError || error == QNetworkReply::TimeoutError || error == QNetworkReply::UnknownNetworkError) {
            QJsonObject errorJson;
            errorJson.insert("code", 0);
            errorJson.insert("message", reply->errorString());
            promise->callError(QJSValueList{QString(QJsonDocument(errorJson).toJson())});
        }
    });

    return promise;
}

PocketBaseCollectionPromise *PocketRequest::makeRequest(QNetworkRequest request, HttpMethod method, QByteArray data)
{
    PocketBaseCollectionPromise *promise = new PocketBaseCollectionPromise(this);

    QNetworkReply *reply;

    reply = manager->sendCustomRequest(request, methodToString(method), data);
    connect(reply, &QNetworkReply::finished, [reply, promise, this]()
            {
        if (reply->error() == QNetworkReply::NoError) {
            promise->callThen(QJSValueList{QString(reply->readAll())});
        }else if (reply->error() != QNetworkReply::ConnectionRefusedError && reply->error() != QNetworkReply::HostNotFoundError && reply->error() != QNetworkReply::TimeoutError && reply->error() != QNetworkReply::UnknownNetworkError) {
            QJsonObject error;
            QString data = reply->readAll();
            error.insert("code", reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
            error.insert("message", reply->errorString() + "," + data);
            error.insert("data", data);
            promise->callError(QJSValueList{QString(QJsonDocument(error).toJson())});
        }
    });

    connect(reply, &QNetworkReply::errorOccurred, [=](QNetworkReply::NetworkError error)
            {
        if (error == QNetworkReply::ConnectionRefusedError || error == QNetworkReply::HostNotFoundError || error == QNetworkReply::TimeoutError || error == QNetworkReply::UnknownNetworkError) {
            QJsonObject errorJson;
            errorJson.insert("code", reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
            errorJson.insert("message", reply->errorString() + reply->readAll());
            promise->callError(QJSValueList{QString(QJsonDocument(errorJson).toJson())});
        }
    });

    return promise;
}

PocketBaseCollectionPromise *PocketRequest::makeRequest(QNetworkRequest *request, HttpMethod method, QHttpMultiPart *multipart)
{
    PocketBaseCollectionPromise *promise = new PocketBaseCollectionPromise(this);

    QNetworkReply *reply;

    reply = manager->sendCustomRequest(*request, methodToString(method), multipart);

    connect(reply, &QNetworkReply::finished, [=]()
            {
        if (reply->error() == QNetworkReply::NoError) {
            promise->callThen(QJSValueList{QString(reply->readAll())});
        }  else if (reply->error() != QNetworkReply::ConnectionRefusedError && reply->error() != QNetworkReply::HostNotFoundError && reply->error() != QNetworkReply::TimeoutError && reply->error() != QNetworkReply::UnknownNetworkError) {
            QJsonObject error;
            error.insert("code", reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
            error.insert("message", reply->errorString());
            error.insert("data", QString(reply->readAll()));
            promise->callError(QJSValueList{QString(QJsonDocument(error).toJson())});
        }
    });

    connect(reply, &QNetworkReply::errorOccurred, [=](QNetworkReply::NetworkError error)
            {
                if (error == QNetworkReply::ConnectionRefusedError || error == QNetworkReply::HostNotFoundError || error == QNetworkReply::TimeoutError || error == QNetworkReply::UnknownNetworkError)
                {
                    QJsonObject errorJson;
                    errorJson.insert("code", 0);
                    errorJson.insert("message", reply->errorString());
                    promise->callError(QJSValueList{QString(QJsonDocument(errorJson).toJson())});
                }
            });

    connect(
        reply, &QNetworkReply::uploadProgress,
        [promise](qint64 bytesSent, qint64 bytesTotal)
        {
            promise->callProgress(QJSValueList{QString::number(bytesSent).toInt(), QString::number(bytesTotal).toInt()});
        });

    return promise;
}

QNetworkRequest PocketRequest::getRequest(QUrlQuery query, bool contentType, QString path, bool full)
{
    QNetworkRequest request;
    QUrl url;
    QString _url = PocketBaseSettings::getApiUrl();
    if (full == true)
        url = QUrl(_url + path);
    else
        url = QUrl(_url + "/api" + route + path);
    url.setQuery(query);
    request.setUrl(url);
    if (contentType)
        request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", PocketBaseSettings::getToken().toUtf8());
    request.setRawHeader("Accept", "application/json");

    return request;
}

QUrlQuery PocketRequest::getRequestParams(QJsonObject obj)
{
    if (obj.isEmpty())
        return QUrlQuery();
    QUrlQuery params;

    QStringList keys = obj.keys();
    for (int i = 0; i < keys.size(); i++)
    {
        params.addQueryItem(keys[i], obj.value(keys[i]).toString());
    }

    return params;
}

void PocketRequest::setRoute(const QString &newRoute)
{
    if (route == newRoute)
        return;
    route = newRoute;
}

QString PocketRequest::getRoute() const
{
    return route;
}

QByteArray PocketRequest::methodToString(HttpMethod method)
{
    switch (method)
    {
    case HttpMethod::GET:
        return "GET";
    case HttpMethod::POST:
        return "POST";
    case HttpMethod::PATCH:
        return "PATCH";
    case HttpMethod::DELETE:
        return "DELETE";
    case HttpMethod::PUT:
        return "PUT";
        break;
    }
    return "";
}
