#ifndef FUNCTIONS_TO_SERVER_H
#define FUNCTIONS_TO_SERVER_H

#include <QString>

// ─── Регистрация и авторизация ────────────────────────────────────────────────

/*! @brief Зарегистрировать пользователя. payload = "login,password" */
QString handleRegister(const QString &payload);

/*! @brief Аутентифицировать пользователя. payload = "login,password". @return "OK:role" или "ERROR:..." */
QString handleAuth(const QString &payload);

/*! @brief Количество пользователей в БД. */
QString handleStats();

// ─── Администратор ────────────────────────────────────────────────────────────

/*! @brief Список всех пользователей (только admin). @param callerRole Роль вызывающего клиента */
QString handleAdminUsers(const QString &callerRole);

/*! @brief Удалить пользователя (только admin). payload = login. @param callerRole Роль вызывающего */
QString handleAdminDelete(const QString &payload, const QString &callerRole);

// ─── Алгоритмы ────────────────────────────────────────────────────────────────

/*! @brief Зашифровать строку шифром Виженера. payload = "text,key" (только латинские буквы) */
QString handleVigenereEncrypt(const QString &payload);

/*! @brief Расшифровать строку шифром Виженера. payload = "text,key" */
QString handleVigenereDecrypt(const QString &payload);

/*! @brief Вычислить SHA-512 хэш. payload = любой текст */
QString handleSHA512(const QString &payload);

/*! @brief Вычислить sqrt методом деления пополам (60 итераций). payload = число */
QString handleBisect(const QString &payload);

/*!
 * @brief Найти длину кратчайшего пути BFS в фиксированном графе (7 вершин).
 * payload = "s,t"
 */
QString handleShortest(const QString &payload);

#endif // FUNCTIONS_TO_SERVER_H
