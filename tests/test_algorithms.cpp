#include <QtTest/QtTest>
#include "algorithms.h"

/*!
 * @brief Тесты алгоритмов: шифр Виженера, SHA-512, деление пополам, BFS.
 */
class TestAlgorithms : public QObject
{
    Q_OBJECT

private slots:
    // ─── Шифр Виженера ────────────────────────────────────────────────────────

    void vigenereEncrypt_basicWord()
    {
        QCOMPARE(vigenereEncrypt("HELLO", "KEY"), QString("RIJVS"));
    }

    void vigenereEncrypt_singleChar()
    {
        QCOMPARE(vigenereEncrypt("ABC", "B"), QString("BCD"));
    }

    void vigenereEncrypt_wrapAround()
    {
        QCOMPARE(vigenereEncrypt("XYZ", "D"), QString("ABC"));
    }

    void vigenereDecrypt_inverse()
    {
        QCOMPARE(vigenereDecrypt("RIJVS", "KEY"), QString("HELLO"));
    }

    void vigenereDecrypt_singleChar()
    {
        QCOMPARE(vigenereDecrypt("BCD", "B"), QString("ABC"));
    }

    void vigenereEncryptDecrypt_roundtrip()
    {
        const QString original = "HelloWorld";
        const QString key = "SECRET";
        QCOMPARE(vigenereDecrypt(vigenereEncrypt(original, key), key), original);
    }

    void vigenereEncrypt_preservesNonAlpha()
    {
        // Пробелы и цифры не шифруются; ключ "K" сдвигает все буквы на 10
        QString result = vigenereEncrypt("A B 1", "K");
        QCOMPARE(result, QString("K L 1"));
    }

    void vigenereEncrypt_keyLongerThanText()
    {
        // H+K=R, I+E=M
        QCOMPARE(vigenereEncrypt("HI", "KEYLONG"), QString("RM"));
    }

    void vigenereEncrypt_lowercasePreserved()
    {
        // Строчные буквы сохраняют регистр
        QString result = vigenereEncrypt("hello", "KEY");
        QCOMPARE(result, QString("rijvs"));
    }

    // ─── SHA-512 ──────────────────────────────────────────────────────────────

    void sha512_emptyString()
    {
        // SHA-512("") — эталонное значение RFC 6234
        const QString expected =
            "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce"
            "47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e";
        QCOMPARE(sha512Hex(""), expected);
    }

    void sha512_knownValue()
    {
        // SHA-512("abc") — эталонное значение RFC 6234
        const QString expected =
            "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a"
            "2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f";
        QCOMPARE(sha512Hex("abc"), expected);
    }

    void sha512_length()
    {
        // SHA-512 всегда 128 hex символов
        QCOMPARE(sha512Hex("test").length(), 128);
        QCOMPARE(sha512Hex("").length(), 128);
    }

    void sha512_deterministic()
    {
        QCOMPARE(sha512Hex("polytech"), sha512Hex("polytech"));
    }

    // ─── Метод деления пополам ────────────────────────────────────────────────

    void bisection_perfectSquare4()
    {
        QVERIFY(qAbs(bisectionSqrt(4.0) - 2.0) < 1e-10);
    }

    void bisection_perfectSquare9()
    {
        QVERIFY(qAbs(bisectionSqrt(9.0) - 3.0) < 1e-10);
    }

    void bisection_two()
    {
        QVERIFY(qAbs(bisectionSqrt(2.0) - 1.41421356237) < 1e-9);
    }

    void bisection_zero()
    {
        QVERIFY(qAbs(bisectionSqrt(0.0) - 0.0) < 1e-15);
    }

    void bisection_one()
    {
        QVERIFY(qAbs(bisectionSqrt(1.0) - 1.0) < 1e-10);
    }

    void bisection_fraction()
    {
        // sqrt(0.25) = 0.5
        QVERIFY(qAbs(bisectionSqrt(0.25) - 0.5) < 1e-10);
    }

    void bisection_large()
    {
        // sqrt(10000) = 100
        QVERIFY(qAbs(bisectionSqrt(10000.0) - 100.0) < 1e-8);
    }

    // ─── BFS на графе ─────────────────────────────────────────────────────────
    // Граф: рёбра 1-2, 1-3, 2-4, 2-5, 3-5, 3-6, 4-7, 5-7, 6-7

    void bfs_sameVertex()
    {
        QCOMPARE(shortestPathBFS(1, 1), 0);
    }

    void bfs_directEdge()
    {
        QCOMPARE(shortestPathBFS(1, 2), 1);
        QCOMPARE(shortestPathBFS(1, 3), 1);
    }

    void bfs_twoHops()
    {
        QCOMPARE(shortestPathBFS(1, 4), 2);
        QCOMPARE(shortestPathBFS(1, 5), 2);
        QCOMPARE(shortestPathBFS(1, 6), 2);
    }

    void bfs_threeHops()
    {
        QCOMPARE(shortestPathBFS(1, 7), 3);
    }

    void bfs_symmetric()
    {
        // Граф неориентированный — расстояния симметричны
        QCOMPARE(shortestPathBFS(7, 1), shortestPathBFS(1, 7));
        QCOMPARE(shortestPathBFS(4, 6), shortestPathBFS(6, 4));
    }

    void bfs_acrossGraph()
    {
        // 4 -> 2 -> 5 -> 7 -> 6  (или 4->7->6), минимум 2
        QCOMPARE(shortestPathBFS(4, 6), 2);
    }

    void bfs_invalidVertex()
    {
        // Вершина 0 не в графе
        QCOMPARE(shortestPathBFS(0, 1), -1);
        QCOMPARE(shortestPathBFS(1, 8), -1);
    }
};

QTEST_MAIN(TestAlgorithms)
#include "test_algorithms.moc"
