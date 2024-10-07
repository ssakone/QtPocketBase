#include "CollectionSubscriber.h"
#include "PocketBaseCollection.h"
#include "PocketBaseCollectionField.h"
#include "PocketBaseCollectionSchema.h"
#include "CollectionFileFieldOptions.h"
#include "CollectionFieldOptions.h"
#include "CollectionTextFieldOptions.h"
#include "PocketBaseClient.h"
#include "PocketBaseServer.h"
#include <QtQml>

void registerPocketBaseType() {
    qmlRegisterType<PocketBaseCollection>("PocketBase", 1, 0, "BaseCollection");
    qmlRegisterType<PocketBaseCollectionField>("PocketBase", 1, 0, "CollectionField");
    qmlRegisterType<PocketBaseCollectionSchema>("PocketBase", 1, 0, "CollectionSchema");
    qmlRegisterType<CollectionFileFieldOptions>("PocketBase", 1, 0, "FileFieldOptions");
    qmlRegisterType<CollectionTextFieldOptions>("PocketBase", 1, 0, "TextFieldOptions");
    qmlRegisterSingletonType<PocketRequest>("PocketBase", 1, 0, "PocketRequest", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return new PocketRequest();
    });

    qmlRegisterType<CollectionSubscriber>("PocketBase", 1, 0, "Subscriber");
    qmlRegisterUncreatableType<CollectionFieldOptions>("PocketBase", 1, 0, "FieldOptions", "FieldOptions is an abstract class");

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
