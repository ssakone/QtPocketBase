#ifndef POCKETUTILITY_H
#define POCKETUTILITY_H

#include "qhttpmultipart.h"
#include "qjsonobject.h"
#include "qjsvalue.h"
#include <QObject>

class PocketUtility : public QObject
{
    Q_OBJECT
public:
    explicit PocketUtility(QObject *parent = nullptr);


    static QJsonObject* jsvalueToJsonObject( QJSValue value, QJsonObject *obj = {});

    static void jsonToFormData(QHttpMultiPart *multiPart, QJsonObject &dataObject);
    static void jsonFilesToFormData(QHttpMultiPart *multiPart, QJsonObject &filesObject, bool removing = false);
signals:
};

#endif // POCKETUTILITY_H
