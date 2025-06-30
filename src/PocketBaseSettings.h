#ifndef POCKETBASESETTINGS_H
#define POCKETBASESETTINGS_H

#include <QObject>
#include <QSettings>

class PocketBaseSettings : public QSettings
{
public:
    explicit PocketBaseSettings(QObject *parent = nullptr);
    [[nodiscard]] static QString getToken();
    [[nodiscard]] static QString getApiUrl();

    static void setToken(QString token);
    static void setApiUrl(QString apiUrl);

private:
    static PocketBaseSettings s_self;
};

#endif // POCKETBASESETTINGS_H
