#include "PocketUtility.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>
#include <QMimeDatabase>
#include <QMimeType>
#include <memory>
#include <QJsonDocument>
#include <QStringList>


PocketUtility::PocketUtility(QObject *parent)
    : QObject{parent}
{}

QJsonObject PocketUtility::jsvalueToJsonObject(QJSValue value)
{
    // qDebug() << value.toVariant().toJsonObject();
    QJsonObject obj;
    if (value.isUndefined() || value.isNull())
        return obj;
    QJsonObject optionsObj = value.toVariant().toJsonObject();
    QStringList keys = optionsObj.keys();
    for (int i = 0; i < keys.size(); i++) {
        obj.insert(keys[i], optionsObj.value(keys[i]).toString());
    }
    return obj;
}


void PocketUtility::jsonToFormData(QHttpMultiPart *multiPart, QJsonObject &dataObject)
{
    for (const auto &key : dataObject.keys()) {
        QHttpPart jsonPart;
        jsonPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QVariant("form-data; name=\"" + key + "\""));

        QJsonValue value = dataObject.value(key);
        if (value.isObject()) {
            jsonPart.setBody(QJsonDocument(value.toObject()).toJson());
        } else if (value.isArray()) {
            jsonPart.setBody(QJsonDocument(value.toArray()).toJson());
        } else {
            jsonPart.setBody(value.toString().toUtf8());
        }

        multiPart->append(jsonPart);
    }
}

void PocketUtility::jsonFilesToFormData(QHttpMultiPart *multiPart, QJsonObject &filesObject, bool removing)
{
    const QStringList keys = filesObject.keys();
    for (const auto &key : keys) {
        QHttpPart filePart;

        const QJsonArray filePaths = filesObject[key].toArray();

        for (const QJsonValue &jsonfilePath : filePaths) {
            const QString filePath = jsonfilePath.toString();
            auto file = std::make_unique<QFile>(filePath);



            QMimeDatabase mimeDb;
            QMimeType mimeType = mimeDb.mimeTypeForFile(filePath);
            QString contentDispositionValue;

            const bool ok = file->open(QIODevice::ReadOnly);
            if (removing) {
                contentDispositionValue = "form-data; name=\"photo." + filePath + "\"";
            } else {
                contentDispositionValue = "form-data; name=\"" + key + "\"; filename=\"" + file->fileName() + "\"";
            }

            if (ok)
                qDebug() << "File opened" << mimeType.name() << contentDispositionValue;
            else
                qWarning() << file->errorString();


            QVariant contentDisposition = QVariant(contentDispositionValue);

            if (!removing)
                filePart.setHeader(QNetworkRequest::ContentTypeHeader,
                                   QVariant(mimeType.name()));
            filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                               contentDisposition);
            if (ok)
            {
                auto *f = file.release();
                filePart.setBodyDevice(f);
                f->setParent(multiPart);
            }

            multiPart->append(filePart);
        }
    }
}
