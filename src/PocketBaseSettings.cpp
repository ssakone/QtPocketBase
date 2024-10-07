#include "PocketBaseSettings.h"

PocketBaseSettings PocketBaseSettings::s_self;

PocketBaseSettings::PocketBaseSettings(QObject *parent)
    : QSettings{"PocketBase", "PocketBaseClient", parent}
{}

QString PocketBaseSettings::getToken()
{
    return s_self.value("authToken").toString();
}

QString PocketBaseSettings::getApiUrl()
{
    return s_self.value("apiUrl").toString();
}

void PocketBaseSettings::setToken(QString token)
{
    s_self.setValue("authToken", token);
}

void PocketBaseSettings::setApiUrl(QString apiUrl)
{
    s_self.setValue("apiUrl", apiUrl);
}
