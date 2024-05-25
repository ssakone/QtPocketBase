#ifndef POCKETBASESERVER_H
#define POCKETBASESERVER_H

#include <QObject>
#include <QProcess>
#include "PocketBaseCollectionPromise.h"

class PocketBaseServer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString dataFolder READ dataFolder WRITE setDataFolder NOTIFY dataFolderChanged FINAL)
    Q_PROPERTY(QString publicFolder READ publicFolder WRITE setPublicFolder NOTIFY publicFolderChanged FINAL)
    Q_PROPERTY(QString hookFolder READ hookFolder WRITE setHookFolder NOTIFY hookFolderChanged FINAL)
    Q_PROPERTY(QString binaryPath READ binaryPath WRITE setBinaryPath NOTIFY binaryPathChanged FINAL)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged FINAL)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged FINAL)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged FINAL)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged FINAL)
    Q_PROPERTY(bool devMode READ devMode WRITE setDevMode NOTIFY devModeChanged FINAL)
public:
    explicit PocketBaseServer(QObject *parent = nullptr);

    Q_INVOKABLE PocketBaseCollectionPromise * start();
    Q_INVOKABLE PocketBaseCollectionPromise * stop();
    Q_INVOKABLE PocketBaseCollectionPromise * restart();

    qint64 isRunning();
    void stopProcess();

    QString address() const;
    void setAddress(const QString &newAddress);

    int port() const;
    void setPort(int newPort);

    bool running() const;
    void setRunning(bool newRunning);

    QString hookFolder() const;
    void setHookFolder(const QString &newHookFolder);

    QString publicFolder() const;
    void setPublicFolder(const QString &newPublicFolder);

    QString dataFolder() const;
    void setDataFolder(const QString &newDataFolder);

    QString binaryPath() const;
    void setBinaryPath(const QString &newBinaryPath);

    bool ready() const;
    void setReady(bool newReady);

    bool devMode() const;
    void setDevMode(bool newDevMode);

signals:
    void addressChanged();
    void portChanged();

    void runningChanged();

    void hookFolderChanged();

    void publicFolderChanged();

    void dataFolderChanged();

    void binaryPathChanged();

    void readyChanged();

    void devModeChanged();

private:
    QString m_address = "localhost";
    QProcess *process;
    int m_port = 8090;
    bool m_running = false;
    QString m_hookFolder = "";
    QString m_publicFolder = "";
    QString m_dataFolder = "";
    QString m_binaryPath = "C:/Users/enokas/bin/bumas.exe";
    bool m_ready = false;
    bool m_devMode = false;
};

#endif // POCKETBASESERVER_H
