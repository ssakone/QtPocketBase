#include "PocketBaseCollectionPromise.h"
#include <QtDebug>

PocketBaseCollectionPromise::PocketBaseCollectionPromise(QObject *parent)
    : QObject{parent}
{}

PocketBaseCollectionPromise *PocketBaseCollectionPromise::then(QJSValue callback)
{
    m_then = callback;
    return this;
}

PocketBaseCollectionPromise * PocketBaseCollectionPromise::error(QJSValue callback)
{
    m_error = callback;
    return this;
}

PocketBaseCollectionPromise *PocketBaseCollectionPromise::progress(QJSValue callback)
{
    m_progress = callback;
    return this;
}

PocketBaseCollectionPromise *PocketBaseCollectionPromise::finally(QJSValue callback)
{
    m_finally = callback;
    return this;
}

QJSValue PocketBaseCollectionPromise::getThen() const
{
    return m_then;
}

QJSValue PocketBaseCollectionPromise::getError() const
{
    return m_error;
}

QJSValue PocketBaseCollectionPromise::getProgress() const
{
    return m_progress;
}

QJSValue PocketBaseCollectionPromise::getFinally() const
{
    return m_finally;
}

void PocketBaseCollectionPromise::callThen(QJSValueList args)
{
    emit onThen(args);
    if (m_then.isCallable()) {
        if (args.length() > 0)
        {
            try {
                m_then.call(args);
            } catch (const std::exception &e) {
                qWarning() << "Error in then callback" << e.what();
            }

        }
        else {
            m_then.call();
        }

    }
    callFinally(args);
}

void PocketBaseCollectionPromise::callError(QJSValueList args)
{
    if (m_error.isCallable()) {
        m_error.call(args);

    }
    emit onError(args);
    callFinally(args);
}

void PocketBaseCollectionPromise::callProgress(QJSValueList args)
{
    if (m_progress.isCallable()) {
        m_progress.call(args);
    }
    emit onProgress(args);
}

void PocketBaseCollectionPromise::callFinally(QJSValueList args)
{
    if (m_finally.isCallable()) {
        m_finally.call(args);
    }
    emit onFinally(args);
}
