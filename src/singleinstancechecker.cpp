#include "singleinstancechecker.h"
SingleInstanceChecker::SingleInstanceChecker(QObject *parent)
    : QObject(parent)
    , sharedMemory_("RAXBOXCLIENT-SharedMemory")
{
    sharedMemory_.create(1); // Create a shared memory segment of size 1 byte

    if (sharedMemory_.error() == QSharedMemory::AlreadyExists) {
        // Another instance is already running
        // Activate the existing instance by connecting to it
        QLocalSocket socket;
        socket.connectToServer("RAXBOXCLIENT-LocalServer");

        if (socket.waitForConnected()) {
            // Send a message to the existing instance to activate it (e.g., bring it to the front)
            QByteArray message;
            QDataStream stream(&message, QIODevice::WriteOnly);
            stream << QString("activate");

            socket.write(message);
            socket.waitForBytesWritten();
        }

        // Exit the current instance
        exit(0);
    } else {
        // Create a local server to listen for connections from subsequent instances
        connect(&localServer_,
                &QLocalServer::newConnection,
                this,
                &SingleInstanceChecker::handleNewConnection);
        localServer_.listen("RAXBOXCLIENT-LocalServer");
    }
}

void SingleInstanceChecker::handleNewConnection()
{
    // Accept the connection from the subsequent instance
    QLocalSocket *socket = localServer_.nextPendingConnection();
    socket->waitForReadyRead();

    // Read the incoming message
    QByteArray message = socket->readAll();
    QDataStream stream(&message, QIODevice::ReadOnly);

    QString command;
    stream >> command;

    // Process the command as needed
    if (command == "activate") {
        qDebug() << "Received activate command from another instance";

        // Perform activation logic here (e.g., bring the application window to the front)
        emit activate();
    }

    // Close the socket
    socket->disconnectFromServer();
    socket->deleteLater();
}
