#include "mytcpserver.h"
#include "functions_to_server.h"
#include <QDebug>

MyTcpServer::MyTcpServer(quint16 port, QObject *parent)
    : QObject(parent),
      m_server(new QTcpServer(this))
{
    connect(m_server, &QTcpServer::newConnection,
            this, &MyTcpServer::onNewConnection);

    if (!m_server->listen(QHostAddress::Any, port)) {
        qCritical() << "Server could not start on port" << port;
    } else {
        qDebug() << "Server started, listening on port" << port;
    }
}

MyTcpServer::~MyTcpServer()
{
    m_server->close();
    qDeleteAll(m_clients);
}

void MyTcpServer::onNewConnection()
{
    while (m_server->hasPendingConnections()) {
        QTcpSocket *client = m_server->nextPendingConnection();
        m_clients.append(client);
        m_buffers[client].clear();

        connect(client, &QTcpSocket::readyRead,
                this, &MyTcpServer::onReadyRead);
        connect(client, &QTcpSocket::disconnected,
                this, &MyTcpServer::onClientDisconnected);

        qDebug() << "New connection from:" << client->peerAddress().toString();
        sendResponse(client, "CONNECTED: Ready for commands\r\n");
    }
}

void MyTcpServer::onReadyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    m_buffers[client] += client->readAll();

    int idx;
    while ((idx = m_buffers[client].indexOf('\n')) != -1) {
        QByteArray raw = m_buffers[client].left(idx).trimmed();
        m_buffers[client].remove(0, idx + 1);

        QString command = QString::fromUtf8(raw);
        qDebug() << "Received:" << command;

        QStringList parts = command.split('&');
        if (parts.isEmpty()) {
            sendResponse(client, "ERROR: Empty command\r\n");
            continue;
        }

        QString action  = parts.takeFirst().toLower();
        QString payload = parts.join("&");
        QString callerRole = m_sessions.value(client).second; // "" если не авторизован
        QString result;

        // ── Регистрация / авторизация ─────────────────────────────────────────
        if (action == "reg") {
            result = handleRegister(payload);

        } else if (action == "auth") {
            result = handleAuth(payload);
            if (result.startsWith("OK:")) {
                QString role  = result.mid(3);
                QString login = payload.left(payload.indexOf(',')).trimmed();
                m_sessions[client] = {login, role};
                qDebug() << "Session opened:" << login << "role:" << role;
            }

        } else if (action == "logout") {
            QString login = m_sessions.value(client).first;
            m_sessions.remove(client);
            result = "OK: Logged out" + (login.isEmpty() ? "" : " (" + login + ")");

        } else if (action == "stats") {
            result = handleStats();

        // ── Администратор ─────────────────────────────────────────────────────
        } else if (action == "admin_users") {
            result = handleAdminUsers(callerRole);

        } else if (action == "admin_delete") {
            result = handleAdminDelete(payload, callerRole);

        // ── Алгоритмы ─────────────────────────────────────────────────────────
        } else if (action == "vigenere_enc") {
            result = handleVigenereEncrypt(payload);
        } else if (action == "vigenere_dec") {
            result = handleVigenereDecrypt(payload);
        } else if (action == "sha512") {
            result = handleSHA512(payload);
        } else if (action == "bisect") {
            result = handleBisect(payload);
        } else if (action == "graph") {
            result = handleShortest(payload);

        } else {
            result = "ECHO: " + command;
        }

        sendResponse(client, result.toUtf8() + "\r\n");
    }
}

void MyTcpServer::onClientDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    qDebug() << "Client disconnected:" << client->peerAddress().toString();
    m_clients.removeAll(client);
    m_buffers.remove(client);
    m_sessions.remove(client);
    client->deleteLater();
}

void MyTcpServer::sendResponse(QTcpSocket *client, const QByteArray &response)
{
    if (client && client->state() == QTcpSocket::ConnectedState) {
        client->write(response);
        client->flush();
    }
}
