#include "algorithms.h"
#include <QCryptographicHash>
#include <QHash>
#include <QVector>
#include <QQueue>

// ─── Шифр Виженера ────────────────────────────────────────────────────────────

static QString vigenereProcess(const QString &text, const QString &key, bool encrypt)
{
    QString result;
    QString upperKey = key.toUpper();
    int ki = 0;
    for (const QChar &ch : text) {
        if (ch.isLetter() && ch.toLatin1() != 0) {
            QChar base = ch.isUpper() ? QChar('A') : QChar('a');
            int shift  = upperKey[ki % upperKey.size()].toLatin1() - 'A';
            int pos    = ch.toLatin1() - base.toLatin1();
            int newPos = encrypt ? (pos + shift) % 26 : (pos - shift + 26) % 26;
            result += QChar(base.toLatin1() + newPos);
            ++ki;
        } else {
            result += ch;
        }
    }
    return result;
}

QString vigenereEncrypt(const QString &text, const QString &key)
{
    return vigenereProcess(text, key, true);
}

QString vigenereDecrypt(const QString &text, const QString &key)
{
    return vigenereProcess(text, key, false);
}

// ─── SHA-512 ──────────────────────────────────────────────────────────────────

QString sha512Hex(const QString &text)
{
    QByteArray hash = QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Sha512);
    return QString::fromLatin1(hash.toHex());
}

// ─── Метод деления пополам ────────────────────────────────────────────────────

double bisectionSqrt(double value)
{
    double lo = 0.0, hi = (value < 1.0) ? 1.0 : value;
    for (int i = 0; i < 60; ++i) {
        double mid = (lo + hi) / 2.0;
        if (mid * mid < value) lo = mid; else hi = mid;
    }
    return (lo + hi) / 2.0;
}

// ─── Кратчайший путь (BFS) ────────────────────────────────────────────────────

struct Graph {
    QHash<int, QVector<int>> adj;
    void addEdge(int u, int v) { adj[u].append(v); adj[v].append(u); }
    int shortestPath(int s, int t) const {
        if (s == t) return 0;
        QHash<int, int> dist;
        QQueue<int> q;
        dist[s] = 0; q.enqueue(s);
        while (!q.isEmpty()) {
            int u = q.dequeue();
            for (int v : adj.value(u)) {
                if (!dist.contains(v)) {
                    dist[v] = dist[u] + 1;
                    if (v == t) return dist[v];
                    q.enqueue(v);
                }
            }
        }
        return -1;
    }
};

static const Graph& graph()
{
    static Graph g = []() {
        Graph g;
        g.addEdge(1,2); g.addEdge(1,3); g.addEdge(2,4); g.addEdge(2,5);
        g.addEdge(3,5); g.addEdge(3,6); g.addEdge(4,7); g.addEdge(5,7); g.addEdge(6,7);
        return g;
    }();
    return g;
}

int shortestPathBFS(int s, int t)
{
    const Graph &g = graph();
    if (!g.adj.contains(s) || !g.adj.contains(t)) return -1;
    return g.shortestPath(s, t);
}
