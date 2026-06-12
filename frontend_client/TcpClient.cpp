#include "TcpClient.h"

TcpClient::TcpClient(QObject *parent)
    : QObject(parent),
      m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::disconnected, this, [this]() {
        emit connectionChanged(false);
    });
}

TcpClient &TcpClient::instance()
{
    static TcpClient inst;
    return inst;
}

bool TcpClient::connectToServer(const QString &host, quint16 port)
{
    m_socket->connectToHost(host, port);
    if (!m_socket->waitForConnected(2000)) return false;
    // Считываем приветственное сообщение сервера
    m_socket->waitForReadyRead(1000);
    m_socket->readAll();
    emit connectionChanged(true);
    return true;
}

void TcpClient::disconnectFromServer()
{
    m_socket->disconnectFromHost();
}

bool TcpClient::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

QString TcpClient::sendCommand(const QString &cmd)
{
    if (!isConnected()) return "ОШИБКА: Нет подключения к серверу";
    m_socket->write((cmd + "\n").toUtf8());
    m_socket->flush();
    if (!m_socket->waitForReadyRead(2000))
        return "ОШИБКА: Нет ответа от сервера";
    return QString::fromUtf8(m_socket->readAll().trimmed());
}
