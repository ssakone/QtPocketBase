#include "PocketBaseCollectionField.h"

PocketBaseCollectionField::PocketBaseCollectionField(QObject *parent)
    : QObject{parent}, m_options{nullptr}
{}

QString PocketBaseCollectionField::name() const
{
    return m_name;
}

void PocketBaseCollectionField::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}


bool PocketBaseCollectionField::unique() const
{
    return m_unique;
}

void PocketBaseCollectionField::setUnique(bool newUnique)
{
    if (m_unique == newUnique)
        return;
    m_unique = newUnique;
    emit uniqueChanged();
}

bool PocketBaseCollectionField::required() const
{
    return m_required;
}

void PocketBaseCollectionField::setRequired(bool newRequired)
{
    if (m_required == newRequired)
        return;
    m_required = newRequired;
    emit requiredChanged();
}

int PocketBaseCollectionField::type() const
{
    return m_type;
}

void PocketBaseCollectionField::setType(int newType)
{
    if (m_type == newType)
        return;
    m_type = newType;
    emit typeChanged();
}

CollectionFieldOptions *PocketBaseCollectionField::options() const
{
    return m_options;
}

void PocketBaseCollectionField::setOptions(CollectionFieldOptions* newOptions)
{
    if (m_options == newOptions)
        return;
    m_options = newOptions;
    emit optionsChanged();
}

QString PocketBaseCollectionField::typeToString(int type) const
{
    switch (type) {
    case String:
        return "text";
    case Number:
        return "number";
    case Boolean:
        return "bool";
    case Date:
        return "date";
    case Select:
        return "select";
    case Json:
        return "json";
    case File:
        return "file";
    case Relation:
        return "relation";
    case Url:
        return "url";
    case Email:
        return "email";
    case Editor:
        return "editor";
    default:
        return "other";
    }
}

QJsonObject PocketBaseCollectionField::toJson() const
{
    QJsonObject fieldObject;
    fieldObject["name"] = name();
    fieldObject["type"] = typeToString(type());
    fieldObject["required"] = required();
    fieldObject["unique"] = unique();
    if (options() != nullptr)
        fieldObject["options"] = options()->toJson();
    fieldObject["id"] = fieldId();
    return fieldObject;
}

QString PocketBaseCollectionField::fieldId() const
{
    return m_fieldId;
}

void PocketBaseCollectionField::setFieldId(const QString &newFieldId)
{
    if (m_fieldId == newFieldId)
        return;
    m_fieldId = newFieldId;
    emit fieldIdChanged();
}
