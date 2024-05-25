#ifndef COLLECTIONTEXTFIELDOPTIONS_H
#define COLLECTIONTEXTFIELDOPTIONS_H

#include <QObject>
#include "CollectionFieldOptions.h"
#include <QJsonObject>


class CollectionTextFieldOptions : public CollectionFieldOptions
{
    Q_OBJECT
    Q_PROPERTY(int min READ min WRITE setMin NOTIFY minChanged FINAL)
    Q_PROPERTY(int max READ max WRITE setMax NOTIFY maxChanged FINAL)
    Q_PROPERTY(QString pattern READ pattern WRITE setPattern NOTIFY patternChanged FINAL)

public:
    CollectionTextFieldOptions();
    int min() const;
    void setMin(int newMin);
    int max() const;
    void setMax(int newMax);

    QString pattern() const;
    void setPattern(const QString &newPattern);

    QJsonObject toJson() const override;

signals:
    void minChanged();
    void maxChanged();

    void patternChanged();

private:
    int m_min;
    int m_max;
    QString m_pattern;
};

#endif // COLLECTIONTEXTFIELDOPTIONS_H
