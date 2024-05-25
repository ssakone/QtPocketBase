#include "PocketBaseCollectionSchema.h"
#include "qjsonarray.h"

PocketBaseCollectionSchema::PocketBaseCollectionSchema(QObject *parent)
    : QObject{parent}
{}

void PocketBaseCollectionSchema::printField()
{
    // field is in PocketBaseCollectionField, children component
    // use QQmlListProperty<PocketBaseCollectionField> children to get children

    QList<PocketBaseCollectionField*> childrenList = findChildren<PocketBaseCollectionField*>();

    for (auto field : childrenList) {
        qDebug() << field->options()->toJson();
    }
}

QJsonArray PocketBaseCollectionSchema::schemaJson() const
{
    QJsonArray fieldsArray;

    QList<PocketBaseCollectionField*> childrenList = findChildren<PocketBaseCollectionField*>();

    for (auto field : childrenList) {
        fieldsArray.append(field->toJson());
    }

    return fieldsArray;
}

QString PocketBaseCollectionSchema::collectionId() const
{
    return m_collectionId;
}

void PocketBaseCollectionSchema::setCollectionId(const QString &newCollectionId)
{
    if (m_collectionId == newCollectionId)
        return;
    m_collectionId = newCollectionId;
    emit collectionIdChanged();
}

QString PocketBaseCollectionSchema::listRule() const
{
    return m_listRule;
}

void PocketBaseCollectionSchema::setListRule(const QString &newListRule)
{
    if (m_listRule == newListRule)
        return;
    m_listRule = newListRule;
    emit listRuleChanged();
}

QString PocketBaseCollectionSchema::updateRule() const
{
    return m_updateRule;
}

void PocketBaseCollectionSchema::setUpdateRule(const QString &newUpdateRule)
{
    if (m_updateRule == newUpdateRule)
        return;
    m_updateRule = newUpdateRule;
    emit updateRuleChanged();
}

QString PocketBaseCollectionSchema::createRule() const
{
    return m_createRule;
}

void PocketBaseCollectionSchema::setCreateRule(const QString &newCreateRule)
{
    if (m_createRule == newCreateRule)
        return;
    m_createRule = newCreateRule;
    emit createRuleChanged();
}

QString PocketBaseCollectionSchema::deleteRule() const
{
    return m_deleteRule;
}

void PocketBaseCollectionSchema::setDeleteRule(const QString &newDeleteRule)
{
    if (m_deleteRule == newDeleteRule)
        return;
    m_deleteRule = newDeleteRule;
    emit deleteRuleChanged();
}

QString PocketBaseCollectionSchema::viewRule() const
{
    return m_viewRule;
}

void PocketBaseCollectionSchema::setViewRule(const QString &newViewRule)
{
    if (m_viewRule == newViewRule)
        return;
    m_viewRule = newViewRule;
    emit viewRuleChanged();
}

QString PocketBaseCollectionSchema::name() const
{
    return m_name;
}

void PocketBaseCollectionSchema::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}
