#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>
#include <QList>

/*!
 * @brief TCP-сервер, обрабатывающий команды от нескольких клиентов.
 *
 * Поддерживает сессии: после успешного auth хранит логин и роль клиента.
 * Формат команды: @c action&payload, завершённый @c '\\n'.
 */
class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(quint16 port = 33333, QObject *parent = nullptr);
    ~MyTcpServer();

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    void sendResponse(QTcpSocket *client, const QByteArray &response);

    QTcpServer                                       *m_server;
    QList<QTcpSocket*>                                m_clients;
    QHash<QTcpSocket*, QByteArray>                    m_buffers;
    QHash<QTcpSocket*, QPair<QString,QString>>         m_sessions; ///< socket -> (login, role)
};

#endif // MYTCPSERVER_H
