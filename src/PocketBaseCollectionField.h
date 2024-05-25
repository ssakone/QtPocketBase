#ifndef POCKETBASECOLLECTIONFIELD_H
#define POCKETBASECOLLECTIONFIELD_H

#include <QObject>
#include "CollectionFieldOptions.h"
#include <QQmlEngine>

class PocketBaseCollectionField : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged FINAL)
    Q_PROPERTY(bool required READ required WRITE setRequired NOTIFY requiredChanged FINAL)
    Q_PROPERTY(bool unique READ unique WRITE setUnique NOTIFY uniqueChanged FINAL)
    Q_PROPERTY(QString fieldId READ fieldId WRITE setFieldId NOTIFY fieldIdChanged FINAL)
    Q_PROPERTY(CollectionFieldOptions* options READ options WRITE setOptions NOTIFY optionsChanged FINAL)

public:

    enum FieldType {
        String,
        Number,
        Boolean,
        Date,
        Select,
        Json,
        File,
        Relation,
        Url,
        Email,
        Editor
    };
    Q_ENUM(FieldType);

    explicit PocketBaseCollectionField(QObject *parent = nullptr);

    QString name() const;
    void setName(const QString &newName);

    bool unique() const;
    void setUnique(bool newUnique);

    bool required() const;
    void setRequired(bool newRequired);

    int type() const;
    void setType(int newType);

    CollectionFieldOptions *options() const;
    void setOptions(CollectionFieldOptions* newOptions);

    QString typeToString(int type) const;

    virtual QJsonObject toJson() const;

    QString fieldId() const;
    void setFieldId(const QString &newFieldId);

signals:
    void nameChanged();
    void optionsChanged();

    void uniqueChanged();

    void requiredChanged();

    void typeChanged();

    void fieldIdChanged();

private:
    CollectionFieldOptions* m_options;
    QString m_name;
    bool m_unique = false;
    bool m_required = false;
    int m_type = FieldType::String;
    QString m_fieldId;
};

#endif // POCKETBASECOLLECTIONFIELD_H
