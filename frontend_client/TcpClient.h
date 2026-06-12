#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

/*!
 * @brief Синглтон-обёртка над QTcpSocket для связи с сервером.
 *
 * Единственный экземпляр создаётся при первом вызове instance().
 * Все команды отправляются через sendCommand().
 */
class TcpClient : public QObject
{
    Q_OBJECT
public:
    /*! @brief Получить единственный экземпляр. */
    static TcpClient &instance();

    /*!
     * @brief Подключиться к серверу.
     * @param host IP-адрес (по умолчанию 127.0.0.1)
     * @param port Порт (по умолчанию 33333)
     * @return true при успехе
     */
    bool connectToServer(const QString &host = "127.0.0.1", quint16 port = 33333);

    /*! @brief Разорвать соединение. */
    void disconnectFromServer();

    /*! @brief Возвращает true, если соединение активно. */
    bool isConnected() const;

    /*!
     * @brief Отправить команду и получить ответ.
     * @param cmd Строка команды формата "action&payload"
     * @return Ответ сервера или сообщение об ошибке
     */
    QString sendCommand(const QString &cmd);

signals:
    /*! @brief Испускается при изменении состояния соединения. */
    void connectionChanged(bool connected);

private:
    explicit TcpClient(QObject *parent = nullptr);
    TcpClient(const TcpClient &) = delete;
    TcpClient &operator=(const TcpClient &) = delete;

    QTcpSocket *m_socket;
};

#endif // TCPCLIENT_H
