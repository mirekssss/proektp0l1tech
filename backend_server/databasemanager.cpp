#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>

QString DatabaseManager::hashPassword(const QString &password)
{
    return QString::fromLatin1(
        QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha512).toHex()
    );
}

DatabaseManager::DatabaseManager()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("users.db");
    if (!m_db.open()) {
        qCritical() << "Cannot open database:" << m_db.lastError().text();
        return;
    }
    initSchema();
}

DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager inst;
    return inst;
}

void DatabaseManager::initSchema()
{
    QSqlQuery q(m_db);
    q.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "  id            INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  login         TEXT    UNIQUE NOT NULL,"
        "  password_hash TEXT    NOT NULL,"
        "  role          TEXT    NOT NULL DEFAULT 'user'"
        ")"
    );

    // Создать администратора по умолчанию при пустой таблице
    q.exec("SELECT COUNT(*) FROM users");
    if (q.next() && q.value(0).toInt() == 0) {
        QSqlQuery ins(m_db);
        ins.prepare("INSERT INTO users (login, password_hash, role) VALUES (?, ?, 'admin')");
        ins.addBindValue("admin");
        ins.addBindValue(hashPassword("admin"));
        ins.exec();
        qDebug() << "Default admin created (login: admin, password: admin)";
    }
}

QString DatabaseManager::registerUser(const QString &login, const QString &password)
{
    if (login.trimmed().isEmpty())    return "ERROR: Login is empty";
    if (password.trimmed().isEmpty()) return "ERROR: Password is empty";

    QSqlQuery check(m_db);
    check.prepare("SELECT id FROM users WHERE login = ?");
    check.addBindValue(login);
    check.exec();
    if (check.next()) return "ERROR: User already exists";

    QSqlQuery ins(m_db);
    ins.prepare("INSERT INTO users (login, password_hash, role) VALUES (?, ?, 'user')");
    ins.addBindValue(login);
    ins.addBindValue(hashPassword(password));
    if (!ins.exec()) return "ERROR: " + ins.lastError().text();
    return "OK: Registered";
}

QString DatabaseManager::authUser(const QString &login, const QString &password)
{
    QSqlQuery q(m_db);
    q.prepare("SELECT role FROM users WHERE login = ? AND password_hash = ?");
    q.addBindValue(login);
    q.addBindValue(hashPassword(password));
    q.exec();
    if (q.next()) return q.value(0).toString();
    return "";
}

QStringList DatabaseManager::allUsers()
{
    QStringList result;
    QSqlQuery q("SELECT login, role FROM users ORDER BY role DESC, login ASC", m_db);
    while (q.next())
        result << q.value(0).toString() + ":" + q.value(1).toString();
    return result;
}

bool DatabaseManager::deleteUser(const QString &login)
{
    if (login == "admin") return false;
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM users WHERE login = ?");
    q.addBindValue(login);
    return q.exec() && q.numRowsAffected() > 0;
}

int DatabaseManager::userCount()
{
    QSqlQuery q("SELECT COUNT(*) FROM users", m_db);
    return q.next() ? q.value(0).toInt() : 0;
}
