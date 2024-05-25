#include "CollectionFieldOptions.h"

CollectionFieldOptions::CollectionFieldOptions(QObject *parent)
    : QObject{parent}
{}

QJsonObject CollectionFieldOptions::toJson() const
{
    return {};
}
