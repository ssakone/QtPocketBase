#ifndef POCKETBASESETTINGS_H
#define POCKETBASESETTINGS_H

#include <QObject>
#include <QSettings>

class PocketBaseSettings : public QSettings
{
public:
    explicit PocketBaseSettings(QObject *parent = nullptr);
    static QString getToken();
    static QString getApiUrl();

    static void setToken(QString token);
    static void setApiUrl(QString apiUrl);
};

#endif // POCKETBASESETTINGS_H
