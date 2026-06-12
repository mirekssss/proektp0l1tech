#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QStringList>
#include <QSqlDatabase>

/*!
 * @brief Синглтон для работы с базой данных пользователей (SQLite).
 *
 * Таблица users: id, login, password_hash (SHA-512), role (user/admin).
 * При первом запуске автоматически создаёт учётную запись admin/admin.
 */
class DatabaseManager
{
public:
    /*! @brief Получить единственный экземпляр. */
    static DatabaseManager &instance();

    /*!
     * @brief Зарегистрировать нового пользователя с ролью "user".
     * @param login    Логин
     * @param password Пароль в открытом виде (хэшируется внутри SHA-512)
     * @return "OK: Registered" или "ERROR: ..."
     */
    QString registerUser(const QString &login, const QString &password);

    /*!
     * @brief Аутентифицировать пользователя.
     * @return Роль ("user" или "admin") при успехе, иначе пустую строку
     */
    QString authUser(const QString &login, const QString &password);

    /*! @brief Список всех пользователей в формате "login:role". */
    QStringList allUsers();

    /*!
     * @brief Удалить пользователя (нельзя удалить admin).
     * @return true при успехе
     */
    bool deleteUser(const QString &login);

    /*! @brief Количество зарегистрированных пользователей. */
    int userCount();

private:
    DatabaseManager();
    DatabaseManager(const DatabaseManager &) = delete;
    DatabaseManager &operator=(const DatabaseManager &) = delete;

    void initSchema();
    static QString hashPassword(const QString &password);

    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
