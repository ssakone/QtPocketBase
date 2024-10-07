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
    const QJsonObject& obj = PocketUtility::jsvalueToJsonObject(data);
    return makeRequest(getRequest(getRequestParams(options), true), HttpMethod::POST, QJsonDocument(obj).toJson());
}

PocketBaseCollectionPromise *PocketRequest::create(QJsonObject data, QJsonObject options)
{
    return makeRequest(getRequest(getRequestParams(options), true), HttpMethod::POST, QJsonDocument(data).toJson());
}

PocketBaseCollectionPromise *PocketRequest::createWithFile(QJSValue data, QJSValue files, QJsonObject options)
{
    QJsonObject obj = PocketUtility::jsvalueToJsonObject(data);
    QJsonObject fobj = files.toVariant().toJsonObject();

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    PocketUtility::jsonToFormData(multiPart, obj);
    PocketUtility::jsonFilesToFormData(multiPart, fobj);
    QNetworkRequest request = getRequest(getRequestParams(options), false);
    return makeRequest(&request, HttpMethod::POST, multiPart);
}

PocketBaseCollectionPromise *PocketRequest::update(QString id, QJSValue data, QJsonObject options)
{
    const QJsonObject& obj = PocketUtility::jsvalueToJsonObject(data);
    return makeRequest(getRequest(getRequestParams(options), true, "/" + id), HttpMethod::PATCH, QJsonDocument(obj).toJson());
}

PocketBaseCollectionPromise *PocketRequest::update(QString id, QJsonObject data, QJsonObject options)
{
    return makeRequest(getRequest(getRequestParams(options), true, "/" + id), HttpMethod::PATCH, QJsonDocument(data).toJson());
}

PocketBaseCollectionPromise *PocketRequest::updateWithFile(QString id, QJSValue data, QJSValue files, QJsonObject options)
{
    QJsonObject obj = PocketUtility::jsvalueToJsonObject(data);
    QJsonObject fobj = files.toVariant().toJsonObject();

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    PocketUtility::jsonToFormData(multiPart, obj);
    PocketUtility::jsonFilesToFormData(multiPart, fobj);

    multiPart->dumpObjectTree();

    QNetworkRequest request = getRequest(getRequestParams(options), false, "/" + id);
    return makeRequest(&request, HttpMethod::PATCH, multiPart);
}

