#ifndef CIRCULARLOGGER_H
#define CIRCULARLOGGER_H

#include <QObject>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QJsonDocument>

class CircularLogger : public QObject {
    Q_OBJECT
public:
    enum LogType { Daily, Hourly, Minutely, Secondly };

    explicit CircularLogger(QObject *parent = nullptr);
    ~CircularLogger();

    bool loadConfig(const QString &configPath);
    void log(const QString &message);

private:
    LogType m_logType;
    int m_maxFiles;
    int m_frequency;
    QString m_logDir;
    QFile m_currentFile;
    QDateTime m_lastLogTime;

    QString getLogFileName(const QDateTime &time) const;
    void openNewLogFile(const QDateTime &time);
    void manageLogFiles();
    bool shouldCreateNewFile(const QDateTime &currentTime) const;
};

#endif // CIRCULARLOGGER_H
