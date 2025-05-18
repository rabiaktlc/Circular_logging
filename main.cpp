#include <QCoreApplication>
#include <QTimer>
#include "CircularLogger.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    qDebug() << "Starting CircularLogging application...";

    CircularLogger logger;
    qDebug() << "Loading config...";
    if (!logger.loadConfig("config.json")) {
        qWarning() << "Failed to load configuration. Exiting.";
        return 1;
    }
    qDebug() << "Config loaded successfully.";

    // Log a message every 2 seconds for testing
    QTimer timer;
    int counter = 1;
    QObject::connect(&timer, &QTimer::timeout, [&logger, &counter]() {
        qDebug() << "Logging message #" << counter;
        logger.log(QString("Test log message #%1").arg(counter++));
    });
    qDebug() << "Starting timer...";
    timer.start(200);

    qDebug() << "Entering event loop...";
    return app.exec();
}
