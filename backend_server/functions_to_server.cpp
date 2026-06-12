#include "functions_to_server.h"
#include "algorithms.h"
#include "databasemanager.h"

// ─── Регистрация и авторизация ────────────────────────────────────────────────

QString handleRegister(const QString &payload)
{
    int sep = payload.indexOf(',');
    if (sep < 0) return "ERROR: Format must be login,password";
    return DatabaseManager::instance().registerUser(
        payload.left(sep).trimmed(),
        payload.mid(sep + 1)
    );
}

QString handleAuth(const QString &payload)
{
    int sep = payload.indexOf(',');
    if (sep < 0) return "ERROR: Format must be login,password";
    QString role = DatabaseManager::instance().authUser(
        payload.left(sep).trimmed(),
        payload.mid(sep + 1)
    );
    return role.isEmpty() ? "ERROR: Invalid credentials" : "OK:" + role;
}

QString handleStats()
{
    return QString("Users: %1").arg(DatabaseManager::instance().userCount());
}

// ─── Администратор ───────────────────────────────────────────────────────────

QString handleAdminUsers(const QString &callerRole)
{
    if (callerRole != "admin") return "ERROR: Access denied";
    QStringList users = DatabaseManager::instance().allUsers();
    return users.isEmpty() ? "EMPTY" : users.join("|");
}

QString handleAdminDelete(const QString &payload, const QString &callerRole)
{
    if (callerRole != "admin") return "ERROR: Access denied";
    QString login = payload.trimmed();
    if (login.isEmpty())   return "ERROR: Login is empty";
    if (login == "admin")  return "ERROR: Cannot delete admin";
    return DatabaseManager::instance().deleteUser(login)
        ? "OK: Deleted " + login
        : "ERROR: User not found";
}

// ─── Шифр Виженера ────────────────────────────────────────────────────────────

QString handleVigenereEncrypt(const QString &payload)
{
    int sep = payload.indexOf(',');
    if (sep < 0) return "ERROR: Format must be text,key";
    QString key = payload.mid(sep + 1);
    if (key.isEmpty()) return "ERROR: Key is empty";
    return "ENCRYPTED: " + vigenereEncrypt(payload.left(sep), key);
}

QString handleVigenereDecrypt(const QString &payload)
{
    int sep = payload.indexOf(',');
    if (sep < 0) return "ERROR: Format must be text,key";
    QString key = payload.mid(sep + 1);
    if (key.isEmpty()) return "ERROR: Key is empty";
    return "DECRYPTED: " + vigenereDecrypt(payload.left(sep), key);
}

// ─── SHA-512 ──────────────────────────────────────────────────────────────────

QString handleSHA512(const QString &payload)
{
    if (payload.isEmpty()) return "ERROR: Input is empty";
    return sha512Hex(payload);
}

// ─── Метод деления пополам ────────────────────────────────────────────────────

QString handleBisect(const QString &payload)
{
    bool ok;
    double value = payload.trimmed().toDouble(&ok);
    if (!ok)       return "ERROR: Not a number";
    if (value < 0) return "ERROR: Square root of negative number is undefined";
    return QString("sqrt(%1) = %2").arg(value, 0, 'g', 10).arg(bisectionSqrt(value), 0, 'g', 15);
}

// ─── Поиск кратчайшего пути в графе (BFS) ────────────────────────────────────

QString handleShortest(const QString &payload)
{
    int sep = payload.indexOf(',');
    if (sep < 0) return "ERROR: Format must be s,t";
    bool ok1, ok2;
    int s = payload.left(sep).trimmed().toInt(&ok1);
    int t = payload.mid(sep + 1).trimmed().toInt(&ok2);
    if (!ok1 || !ok2) return "ERROR: Vertices must be integers (1-7)";
    int dist = shortestPathBFS(s, t);
    if (dist == -1) {
        if (s < 1 || s > 7) return QString("ERROR: Vertex %1 not in graph (1-7)").arg(s);
        if (t < 1 || t > 7) return QString("ERROR: Vertex %1 not in graph (1-7)").arg(t);
        return QString("No path from %1 to %2").arg(s).arg(t);
    }
    return QString("Distance(%1 -> %2) = %3").arg(s).arg(t).arg(dist);
}
