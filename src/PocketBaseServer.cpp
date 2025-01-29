#include "PocketBaseServer.h"
#include "qregularexpression.h"
#include <QDir>
#include <QFileSystemWatcher>
#include <QDirIterator>


PocketBaseServer::PocketBaseServer(QObject *parent)
    : QObject{parent}
{
    #if defined(Q_OS_WIN) || defined(Q_OS_MAC) || defined(Q_OS_LINUX)
        process = new QProcess(this);

        connect(process, &QProcess::started, this, [this](){
            setRunning(true);
        });

        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this](int exitCode, QProcess::ExitStatus exitStatus){
            setRunning(false);
            setReady(false);
        });

        connect(process, &QProcess::errorOccurred, this, [](QProcess::ProcessError error){
            qWarning() << "\033[31m" << "Error occurred:" << error << "\033[39m";
        });

        connect(process, &QProcess::readyReadStandardOutput, this, [this](){
            QByteArray data = process->readAllStandardOutput();
            if (data.contains("Server started")) setReady(true);
            qInfo() << "\033[33m" << data.toStdString().c_str() << "\033[39m";
        });

    #endif

}

bool PocketBaseServer::start()
{
#if defined(Q_OS_WIN) || defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    stopProcess();

    QStringList args;
    QString hooksPath = m_hookFolder;

    args << "serve"
         << "--dir" << m_dataFolder
         << "--hooksDir" << hooksPath
         << "--migrationsDir" << m_migrationDir;
    if (m_publicFolder != "") args << "--publicDir" << m_publicFolder;
    if (m_devMode) args << "--dev";
    args << "--http" << m_address + ":" + QString::number(m_port);

    qInfo() << "Starting server with args:" << m_binaryPath << args.join(" ");

    process->start(m_binaryPath, args);
    return  process->waitForStarted();

#elif defined(Q_OS_WASM) || defined(Q_OS_IOS)
    return false;
#else
#error "Unsupported platform"
#endif
}


PocketBaseCollectionPromise * PocketBaseServer::stop()
{
    #if defined(Q_OS_WIN) || defined(Q_OS_MAC) || defined(Q_OS_LINUX)
        stopProcess();
        PocketBaseCollectionPromise * promise = new PocketBaseCollectionPromise(this);
        process->terminate();
        process->waitForFinished();
        promise->callThen({});
        return promise;
    #elif defined(Q_OS_WASM) || defined(Q_OS_IOS)
        PocketBaseCollectionPromise * promise = new PocketBaseCollectionPromise(this);
        // Return promise without starting the process
        return promise;

    #else
    #error "Unsupported platform"
    #endif
}

qint64 PocketBaseServer::isRunning()
{
    #if defined(Q_OS_WIN) || defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    #if defined(Q_OS_WIN)
        QProcess p;
        QString binaryName = "bumas.exe";
        QStringList args;
        args << "/FI" << "IMAGENAME eq " + binaryName;
        p.start("tasklist", args);
        p.waitForFinished();
        QString output = p.readAllStandardOutput();
        if (output.contains(binaryName)) {
            QStringList lines = output.split("\n");
            for (QString line : lines) {
                if (line.contains(binaryName)) {
                    QString pid = line.split(QRegularExpression("\\s+"))[1];
                    return pid.toLongLong();
                }
            }
        }
    #else
        QProcess p;
        p.start("pgrep", QStringList() << m_binaryPath.split("/").last());
        p.waitForFinished();
        QString output = p.readAllStandardOutput();
        if (!output.isEmpty()) {
            return output.toLongLong();
        }
    #endif
    #elif defined(Q_OS_WASM) || defined(Q_OS_IOS)
    return -1;
    #else
    #error "Unsupported platform"
    #endif

    return -1;
}

void PocketBaseServer::stopProcess()
{
    #if defined(Q_OS_WIN) || defined(Q_OS_MAC) || defined(Q_OS_LINUX)
        #if defined(Q_OS_WIN)
            QStringList args;
            args << "/F" << "/IM" << m_binaryPath.split("/").last();

            QProcess p;
            p.start("taskkill", args);
            p.waitForFinished();
        #else
            qint64 pid = isRunning();
            if (pid != -1) {
                QProcess::execute("kill", QStringList() << QString::number(pid));
            }
        #endif
    #endif
}


