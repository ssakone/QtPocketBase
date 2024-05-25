#ifndef POCKETBASECOLLECTIONSCHEMA_H
#define POCKETBASECOLLECTIONSCHEMA_H

#include "PocketBaseCollectionField.h"
#include <QObject>
#include <QQmlEngine>

class PocketBaseCollectionSchema : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<PocketBaseCollectionField> children READ children)
    Q_PROPERTY(QString collectionId READ collectionId WRITE setCollectionId NOTIFY collectionIdChanged FINAL)
    Q_PROPERTY(QString listRule READ listRule WRITE setListRule NOTIFY listRuleChanged FINAL)
    Q_PROPERTY(QString updateRule READ updateRule WRITE setUpdateRule NOTIFY updateRuleChanged FINAL)
    Q_PROPERTY(QString createRule READ createRule WRITE setCreateRule NOTIFY createRuleChanged FINAL)
    Q_PROPERTY(QString deleteRule READ deleteRule WRITE setDeleteRule NOTIFY deleteRuleChanged FINAL)
    Q_PROPERTY(QString viewRule READ viewRule WRITE setViewRule NOTIFY viewRuleChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)

    Q_CLASSINFO("DefaultProperty", "children")
public:
    explicit PocketBaseCollectionSchema(QObject *parent = nullptr);


    Q_INVOKABLE void printField();

    QQmlListProperty<PocketBaseCollectionField> children() {
        return QQmlListProperty<PocketBaseCollectionField>(this, nullptr,
                                            &PocketBaseCollectionSchema::append_child, nullptr, nullptr, nullptr);
    }

    static void append_child(QQmlListProperty<PocketBaseCollectionField>* list, PocketBaseCollectionField* item) {
        if (item) {
            PocketBaseCollectionSchema* parent = qobject_cast<PocketBaseCollectionSchema*>(list->object);
            if (parent) {
                item->setParent(parent);
            }
        }
    }

    QJsonArray schemaJson() const;

    QString collectionId() const;
    void setCollectionId(const QString &newCollectionId);

    QString listRule() const;
    void setListRule(const QString &newListRule);

    QString updateRule() const;
    void setUpdateRule(const QString &newUpdateRule);

    QString createRule() const;
    void setCreateRule(const QString &newCreateRule);

    QString deleteRule() const;
    void setDeleteRule(const QString &newDeleteRule);

    QString viewRule() const;
    void setViewRule(const QString &newViewRule);

    QString name() const;
    void setName(const QString &newName);

signals:
    void collectionIdChanged();
    void listRuleChanged();

    void updateRuleChanged();

    void createRuleChanged();

    void deleteRuleChanged();

    void viewRuleChanged();

    void nameChanged();

private:
    QString m_collectionId;
    QString m_listRule;
    QString m_updateRule;
    QString m_createRule;
    QString m_deleteRule;
    QString m_viewRule;
    QString m_name;
};

#endif // POCKETBASECOLLECTIONSCHEMA_H
