#include "CircularLogger.h"
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>

CircularLogger::CircularLogger(QObject *parent) : QObject(parent),
    m_logType(Daily), m_maxFiles(7), m_frequency(1), m_logDir("logs") {
}

CircularLogger::~CircularLogger() {
    if (m_currentFile.isOpen()) {
        m_currentFile.close();
    }
}

bool CircularLogger::loadConfig(const QString &configPath) {
    QFile configFile(configPath);
    if (!configFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open config file:" << configPath;
        return false;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(configFile.readAll(), &parseError);
    configFile.close();

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return false;
    }

    QJsonObject obj = doc.object();
    QString logTypeStr = obj["logType"].toString().toLower();
    if (logTypeStr == "hourly") m_logType = Hourly;
    else if (logTypeStr == "minutely") m_logType = Minutely;
    else if (logTypeStr == "secondly") m_logType = Secondly;
    else m_logType = Daily;

    m_maxFiles = obj["maxFiles"].toInt(7);
    m_frequency = obj["frequency"].toInt(1);
    m_logDir = obj["logDir"].toString("logs");

    QDir dir;
    if (!dir.exists(m_logDir)) {
        dir.mkpath(m_logDir);
    }

    return true;
}

void CircularLogger::log(const QString &message) {
    QDateTime currentTime = QDateTime::currentDateTimeUtc();
    if (!m_currentFile.isOpen() || shouldCreateNewFile(currentTime)) {
        openNewLogFile(currentTime);
        m_lastLogTime = currentTime;
    }

    QTextStream out(&m_currentFile);
    out << currentTime.toString("yyyy-MM-dd HH:mm:ss") << " UTC: " << message << "\n";
    out.flush();
}

QString CircularLogger::getLogFileName(const QDateTime &time) const {
    QString format;
    switch (m_logType) {
    case Daily: format = "yyyy-MM-dd"; break;
    case Hourly: format = "yyyy-MM-dd_HH"; break;
    case Minutely: format = "yyyy-MM-dd_HH-mm"; break;
    case Secondly: format = "yyyy-MM-dd_HH-mm-ss"; break;
    }
    return QString("%1/log_%2.txt").arg(m_logDir, time.toString(format));
}

void CircularLogger::openNewLogFile(const QDateTime &time) {
    if (m_currentFile.isOpen()) {
        m_currentFile.close();
    }

    QString fileName = getLogFileName(time);
    m_currentFile.setFileName(fileName);
    if (!m_currentFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Failed to open log file:" << fileName;
        return;
    }

    manageLogFiles();
}

void CircularLogger::manageLogFiles() {
    QDir dir(m_logDir);
    QStringList filters = {"log_*.txt"};
    QStringList files = dir.entryList(filters, QDir::Files, QDir::Name);

    if (files.count() > m_maxFiles) {
        for (int i = 0; i < files.count() - m_maxFiles; ++i) {
            dir.remove(files[i]);
            qDebug() << "Deleted old log file:" << files[i];
        }
    }
}

bool CircularLogger::shouldCreateNewFile(const QDateTime &currentTime) const {
    if (!m_lastLogTime.isValid()) return true;

    qint64 diff = m_lastLogTime.secsTo(currentTime);
    switch (m_logType) {
    case Daily: return diff >= m_frequency * 24 * 3600;
    case Hourly: return diff >= m_frequency * 3600;
    case Minutely: return diff >= m_frequency * 60;
    case Secondly: return diff >= m_frequency;
    }
    return false;
}