bool PocketBaseServer::restart()
{
    qDebug() << "Stopping Server";
    stop();
    qDebug() << "Stopping Server";
    return start();
}

QString PocketBaseServer::address() const
{
    return m_address;
}

void PocketBaseServer::setAddress(const QString &newAddress)
{
    if (m_address == newAddress)
        return;
    m_address = newAddress;
    emit addressChanged();
}

int PocketBaseServer::port() const
{
    return m_port;
}

void PocketBaseServer::setPort(int newPort)
{
    if (m_port == newPort)
        return;
    m_port = newPort;
    emit portChanged();
}

bool PocketBaseServer::running() const
{
    return m_running;
}

void PocketBaseServer::setRunning(bool newRunning)
{
    if (m_running == newRunning)
        return;
    m_running = newRunning;
    emit runningChanged();
}

QString PocketBaseServer::hookFolder() const
{
    return m_hookFolder;
}

void PocketBaseServer::setHookFolder(const QString &newHookFolder)
{
    if (m_hookFolder == newHookFolder)
        return;
    m_hookFolder = newHookFolder;
    emit hookFolderChanged();
}

QString PocketBaseServer::publicFolder() const
{
    return m_publicFolder;
}

void PocketBaseServer::setPublicFolder(const QString &newPublicFolder)
{
    if (m_publicFolder == newPublicFolder)
        return;
    m_publicFolder = newPublicFolder;
    emit publicFolderChanged();
}

QString PocketBaseServer::dataFolder() const
{
    return m_dataFolder;
}

void PocketBaseServer::setDataFolder(const QString &newDataFolder)
{
    if (m_dataFolder == newDataFolder)
        return;
    m_dataFolder = newDataFolder;
    emit dataFolderChanged();
}

QString PocketBaseServer::binaryPath() const
{
    return m_binaryPath;
}

void PocketBaseServer::setBinaryPath(const QString &newBinaryPath)
{
    if (m_binaryPath == newBinaryPath)
        return;
    m_binaryPath = newBinaryPath;
    emit binaryPathChanged();
}

bool PocketBaseServer::ready() const
{
    return m_ready;
}

void PocketBaseServer::setReady(bool newReady)
{
    if (m_ready == newReady)
        return;
    m_ready = newReady;
    emit readyChanged();
}


/*
 * PocketBase CLI

Usage:
  pocketbase.exe [command]

Available Commands:
  admin       Manages admin accounts
  migrate     Executes app DB migration scripts
  serve       Starts the web server (default to 127.0.0.1:8090 if no domain is specified)
  update      Automatically updates the current app executable with the latest available version

Flags:
      --automigrate            enable/disable auto migrations (default true)
      --dev                    enable dev mode, aka. printing logs and sql statements to the console
      --dir string             the PocketBase data directory (default "C:\\Users\\enokas\\bin\\pb_data")
      --encryptionEnv string   the env variable whose value of 32 characters will be used
                               as encryption key for the app settings (default none)
  -h, --help                   help for pocketbase.exe
      --hooksDir string        the directory with the JS app hooks
      --hooksPool int          the total prewarm goja.Runtime instances for the JS app hooks execution (default 25)
      --hooksWatch             auto restart the app on pb_hooks file change (default true)
      --indexFallback          fallback the request to index.html on missing static path (eg. when pretty urls are used with SPA) (default true)
      --migrationsDir string   the directory with the user defined migrations
      --publicDir string       the directory to serve static files (default "C:\\Users\\enokas\\bin\\pb_public")
      --queryTimeout int       the default SELECT queries timeout in seconds (default 30)
  -v, --version                version for pocketbase.exe
*/


bool PocketBaseServer::devMode() const
{
    return m_devMode;
}

void PocketBaseServer::setDevMode(bool newDevMode)
{
    if (m_devMode == newDevMode)
        return;
    m_devMode = newDevMode;
    emit devModeChanged();
}

QString PocketBaseServer::migrationDir() const
{
    return m_migrationDir;
}

void PocketBaseServer::setMigrationDir(const QString &newMigrationDir)
{
    if (m_migrationDir == newMigrationDir)
        return;
    m_migrationDir = newMigrationDir;
    emit migrationDirChanged();
}
