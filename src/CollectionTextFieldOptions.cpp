#include "CollectionTextFieldOptions.h"

CollectionTextFieldOptions::CollectionTextFieldOptions() {}

int CollectionTextFieldOptions::min() const
{
    return m_min;
}

void CollectionTextFieldOptions::setMin(int newMin)
{
    if (m_min == newMin)
        return;
    m_min = newMin;
    emit minChanged();
}

int CollectionTextFieldOptions::max() const
{
    return m_max;
}

void CollectionTextFieldOptions::setMax(int newMax)
{
    if (m_max == newMax)
        return;
    m_max = newMax;
    emit maxChanged();
}

QString CollectionTextFieldOptions::pattern() const
{
    return m_pattern;
}

void CollectionTextFieldOptions::setPattern(const QString &newPattern)
{
    if (m_pattern == newPattern)
        return;
    m_pattern = newPattern;
    emit patternChanged();
}

QJsonObject CollectionTextFieldOptions::toJson() const
{
    QJsonObject json;
    json["min"] = m_min;
    json["max"] = m_max;
    json["pattern"] = m_pattern;
    return json;
}
