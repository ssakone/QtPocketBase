#ifndef POCKETBASECOLLECTION_H
#define POCKETBASECOLLECTION_H

#include "PocketBaseCollectionPromise.h"
#include "PocketBaseCollectionSchema.h"
#include "PocketRequest.h"
#include "PocketBaseSettings.h"
#include "qqmlintegration.h"
#include <QObject>
#include <QVariant>
#include <QJSValue>
#include <QJSValueList>
#include <QStringList>
#include <QtNetwork>

class PocketBaseCollection : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_NAMED_ELEMENT(BaseCollection)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString expands READ expands WRITE setExpands NOTIFY expandChangeds FINAL)
    Q_PROPERTY(QStringList fields READ fields WRITE setFields NOTIFY fieldsChanged FINAL)


public:
    explicit PocketBaseCollection(QObject *parent = nullptr);

    [[nodiscard]] Q_INVOKABLE PocketBaseCollectionPromise *getOne(QString id, QJSValue options = {});
    [[nodiscard]] Q_INVOKABLE PocketBaseCollectionPromise *getList(int page = 1, int perPage = 15, QJSValue options = {});
    [[nodiscard]] Q_INVOKABLE PocketBaseCollectionPromise *create(QJSValue data);
    [[nodiscard]] Q_INVOKABLE PocketBaseCollectionPromise *create(QString data);
    [[nodiscard]] Q_INVOKABLE PocketBaseCollectionPromise *createWithFile(QJSValue data,
                                                            QJSValue files);
    [[nodiscard]] Q_INVOKABLE PocketBaseCollectionPromise *update(QString id, QJSValue data, QJSValue options = {});
    [[nodiscard]] Q_INVOKABLE PocketBaseCollectionPromise *updateWithFile(QString id, QJSValue data,
                                                            QJSValue files);
    [[nodiscard]] Q_INVOKABLE PocketBaseCollectionPromise *deleteFile(QString id, QJSValue files);
    [[nodiscard]] Q_INVOKABLE PocketBaseCollectionPromise *deleteOne(QString id);

    [[nodiscard]] Q_INVOKABLE PocketBaseCollectionPromise *getViewList(QString view, int page = 1, int perPage = 15, QJSValue options = {});
    [[nodiscard]] Q_INVOKABLE PocketBaseCollectionPromise *getViewOne(QString view, QString id, QJSValue options = {});

    Q_INVOKABLE QString getFileUrl(QString fileId, QString fileName);

    [[nodiscard]] Q_INVOKABLE PocketBaseCollectionPromise *send(QString path, QString method, QJSValue data = {}, QJSValue options = {});

    QJsonObject getOptions(QJSValue options = {});
    Q_INVOKABLE void prepare();

    QString expands() const;
    void setExpands(const QString &newExpands);

    QStringList fields() const;
    void setFields(const QStringList &newFields);

    QString name() const;
    void setName(const QString &newName);

    [[nodiscard]] PocketRequest *getRequest() const;

    QJsonObject toJson();

private:
    QString m_name;
    QString clientId;
    QMap<QString, QJSValue> callbacks;
    QNetworkAccessManager *manager;


    QString m_expands = "";

    QStringList m_fields = {};

    PocketRequest *request;
    PocketRequest *viewRequest;



signals:
    void expandChangeds();
    void fieldsChanged();
    void nameChanged();
    void schemaChanged();
};

#endif // POCKETBASECOLLECTION_H
