#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLineEdit;
class QLabel;
class QPushButton;
class QTableWidget;
class QGroupBox;
class QComboBox;

/*!
 * @brief Главное окно GUI-клиента.
 *
 * Включает панели: регистрация, авторизация, алгоритмы, история.
 * Панель администратора отображается только при входе с ролью admin.
 * Использует TcpClient (синглтон) для сетевого взаимодействия.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnectClicked();
    void onConnectionChanged(bool connected);
    void onRegisterClicked();
    void onLoginClicked();
    void onAdminRefreshClicked();
    void onAdminDeleteClicked();
    void onVigenereEncryptClicked();
    void onVigenereDecryptClicked();
    void onSHA512Clicked();
    void onBisectClicked();
    void onGraphClicked();
    void onHistFilterChanged();

private:
    void setupUI();
    void addHistoryRow(const QString &op, const QString &input, const QString &result);
    void setAdminPanelVisible(bool visible);
    void updateLoginStatus(const QString &login, const QString &role);

    QString m_currentRole;

    // Подключение
    QPushButton  *m_connectButton;
    QLabel       *m_statusLabel;

    // Регистрация
    QLineEdit    *m_regLoginEdit;
    QLineEdit    *m_regPassEdit;
    QPushButton  *m_regButton;
    QLabel       *m_regResultLabel;

    // Авторизация
    QLineEdit    *m_logLoginEdit;
    QLineEdit    *m_logPassEdit;
    QPushButton  *m_logButton;
    QLabel       *m_logResultLabel;

    // Панель администратора
    QGroupBox    *m_adminGroup;
    QTableWidget *m_adminUsersTable;
    QPushButton  *m_adminRefreshButton;
    QLineEdit    *m_adminDeleteEdit;
    QPushButton  *m_adminDeleteButton;
    QLabel       *m_adminResultLabel;

    // Шифр Виженера
    QLineEdit    *m_vigTextEdit;
    QLineEdit    *m_vigKeyEdit;
    QPushButton  *m_vigEncButton;
    QPushButton  *m_vigDecButton;
    QLabel       *m_vigResultLabel;

    // SHA-512
    QLineEdit    *m_shaInputEdit;
    QPushButton  *m_shaButton;
    QLabel       *m_shaResultLabel;

    // Метод деления пополам
    QLineEdit    *m_bisectInputEdit;
    QPushButton  *m_bisectButton;
    QLabel       *m_bisectResultLabel;

    // Кратчайший путь в графе
    QLineEdit    *m_graphSEdit;
    QLineEdit    *m_graphTEdit;
    QPushButton  *m_graphButton;
    QLabel       *m_graphResultLabel;

    // История запросов
    QTableWidget *m_historyTable;
    QComboBox    *m_histFilterCombo;
    QLineEdit    *m_histFilterEdit;
};

#endif // MAINWINDOW_H
