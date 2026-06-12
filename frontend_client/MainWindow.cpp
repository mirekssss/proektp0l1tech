#include "MainWindow.h"
#include "TcpClient.h"
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    connect(&TcpClient::instance(), &TcpClient::connectionChanged,
            this, &MainWindow::onConnectionChanged);
}

MainWindow::~MainWindow()
{
    TcpClient::instance().disconnectFromServer();
}

void MainWindow::setupUI()
{
    QWidget     *container  = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(container);

    // ── Подключение ───────────────────────────────────────────────────────────
    m_connectButton = new QPushButton("Подключиться", this);
    m_statusLabel   = new QLabel("Статус: Отключён", this);
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(m_connectButton);
    topLayout->addWidget(m_statusLabel);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    // ── Регистрация ───────────────────────────────────────────────────────────
    QGroupBox *regGroup = new QGroupBox("Регистрация", this);
    m_regLoginEdit      = new QLineEdit(this);
    m_regPassEdit       = new QLineEdit(this);
    m_regPassEdit->setEchoMode(QLineEdit::Password);
    m_regButton         = new QPushButton("Зарегистрироваться", this);
    m_regResultLabel    = new QLabel(this);

    QGridLayout *regLayout = new QGridLayout;
    regLayout->addWidget(new QLabel("Логин:"),  0, 0);
    regLayout->addWidget(m_regLoginEdit,         0, 1);
    regLayout->addWidget(new QLabel("Пароль:"), 1, 0);
    regLayout->addWidget(m_regPassEdit,          1, 1);
    regLayout->addWidget(m_regButton,            2, 0, 1, 2);
    regLayout->addWidget(m_regResultLabel,       3, 0, 1, 2);
    regGroup->setLayout(regLayout);
    mainLayout->addWidget(regGroup);

    // ── Авторизация ───────────────────────────────────────────────────────────
    QGroupBox *logGroup = new QGroupBox("Авторизация", this);
    m_logLoginEdit      = new QLineEdit(this);
    m_logPassEdit       = new QLineEdit(this);
    m_logPassEdit->setEchoMode(QLineEdit::Password);
    m_logButton         = new QPushButton("Войти", this);
    m_logResultLabel    = new QLabel(this);

    QGridLayout *logLayout = new QGridLayout;
    logLayout->addWidget(new QLabel("Логин:"),  0, 0);
    logLayout->addWidget(m_logLoginEdit,         0, 1);
    logLayout->addWidget(new QLabel("Пароль:"), 1, 0);
    logLayout->addWidget(m_logPassEdit,          1, 1);
    logLayout->addWidget(m_logButton,            2, 0, 1, 2);
    logLayout->addWidget(m_logResultLabel,       3, 0, 1, 2);
    logGroup->setLayout(logLayout);
    mainLayout->addWidget(logGroup);

    // ── Панель администратора (скрыта по умолчанию) ───────────────────────────
    m_adminGroup         = new QGroupBox("Администратор", this);
    m_adminUsersTable    = new QTableWidget(0, 2, this);
    m_adminUsersTable->setHorizontalHeaderLabels({"Логин", "Роль"});
    m_adminUsersTable->horizontalHeader()->setStretchLastSection(true);
    m_adminUsersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_adminUsersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_adminUsersTable->setSortingEnabled(true);
    m_adminUsersTable->setMinimumHeight(120);
    m_adminRefreshButton = new QPushButton("Обновить список", this);
    m_adminDeleteEdit    = new QLineEdit(this);
    m_adminDeleteEdit->setPlaceholderText("Логин для удаления");
    m_adminDeleteButton  = new QPushButton("Удалить пользователя", this);
    m_adminResultLabel   = new QLabel(this);

    QVBoxLayout *adminLayout = new QVBoxLayout;
    adminLayout->addWidget(m_adminRefreshButton);
    adminLayout->addWidget(m_adminUsersTable);
    QHBoxLayout *delLayout = new QHBoxLayout;
    delLayout->addWidget(m_adminDeleteEdit);
    delLayout->addWidget(m_adminDeleteButton);
    adminLayout->addLayout(delLayout);
    adminLayout->addWidget(m_adminResultLabel);
    m_adminGroup->setLayout(adminLayout);
    m_adminGroup->setVisible(false);
    mainLayout->addWidget(m_adminGroup);

    // ── Шифр Виженера ─────────────────────────────────────────────────────────
    QGroupBox *vigGroup  = new QGroupBox("Шифр Виженера", this);
    m_vigTextEdit        = new QLineEdit(this);
    m_vigKeyEdit         = new QLineEdit(this);
    m_vigEncButton       = new QPushButton("Зашифровать", this);
    m_vigDecButton       = new QPushButton("Расшифровать", this);
    m_vigResultLabel     = new QLabel(this);
    m_vigResultLabel->setWordWrap(true);
    QGridLayout *vigLayout = new QGridLayout;
    vigLayout->addWidget(new QLabel("Текст:"), 0, 0);
    vigLayout->addWidget(m_vigTextEdit,         0, 1, 1, 2);
    vigLayout->addWidget(new QLabel("Ключ:"),   1, 0);
    vigLayout->addWidget(m_vigKeyEdit,          1, 1, 1, 2);
    vigLayout->addWidget(m_vigEncButton,        2, 0);
    vigLayout->addWidget(m_vigDecButton,        2, 1);
    vigLayout->addWidget(m_vigResultLabel,      3, 0, 1, 3);
    vigGroup->setLayout(vigLayout);
    mainLayout->addWidget(vigGroup);

    // ── SHA-512 ───────────────────────────────────────────────────────────────
    QGroupBox *shaGroup  = new QGroupBox("SHA-512", this);
    m_shaInputEdit       = new QLineEdit(this);
    m_shaButton          = new QPushButton("Хэшировать", this);
    m_shaResultLabel     = new QLabel(this);
    m_shaResultLabel->setWordWrap(true);
    QGridLayout *shaLayout = new QGridLayout;
    shaLayout->addWidget(new QLabel("Ввод:"), 0, 0);
    shaLayout->addWidget(m_shaInputEdit,       0, 1);
    shaLayout->addWidget(m_shaButton,          1, 0, 1, 2);
    shaLayout->addWidget(m_shaResultLabel,     2, 0, 1, 2);
    shaGroup->setLayout(shaLayout);
    mainLayout->addWidget(shaGroup);

    // ── Метод деления пополам ─────────────────────────────────────────────────
    QGroupBox *bisectGroup  = new QGroupBox("Метод деления пополам (sqrt)", this);
    m_bisectInputEdit       = new QLineEdit(this);
    m_bisectButton          = new QPushButton("Вычислить", this);
    m_bisectResultLabel     = new QLabel(this);
    QGridLayout *bisectLayout = new QGridLayout;
    bisectLayout->addWidget(new QLabel("Число:"), 0, 0);
    bisectLayout->addWidget(m_bisectInputEdit,     0, 1);
    bisectLayout->addWidget(m_bisectButton,        1, 0, 1, 2);
    bisectLayout->addWidget(m_bisectResultLabel,   2, 0, 1, 2);
    bisectGroup->setLayout(bisectLayout);
    mainLayout->addWidget(bisectGroup);

    // ── Кратчайший путь в графе ───────────────────────────────────────────────
    QGroupBox *graphGroup  = new QGroupBox("Кратчайший путь в графе (вершины 1–7)", this);
    m_graphSEdit           = new QLineEdit(this);
    m_graphTEdit           = new QLineEdit(this);
    m_graphButton          = new QPushButton("Найти путь", this);
    m_graphResultLabel     = new QLabel(this);
    QGridLayout *graphLayout = new QGridLayout;
    graphLayout->addWidget(new QLabel("Откуда:"), 0, 0);
    graphLayout->addWidget(m_graphSEdit,           0, 1);
    graphLayout->addWidget(new QLabel("Куда:"),    1, 0);
    graphLayout->addWidget(m_graphTEdit,           1, 1);
    graphLayout->addWidget(m_graphButton,          2, 0, 1, 2);
    graphLayout->addWidget(m_graphResultLabel,     3, 0, 1, 2);
    graphGroup->setLayout(graphLayout);
    mainLayout->addWidget(graphGroup);

    // ── История запросов ──────────────────────────────────────────────────────
    QGroupBox *histGroup = new QGroupBox("История запросов", this);
    m_historyTable = new QTableWidget(0, 3, this);
    m_historyTable->setHorizontalHeaderLabels({"Операция", "Ввод", "Результат"});
    m_historyTable->horizontalHeader()->setStretchLastSection(true);
    m_historyTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_historyTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_historyTable->setSortingEnabled(true);
    m_historyTable->setMinimumHeight(160);

    // Фильтр по столбцу
    m_histFilterCombo = new QComboBox(this);
    m_histFilterCombo->addItems({"Операция", "Ввод", "Результат"});
    m_histFilterEdit  = new QLineEdit(this);
    m_histFilterEdit->setPlaceholderText("Фильтр...");
    QHBoxLayout *filterLayout = new QHBoxLayout;
    filterLayout->addWidget(new QLabel("Фильтр:"));
    filterLayout->addWidget(m_histFilterCombo);
    filterLayout->addWidget(m_histFilterEdit, 1);

    QVBoxLayout *histLayout = new QVBoxLayout;
    histLayout->addLayout(filterLayout);
    histLayout->addWidget(m_historyTable);
    histGroup->setLayout(histLayout);
    mainLayout->addWidget(histGroup);

    mainLayout->addStretch();

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setWidget(container);
    setCentralWidget(scroll);
    setWindowTitle("Клиент — Группа 251-353");
    resize(640, 900);

    connect(m_connectButton,    &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(m_regButton,        &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    connect(m_logButton,        &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(m_adminRefreshButton,&QPushButton::clicked,this, &MainWindow::onAdminRefreshClicked);
    connect(m_adminDeleteButton, &QPushButton::clicked,this, &MainWindow::onAdminDeleteClicked);
    connect(m_vigEncButton,     &QPushButton::clicked, this, &MainWindow::onVigenereEncryptClicked);
    connect(m_vigDecButton,     &QPushButton::clicked, this, &MainWindow::onVigenereDecryptClicked);
    connect(m_shaButton,        &QPushButton::clicked, this, &MainWindow::onSHA512Clicked);
    connect(m_bisectButton,     &QPushButton::clicked, this, &MainWindow::onBisectClicked);
    connect(m_graphButton,      &QPushButton::clicked, this, &MainWindow::onGraphClicked);
    connect(m_histFilterEdit,   &QLineEdit::textChanged, this, &MainWindow::onHistFilterChanged);
    connect(m_histFilterCombo,  QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onHistFilterChanged);
}

// ─── Вспомогательные методы ───────────────────────────────────────────────────

void MainWindow::addHistoryRow(const QString &op, const QString &input, const QString &result)
{
    m_historyTable->setSortingEnabled(false);
    int row = m_historyTable->rowCount();
    m_historyTable->insertRow(row);
    m_historyTable->setItem(row, 0, new QTableWidgetItem(op));
    m_historyTable->setItem(row, 1, new QTableWidgetItem(input));
    m_historyTable->setItem(row, 2, new QTableWidgetItem(result));
    m_historyTable->setSortingEnabled(true);
    m_historyTable->scrollToBottom();
    onHistFilterChanged();
}

void MainWindow::onHistFilterChanged()
{
    const QString filter = m_histFilterEdit->text().toLower();
    const int     col    = m_histFilterCombo->currentIndex();
    for (int row = 0; row < m_historyTable->rowCount(); ++row) {
        QTableWidgetItem *item = m_historyTable->item(row, col);
        bool match = filter.isEmpty() ||
                     (item && item->text().toLower().contains(filter));
        m_historyTable->setRowHidden(row, !match);
    }
}

void MainWindow::setAdminPanelVisible(bool visible)
{
    m_adminGroup->setVisible(visible);
}

void MainWindow::updateLoginStatus(const QString &login, const QString &role)
{
    if (login.isEmpty()) {
        m_logResultLabel->setText("Не авторизован");
        m_logButton->setText("Войти");
        m_currentRole.clear();
        setAdminPanelVisible(false);
    } else {
        m_logResultLabel->setText(QString("Вошёл: %1  [%2]").arg(login).arg(role));
        m_logButton->setText("Выйти");
        m_currentRole = role;
        setAdminPanelVisible(role == "admin");
    }
}

// ─── Слоты ────────────────────────────────────────────────────────────────────

void MainWindow::onConnectionChanged(bool connected)
{
    m_statusLabel->setText(connected ? "Статус: Подключён" : "Статус: Отключён");
    m_connectButton->setText(connected ? "Отключиться" : "Подключиться");
    if (!connected) updateLoginStatus("", "");
}

void MainWindow::onConnectClicked()
{
    TcpClient &tc = TcpClient::instance();
    if (tc.isConnected()) {
        tc.disconnectFromServer();
    } else if (!tc.connectToServer()) {
        QMessageBox::warning(this, "Подключение", "Не удалось подключиться к серверу");
    }
}

void MainWindow::onRegisterClicked()
{
    QString login = m_regLoginEdit->text().trimmed();
    QString pass  = m_regPassEdit->text();
    if (login.isEmpty() || pass.isEmpty()) {
        m_regResultLabel->setText("Введите логин и пароль");
        return;
    }
    QString result = TcpClient::instance().sendCommand("reg&" + login + "," + pass);
    m_regResultLabel->setText(result);
    addHistoryRow("Регистрация", login, result);
}

void MainWindow::onLoginClicked()
{
    TcpClient &tc = TcpClient::instance();

    // Выход
    if (!m_currentRole.isEmpty()) {
        tc.sendCommand("logout");
        updateLoginStatus("", "");
        return;
    }

    QString login = m_logLoginEdit->text().trimmed();
    QString pass  = m_logPassEdit->text();
    if (login.isEmpty() || pass.isEmpty()) {
        m_logResultLabel->setText("Введите логин и пароль");
        return;
    }

    QString result = tc.sendCommand("auth&" + login + "," + pass);
    if (result.startsWith("OK:")) {
        QString role = result.mid(3);
        updateLoginStatus(login, role);
        addHistoryRow("Авторизация", login, "OK [" + role + "]");
    } else {
        m_logResultLabel->setText(result);
        addHistoryRow("Авторизация", login, result);
    }
}

void MainWindow::onAdminRefreshClicked()
{
    QString response = TcpClient::instance().sendCommand("admin_users");
    m_adminUsersTable->setRowCount(0);

    if (response.startsWith("ERROR") || response == "EMPTY") {
        m_adminResultLabel->setText(response);
        return;
    }

    for (const QString &entry : response.split('|', Qt::SkipEmptyParts)) {
        int sep = entry.indexOf(':');
        QString uLogin = entry.left(sep);
        QString uRole  = entry.mid(sep + 1);
        int row = m_adminUsersTable->rowCount();
        m_adminUsersTable->insertRow(row);
        m_adminUsersTable->setItem(row, 0, new QTableWidgetItem(uLogin));
        m_adminUsersTable->setItem(row, 1, new QTableWidgetItem(uRole));
    }
    m_adminResultLabel->setText(QString("Пользователей: %1").arg(m_adminUsersTable->rowCount()));
}

void MainWindow::onAdminDeleteClicked()
{
    QString login = m_adminDeleteEdit->text().trimmed();
    if (login.isEmpty()) { m_adminResultLabel->setText("Введите логин"); return; }
    QString result = TcpClient::instance().sendCommand("admin_delete&" + login);
    m_adminResultLabel->setText(result);
    if (result.startsWith("OK")) {
        m_adminDeleteEdit->clear();
        onAdminRefreshClicked();
        addHistoryRow("Удаление юзера", login, result);
    }
}

void MainWindow::onVigenereEncryptClicked()
{
    QString text = m_vigTextEdit->text();
    QString key  = m_vigKeyEdit->text().trimmed();
    if (text.isEmpty() || key.isEmpty()) { m_vigResultLabel->setText("Введите текст и ключ"); return; }
    QString result = TcpClient::instance().sendCommand("vigenere_enc&" + text + "," + key);
    m_vigResultLabel->setText(result);
    addHistoryRow("Виженер: шифр", text + " / " + key, result);
}

void MainWindow::onVigenereDecryptClicked()
{
    QString text = m_vigTextEdit->text();
    QString key  = m_vigKeyEdit->text().trimmed();
    if (text.isEmpty() || key.isEmpty()) { m_vigResultLabel->setText("Введите текст и ключ"); return; }
    QString result = TcpClient::instance().sendCommand("vigenere_dec&" + text + "," + key);
    m_vigResultLabel->setText(result);
    addHistoryRow("Виженер: расшифр", text + " / " + key, result);
}

void MainWindow::onSHA512Clicked()
{
    QString text = m_shaInputEdit->text();
    if (text.isEmpty()) { m_shaResultLabel->setText("Введите текст"); return; }
    QString hash = TcpClient::instance().sendCommand("sha512&" + text);
    m_shaResultLabel->setText(hash.size() == 128
        ? hash.left(64) + "\n" + hash.right(64) : hash);
    addHistoryRow("SHA-512", text, hash.left(16) + "…");
}

void MainWindow::onBisectClicked()
{
    QString val = m_bisectInputEdit->text().trimmed();
    if (val.isEmpty()) { m_bisectResultLabel->setText("Введите число"); return; }
    QString result = TcpClient::instance().sendCommand("bisect&" + val);
    m_bisectResultLabel->setText(result);
    addHistoryRow("Бисекция", val, result);
}

void MainWindow::onGraphClicked()
{
    QString s = m_graphSEdit->text().trimmed();
    QString t = m_graphTEdit->text().trimmed();
    if (s.isEmpty() || t.isEmpty()) { m_graphResultLabel->setText("Введите вершины"); return; }
    QString result = TcpClient::instance().sendCommand("graph&" + s + "," + t);
    m_graphResultLabel->setText(result);
    addHistoryRow("Граф", s + " → " + t, result);
}
