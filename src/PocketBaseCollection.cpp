#include "PocketBaseCollection.h"
#include "qjsvalueiterator.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJSValueList>
#include "pocketnetworkmanager.h"

PocketBaseCollection::PocketBaseCollection(QObject *parent)
    : QObject{parent}
{
    manager = PocketNetworkManager::instance();
    request = new PocketRequest(this);
    viewRequest = new PocketRequest(this);
    request->setRoute("/collections/" + this->name() + "/records");
}

PocketBaseCollectionPromise* PocketBaseCollection::getOne(QString id, QJSValue options)
{
    return request->getOne(id, getOptions(options));
}

PocketBaseCollectionPromise *PocketBaseCollection::getList(int page, int perPage, QJSValue options)
{
    return request->getList(page, perPage, getOptions(options));
}

PocketBaseCollectionPromise *PocketBaseCollection::create(QJSValue data)
{
    return request->create(data.toVariant().toJsonObject(), getOptions());
}

PocketBaseCollectionPromise *PocketBaseCollection::create(QString data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    return request->create(doc.object(), getOptions());
}

PocketBaseCollectionPromise *
PocketBaseCollection::createWithFile(QJSValue data, QJSValue files) {
    return request->createWithFile(data, files, getOptions());
}

PocketBaseCollectionPromise *PocketBaseCollection::update(QString id, QJSValue data, QJSValue options)
{
    return request->update(id, data.toVariant().toJsonObject(), getOptions(options));
}

PocketBaseCollectionPromise *PocketBaseCollection::updateWithFile(QString id, QJSValue data, QJSValue files)
{
    return request->updateWithFile(id, data, files, getOptions());
}

PocketBaseCollectionPromise *PocketBaseCollection::deleteFile(QString id, QJSValue files)
{
    return request->deleteFile(id, files, getOptions());
}

PocketBaseCollectionPromise *PocketBaseCollection::deleteOne(QString id)
{
    return request->deleteOne(id);
}

PocketBaseCollectionPromise *PocketBaseCollection::getViewList(QString view, int page, int perPage, QJSValue options)
{
    viewRequest->setRoute("/collections/" + view + "/records");
    return viewRequest->getList(page, perPage, getOptions(options));
}

PocketBaseCollectionPromise *PocketBaseCollection::getViewOne(QString view, QString id, QJSValue options)
{
    viewRequest->setRoute("/collections/" + view + "/records");
    return viewRequest->getOne(id, getOptions(options));
}

QString PocketBaseCollection::getFileUrl(QString fileId, QString fileName)
{
    return PocketBaseSettings::getApiUrl() + "/api/files/" + m_name + "/" + fileId + "/" + fileName;
}

PocketBaseCollectionPromise *PocketBaseCollection::send(QString path, QString method, QJSValue data, QJSValue options)
{
    viewRequest->setRoute("");
    switch (method.toStdString()[0]) {
    case 'G':
        return viewRequest->HttpGet(path);
    case 'P':
        return viewRequest->HttpPost(path, data.toVariant().toJsonObject(), getOptions(options));
    case 'D':
        return viewRequest->HttpDelete(path);
    case 'U':
        return viewRequest->HttpPut(path, data, options);
    default:
        return nullptr;
    }
}

QJsonObject PocketBaseCollection::getOptions(QJSValue options)
{

    QJsonObject allOptions;

    allOptions.insert("expand", QString(m_expands));
    allOptions.insert("fields", QString(m_fields.join(",")));

    QJSValueIterator it(options);
    while (it.hasNext()) {
        it.next();
        allOptions.insert(it.name(), it.value().toString());
    }

    QJSEngine engine;
    QJSValue val =  engine.toScriptValue(allOptions);
    return allOptions;
}

void PocketBaseCollection::prepare()
{
    PocketBaseCollectionSchema *schema = findChild<PocketBaseCollectionSchema*>();
    QJsonObject collectionSchema = {};
    collectionSchema["schema"] = schema->schemaJson();
    collectionSchema["name"] = schema->name();
    collectionSchema["type"] = "base";
    collectionSchema["indexes"] = QJsonArray();
    collectionSchema["listRule"] = schema->listRule();
    collectionSchema["viewRule"] = schema->viewRule();
    collectionSchema["createRule"] = schema->createRule();
    collectionSchema["updateRule"] = schema->updateRule();
    collectionSchema["deleteRule"] = schema->deleteRule();
    collectionSchema["id"] = schema->collectionId();
    collectionSchema["options"] = QJsonObject();
}


QString PocketBaseCollection::expands() const
{
    return m_expands;
}

void PocketBaseCollection::setExpands(const QString &newExpands)
{
    if (m_expands == newExpands)
        return;
    m_expands = newExpands;
    emit expandChangeds();
}

QStringList PocketBaseCollection::fields() const
{
    return m_fields;
}

void PocketBaseCollection::setFields(const QStringList &newFields)
{
    if (m_fields == newFields)
        return;
    m_fields = newFields;
    emit fieldsChanged();
}


QString PocketBaseCollection::name() const
{
    return QString(m_name);
}

void PocketBaseCollection::setName(const QString &newName)
{
    if (m_name == QString(newName))
        return;
    m_name = QString(newName);
    request->setRoute("/collections/" + m_name + "/records");
    emit nameChanged();
}

PocketBaseCollectionSchema *PocketBaseCollection::schema() const
{
    return m_schema;
}

void PocketBaseCollection::setSchema(PocketBaseCollectionSchema *newSchema)
{
    if (m_schema == newSchema)
        return;
    m_schema = newSchema;
    emit schemaChanged();
}

PocketRequest *PocketBaseCollection::getRequest() const
{
    return request;
}

QJsonObject PocketBaseCollection::toJson()
{
    if (m_schema) {
        PocketBaseCollectionSchema *schema = findChild<PocketBaseCollectionSchema*>();
        QJsonObject collectionSchema = {};
        collectionSchema["schema"] = schema->schemaJson();
        collectionSchema["name"] = schema->name();
        collectionSchema["type"] = "base";
        collectionSchema["indexes"] = QJsonArray();
        collectionSchema["listRule"] = schema->listRule();
        collectionSchema["viewRule"] = schema->viewRule();
        collectionSchema["createRule"] = schema->createRule();
        collectionSchema["updateRule"] = schema->updateRule();
        collectionSchema["deleteRule"] = schema->deleteRule();
        collectionSchema["id"] = schema->collectionId();
        collectionSchema["options"] = QJsonObject();
        return collectionSchema;
    }
    return QJsonObject();
}
