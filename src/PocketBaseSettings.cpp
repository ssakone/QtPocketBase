#include "PocketBaseSettings.h"

PocketBaseSettings::PocketBaseSettings(QObject *parent)
    : QSettings{"PocketBase", "PocketBaseClient", parent}
{}

QString PocketBaseSettings::getToken()
{
    QSettings *settings = new PocketBaseSettings();
    return settings->value("authToken").toString();
}

QString PocketBaseSettings::getApiUrl()
{
    QSettings *settings = new PocketBaseSettings();
    return settings->value("apiUrl").toString();
}

void PocketBaseSettings::setToken(QString token)
{
    QSettings *settings = new PocketBaseSettings();
    settings->setValue("authToken", token);
}

void PocketBaseSettings::setApiUrl(QString apiUrl)
{
    QSettings *settings = new PocketBaseSettings();
    settings->setValue("apiUrl", apiUrl);
}
