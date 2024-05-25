#ifndef POCKETBASECOLLECTIONPROMISE_H
#define POCKETBASECOLLECTIONPROMISE_H

#include <QObject>
#include <QJSValue>
#include <QJSValueList>

class PocketBaseCollectionPromise : public QObject
{
    Q_OBJECT
public:
    explicit PocketBaseCollectionPromise(QObject *parent = nullptr);

    Q_INVOKABLE PocketBaseCollectionPromise * then(QJSValue callback);
    Q_INVOKABLE PocketBaseCollectionPromise * error(QJSValue callback);
    Q_INVOKABLE PocketBaseCollectionPromise * progress(QJSValue callback);
    Q_INVOKABLE PocketBaseCollectionPromise * finally(QJSValue callback);


    QJSValue getThen() const;
    QJSValue getError() const;
    QJSValue getProgress() const;
    QJSValue getFinally() const;

    void callThen(QJSValueList args);
    void callError(QJSValueList args);
    void callProgress(QJSValueList args);
    void callFinally(QJSValueList args);

private:
    QJSValue m_then;
    QJSValue m_error;
    QJSValue m_progress;
    QJSValue m_finally;
signals:
    void onThen(QJSValueList args);
    void onError(QJSValueList args);
    void onProgress(QJSValueList args);
    void onFinally(QJSValueList args);

};

#endif // POCKETBASECOLLECTIONPROMISE_H
