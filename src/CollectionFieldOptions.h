#ifndef POCKETBASECOLLECTIONFIELDOPTIONS_H
#define POCKETBASECOLLECTIONFIELDOPTIONS_H

#include <QObject>
#include <QJsonObject>

class CollectionFieldOptions : public QObject
{
    Q_OBJECT
public:
    explicit CollectionFieldOptions(QObject *parent = nullptr);

    virtual QJsonObject toJson() const;

signals:
};

#endif // POCKETBASECOLLECTIONFIELDOPTIONS_H
