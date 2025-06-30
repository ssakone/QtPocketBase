#include "CollectionSubscriber.h"
#include "PocketBaseCollection.h"
#include "PocketBaseClient.h"
#include "PocketBaseServer.h"
#include <QtQml>

void registerPocketBaseType() {
    // qmlRegisterType<PocketBaseCollection>("PocketBase", 1, 0, "BaseCollection");
    qmlRegisterSingletonType<PocketRequest>("PocketBase", 1, 0, "PocketRequest", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return new PocketRequest();
    });

    qmlRegisterType<CollectionSubscriber>("PocketBase", 1, 0, "Subscriber");

    qmlRegisterSingletonType<PocketBaseClient>("PocketBase", 1, 0, "PocketBaseClient", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return new PocketBaseClient();
    });

    qmlRegisterSingletonType<PocketBaseServer>("PocketBase", 1, 0, "PocketBaseServer", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return new PocketBaseServer();
    });
}

// Q_COREAPP_STARTUP_FUNCTION(registerPocketBaseType);