PocketBaseCollectionPromise *PocketRequest::deleteFile(QString id, QJSValue files, QJsonObject options)
{
    QJsonObject fobj = files.toVariant().toJsonObject();
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    PocketUtility::jsonFilesToFormData(multiPart, fobj);

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

PocketBaseCollectionPromise *PocketRequest::HttpGet(QString path, QJSValue options, bool other)
{
    const QJsonObject& obj = PocketUtility::jsvalueToJsonObject(options);
    return makeRequest(getRequest(getRequestParams(obj), false, path, true, other), HttpMethod::GET);
}

PocketBaseCollectionPromise *PocketRequest::HttpPostFile(QString path, QJSValue data, QJSValue files, QJSValue options, bool other)
{
    const QJsonObject& op = PocketUtility::jsvalueToJsonObject(options);
    QJsonObject obj = PocketUtility::jsvalueToJsonObject(data);
    QJsonObject fobj = files.toVariant().toJsonObject();

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    PocketUtility::jsonToFormData(multiPart, obj);
    PocketUtility::jsonFilesToFormData(multiPart, fobj);
    QNetworkRequest request = getRequest(getRequestParams(op), true, path, true, other);
    return makeRequest(&request, HttpMethod::POST, multiPart);

}

PocketBaseCollectionPromise *PocketRequest::HttpPost(QString path, QJSValue data, QJSValue options, bool other)
{
    const QJsonObject& obj = PocketUtility::jsvalueToJsonObject(options);
    const QJsonObject& datas = PocketUtility::jsvalueToJsonObject(data);
    return makeRequest(getRequest(getRequestParams(obj), true, path, true, other), HttpMethod::POST, QJsonDocument(datas).toJson());
}

PocketBaseCollectionPromise *PocketRequest::HttpPost(QString path, QJsonObject datas, QJsonObject obj)
{
    return makeRequest(getRequest(getRequestParams(obj), true, path, true), HttpMethod::POST, QJsonDocument(datas).toJson());
}

PocketBaseCollectionPromise *PocketRequest::HttpPut(QString path, QJSValue data, QJSValue options)
{
    const QJsonObject& obj = PocketUtility::jsvalueToJsonObject(options);
    QJsonObject datas = data.toVariant().toJsonObject();
    return makeRequest(getRequest(QUrlQuery(), true, path, true), HttpMethod::PUT, QJsonDocument(datas).toJson());
}

PocketBaseCollectionPromise *PocketRequest::HttpPut(QString path, QJsonObject datas)
{
    return makeRequest(getRequest(QUrlQuery(), true, path, true), HttpMethod::PUT, QJsonDocument(datas).toJson());
}

PocketBaseCollectionPromise *PocketRequest::HttpPatch(QString path, QJSValue data, QJSValue options)
{
    const QJsonObject& obj = PocketUtility::jsvalueToJsonObject(options);
    const QJsonObject& datas = PocketUtility::jsvalueToJsonObject(data);
    return makeRequest(getRequest(getRequestParams(obj), true, path, true), HttpMethod::PATCH, QJsonDocument(datas).toJson());
}

PocketBaseCollectionPromise *PocketRequest::HttpDelete(QString path, QJSValue options)
{
    const QJsonObject& obj = PocketUtility::jsvalueToJsonObject(options);
    return makeRequest(getRequest(getRequestParams(obj), false, path, true, true), HttpMethod::DELETE);
}

PocketBaseCollectionPromise *PocketRequest::makeRequest(QNetworkRequest request, HttpMethod method)
{
    PocketBaseCollectionPromise *promise = new PocketBaseCollectionPromise(this);

    QNetworkReply *reply;

    reply = manager->sendCustomRequest(request, methodToString(method));

    connect(reply, &QNetworkReply::finished, promise, [=]()
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

    connect(reply, &QNetworkReply::errorOccurred, promise, [=](QNetworkReply::NetworkError error)
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
    connect(reply, &QNetworkReply::finished, promise, [reply, promise]()
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

    connect(reply, &QNetworkReply::errorOccurred, promise, [=](QNetworkReply::NetworkError error)
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

    connect(reply, &QNetworkReply::finished, promise, [=]()
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

    connect(reply, &QNetworkReply::errorOccurred, promise, [=](QNetworkReply::NetworkError error)
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
        promise, [promise](qint64 bytesSent, qint64 bytesTotal)
        {
            promise->callProgress(QJSValueList{QString::number(bytesSent).toInt(), QString::number(bytesTotal).toInt()});
        });

    return promise;
}

PocketBaseCollectionPromise *PocketRequest::get(QString url, QJSValue headers)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    const QJsonObject& header = PocketUtility::jsvalueToJsonObject(headers);
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", header.value("Authorization").toString().toUtf8());

    return makeRequest(request, HttpMethod::GET);
}

PocketBaseCollectionPromise *PocketRequest::post(QString url, QJSValue data, QJSValue headers)
{
    const QJsonObject& obj = PocketUtility::jsvalueToJsonObject(data);
    QNetworkRequest request= getRequest(QUrlQuery(), true, url, true, true);
    const QJsonObject& header = PocketUtility::jsvalueToJsonObject(headers);
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", header.value("Authorization").toString().toUtf8());
    return makeRequest(request, HttpMethod::POST, QJsonDocument(obj).toJson());
}

PocketBaseCollectionPromise *PocketRequest::put(QString url, QJSValue data, QJSValue headers)
{
    const QJsonObject& obj = PocketUtility::jsvalueToJsonObject(data);
    return makeRequest(getRequest(QUrlQuery(), true, url, true, true), HttpMethod::PUT, QJsonDocument(obj).toJson());
}

PocketBaseCollectionPromise *PocketRequest::postFile(QString url, QJSValue data, QJSValue files, QJSValue headers)
{
    QJsonObject obj = PocketUtility::jsvalueToJsonObject(data);
    QJsonObject fobj = files.toVariant().toJsonObject();
    const QJsonObject& header = PocketUtility::jsvalueToJsonObject(headers);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    PocketUtility::jsonToFormData(multiPart, obj);
    PocketUtility::jsonFilesToFormData(multiPart, fobj);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Authorization", header.value("Authorization").toString().toUtf8());
    return makeRequest(&request, HttpMethod::POST, multiPart);
}

PocketBaseCollectionPromise *PocketRequest::putFile(QString url, QJSValue data, QJSValue files, QJSValue headers)
{
    QJsonObject obj = PocketUtility::jsvalueToJsonObject(data);
    QJsonObject fobj = files.toVariant().toJsonObject();
    const QJsonObject& header = PocketUtility::jsvalueToJsonObject(headers);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    PocketUtility::jsonToFormData(multiPart, obj);
    PocketUtility::jsonFilesToFormData(multiPart, fobj);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Authorization", header.value("Authorization").toString().toUtf8());
    return makeRequest(&request, HttpMethod::PATCH, multiPart);
}

PocketBaseCollectionPromise *PocketRequest::removeFile(QString url, QJSValue files, QJSValue headers)
{
    QJsonObject fobj = files.toVariant().toJsonObject();
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    PocketUtility::jsonFilesToFormData(multiPart, fobj, true);
    const QJsonObject& header = PocketUtility::jsvalueToJsonObject(headers);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("Authorization", header.value("Authorization").toString().toUtf8());
    request.setRawHeader("Accept", "application/json");

    return makeRequest(&request, HttpMethod::PATCH, multiPart);
}

PocketBaseCollectionPromise *PocketRequest::patch(QString url, QJSValue data, QJSValue headers)
{
    const QJsonObject& obj = PocketUtility::jsvalueToJsonObject(data);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    const QJsonObject& header = PocketUtility::jsvalueToJsonObject(headers);
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", header.value("Authorization").toString().toUtf8());
    return makeRequest(request, HttpMethod::PATCH, QJsonDocument(obj).toJson());
}

PocketBaseCollectionPromise *PocketRequest::del(QString url, QJSValue headers)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("Accept", "application/json");
    return makeRequest(request, HttpMethod::DELETE);
}

QNetworkRequest PocketRequest::getRequest(QUrlQuery query, bool contentType, QString path, bool full, bool other)
{
    QNetworkRequest request;
    QUrl url;
    QString _url;
    if (other)
    {
         _url = path;
        url = QUrl(_url);
    }

    else {
        _url = PocketBaseSettings::getApiUrl();
        if (full == true)
            url = QUrl(_url + path);
        else
            url = QUrl(_url + "/api" + route + path);
    }

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

PocketBaseCollectionPromise *PocketRequest::downloadFile(const QString &url, const QString &saveDir, QJSValue headers)
{
    PocketBaseCollectionPromise *promise = new PocketBaseCollectionPromise(this);

    const QJsonObject& header = PocketUtility::jsvalueToJsonObject(headers);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("Authorization", header.value("Authorization").toString().toUtf8());

    QNetworkReply *reply = manager->get(request);

    // Générer un nom de fichier basé sur l'URL
    QString fileName = QFileInfo(QUrl(url).path()).fileName();
    if (fileName.isEmpty()) {
        fileName = "downloaded_file";
    }

    // Assurer un nom de fichier unique
    QString filePath = QDir(saveDir).filePath(fileName);
    int counter = 1;
    while (QFile::exists(filePath)) {
        QString newName = QString("%1_%2").arg(QFileInfo(fileName).baseName()).arg(counter);
        if (QFileInfo(fileName).completeSuffix().isEmpty()) {
            filePath = QDir(saveDir).filePath(newName);
        } else {
            filePath = QDir(saveDir).filePath(newName + "." + QFileInfo(fileName).completeSuffix());
        }
        counter++;
    }

    QFile *file = new QFile(filePath);
    if (!file->open(QIODevice::WriteOnly)) {
        delete file;
        QJsonObject error;
        error.insert("code", 0);
        error.insert("message", "Impossible d'ouvrir le fichier pour l'écriture");
        promise->callError(QJSValueList{QString(QJsonDocument(error).toJson())});
        return promise;
    }

    connect(reply, &QNetworkReply::readyRead, [=]() {
        if (file->isOpen()) {
            file->write(reply->readAll());
        }
    });

    connect(reply, &QNetworkReply::finished, promise, [=]() {
        if (file->isOpen()) {
            file->close();
        }

        if (reply->error() == QNetworkReply::NoError) {
            promise->callThen(QJSValueList{filePath});
        } else {
            QJsonObject error;
            error.insert("code", reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
            error.insert("message", reply->errorString());
            promise->callError(QJSValueList{QString(QJsonDocument(error).toJson())});
        }

        file->deleteLater();
        reply->deleteLater();
    });

    connect(reply, &QNetworkReply::downloadProgress, promise, [promise](qint64 bytesReceived, qint64 bytesTotal) {
        promise->callProgress(QJSValueList{QString::number(bytesReceived).toInt(), QString::number(bytesTotal).toInt()});
    });

    return promise;
}

