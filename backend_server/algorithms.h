#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QString>

/*!
 * @brief Зашифровать текст шифром Виженера (только латинские буквы).
 * @param text Исходный текст
 * @param key  Ключ (только латинские буквы, регистр игнорируется)
 * @return Зашифрованный текст
 */
QString vigenereEncrypt(const QString &text, const QString &key);

/*!
 * @brief Расшифровать текст шифром Виженера.
 * @param text Зашифрованный текст
 * @param key  Ключ
 * @return Расшифрованный текст
 */
QString vigenereDecrypt(const QString &text, const QString &key);

/*!
 * @brief Вычислить SHA-512 хэш строки.
 * @return 128-символьная hex-строка
 */
QString sha512Hex(const QString &text);

/*!
 * @brief Вычислить квадратный корень методом деления пополам (60 итераций).
 * @param value Неотрицательное число
 * @return Приближённое значение sqrt(value)
 */
double bisectionSqrt(double value);

/*!
 * @brief Найти длину кратчайшего пути BFS в фиксированном графе (7 вершин).
 *
 * Граф: рёбра 1-2, 1-3, 2-4, 2-5, 3-5, 3-6, 4-7, 5-7, 6-7.
 * @return Расстояние >= 0, или -1 если путь не найден / вершина не существует
 */
int shortestPathBFS(int s, int t);

#endif // ALGORITHMS_H
