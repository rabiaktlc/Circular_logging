#include <QtTest>
#include <QTemporaryDir>
#include "CircularLogger.h"

class TestCircularLogger : public QObject {
    Q_OBJECT
private slots:
    void initTestCase();
    void testLoadConfig();
    void testLogDaily();
    void testLogRotation();
    void testMaxFiles();
    void cleanupTestCase();

private:
    QTemporaryDir tempDir;
    CircularLogger logger;
};

void TestCircularLogger::initTestCase() {
    // Ensure the temporary directory is valid
    QVERIFY(tempDir.isValid());
}

void TestCircularLogger::testLoadConfig() {
    // Create a sample config file
    QFile configFile(tempDir.path() + "/config.json");
    QVERIFY(configFile.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream out(&configFile);
    out << R"({
        "logType": "Hourly",
        "maxFiles": 5,
        "frequency": 1,
        "logDir": ")" << tempDir.path() << R"("
    })";
    configFile.close();

    // Test loading config
    QVERIFY(logger.loadConfig(configFile.fileName()));
    QCOMPARE(logger.property("logType").toInt(), CircularLogger::Hourly);
    QCOMPARE(logger.property("maxFiles").toInt(), 5);
    QCOMPARE(logger.property("frequency").toInt(), 1);
    QCOMPARE(logger.property("logDir").toString(), tempDir.path());
}

void TestCircularLogger::testLogDaily() {
    // Set up logger with Daily log type
    QFile configFile(tempDir.path() + "/config.json");
    QVERIFY(configFile.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream out(&configFile);
    out << R"({
        "logType": "Daily",
        "maxFiles": 5,
        "frequency": 1,
        "logDir": ")" << tempDir.path() << R"("
    })";
    configFile.close();
    QVERIFY(logger.loadConfig(configFile.fileName()));

    // Log a message
    logger.log("Test message");
    QFile logFile(tempDir.path() + "/log_" + QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd") + ".txt");
    QVERIFY(logFile.exists());
    QVERIFY(logFile.open(QIODevice::ReadOnly | QIODevice::Text));
    QString content = logFile.readAll();
    QVERIFY(content.contains("Test message"));
    logFile.close();
}

void TestCircularLogger::testLogRotation() {
    // Set up logger with Secondly log type for faster rotation
    QFile configFile(tempDir.path() + "/config.json");
    QVERIFY(configFile.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream out(&configFile);
    out << R"({
        "logType": "Secondly",
        "maxFiles": 5,
        "frequency": 1,
        "logDir": ")" << tempDir.path() << R"("
    })";
    configFile.close();
    QVERIFY(logger.loadConfig(configFile.fileName()));

    // Log messages with time manipulation
    QDateTime time1 = QDateTime::currentDateTimeUtc();
    logger.log("Message 1");
    QString file1 = tempDir.path() + "/log_" + time1.toString("yyyy-MM-dd_HH-mm-ss") + ".txt";
    QVERIFY(QFile(file1).exists());

    // Simulate time passing (1 second later)
    QDateTime time2 = time1.addSecs(1);
    logger.log("Message 2");
    QString file2 = tempDir.path() + "/log_" + time2.toString("yyyy-MM-dd_HH-mm-ss") + ".txt";
    QVERIFY(QFile(file2).exists());
    QVERIFY(file1 != file2);
}

void TestCircularLogger::testMaxFiles() {
    // Set up logger with maxFiles = 3
    QFile configFile(tempDir.path() + "/config.json");
    QVERIFY(configFile.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream out(&configFile);
    out << R"({
        "logType": "Daily",
        "maxFiles": 3,
        "frequency": 1,
        "logDir": ")" << tempDir.path() << R"("
    })";
    configFile.close();
    QVERIFY(logger.loadConfig(configFile.fileName()));

    // Create 5 log files
    for (int i = 0; i < 5; ++i) {
        QDateTime time = QDateTime::currentDateTimeUtc().addDays(-i);
        QFile file(tempDir.path() + "/log_" + time.toString("yyyy-MM-dd") + ".txt");
        QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Text));
        file.close();
    }

    // Log a new message to trigger file management
    logger.log("Test max files");
    QDir dir(tempDir.path());
    QStringList files = dir.entryList(QStringList() << "log_*.txt", QDir::Files, QDir::Name);
    QCOMPARE(files.count(), 3); // Should only have 3 files
}

void TestCircularLogger::cleanupTestCase() {
    // Temporary directory is automatically cleaned up
}

QTEST_MAIN(TestCircularLogger)
#include "test_CircularLogger.moc"
