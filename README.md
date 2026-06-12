# Клиент-серверное приложение с алгоритмами

**Группа:** 251-353 | **Специальность:** Информационная безопасность

## Описание

Учебный проект — клиент-серверное приложение на **C++17 / Qt 6**.  
Сервер принимает TCP-соединения и выполняет алгоритмы по запросу клиентов.  
Клиент предоставляет графический интерфейс (Qt Widgets) для работы с сервером.

### Реализованные алгоритмы

| Алгоритм | Команда |
|----------|---------|
| Шифр Виженера (шифрование/расшифровка) | `vigenere_enc` / `vigenere_dec` |
| SHA-512 хэширование | `sha512` |
| Метод деления пополам (√x) | `bisect` |
| Кратчайший путь в графе (BFS) | `graph` |

## Архитектура

```
┌─────────────────────────────────────────────────────┐
│                  frontend_client                     │
│                                                      │
│  MainWindow ──► TcpClient (синглтон)                 │
│     │               │  TCP :33333                    │
│  История (таблица)  │                                │
│  Фильтр / Сортировка│                                │
└─────────────────────┼───────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────┐
│                  backend_server                      │
│                                                      │
│  MyTcpServer ──► функции-обработчики                 │
│       │               │                              │
│  Сессии клиентов  DatabaseManager (синглтон)         │
│                        │                             │
│                   algorithms.cpp                     │
│            (Виженер, SHA-512, bisect, BFS)           │
└─────────────────────────────────────────────────────┘
```

## Структура репозитория

```
polytech/
├── backend_server/          # Сервер (Qt Core + Network + Sql)
│   ├── main_serv.cpp
│   ├── mytcpserver.h/.cpp
│   ├── databasemanager.h/.cpp
│   ├── functions_to_server.h/.cpp
│   └── algorithms.h/.cpp
├── frontend_client/         # Клиент (Qt Widgets + Network)
│   ├── main.cpp
│   ├── MainWindow.h/.cpp
│   └── TcpClient.h/.cpp
├── tests/                   # Unit-тесты (Qt Test)
│   ├── CMakeLists.txt
│   └── test_algorithms.cpp
├── docs/                    # Документация
│   ├── class_diagram.puml
│   ├── usecase.puml
│   ├── requirements.txt
│   ├── test_strategy.csv
│   └── test_cases.csv
├── Dockerfile               # Docker-образ сервера
├── Doxyfile                 # Конфигурация Doxygen
└── README.md
```

## Граф (7 вершин)

```
    1
   / \
  2   3
 / \ / \
4  5   6
 \ | /
   7
```
Рёбра: 1-2, 1-3, 2-4, 2-5, 3-5, 3-6, 4-7, 5-7, 6-7

## Сборка и запуск

### Требования
- MSYS2 с пакетом `mingw-w64-x86_64-qt6` (или Qt 6 SDK)
- CMake 3.16+, Ninja

### Сервер
```bash
cd backend_server
cmake -B build -G Ninja -DCMAKE_PREFIX_PATH="C:/msys64/mingw64"
cmake --build build
./build/main_server.exe
```

### Клиент
```bash
cd frontend_client
cmake -B build -G Ninja -DCMAKE_PREFIX_PATH="C:/msys64/mingw64"
cmake --build build
./build/frontend_client.exe
```

### Unit-тесты
```bash
cd tests
cmake -B build -G Ninja -DCMAKE_PREFIX_PATH="C:/msys64/mingw64"
cmake --build build
./build/test_algorithms.exe
```

### Docker (сервер)
```bash
docker build -t polytech-server .
docker run -p 33333:33333 polytech-server
```

## Роли пользователей

| Роль | Возможности |
|------|-------------|
| `user` | Регистрация, авторизация, все алгоритмы |
| `admin` | Всё выше + просмотр/удаление пользователей |

По умолчанию создаётся учётная запись **admin / admin**.

## Протокол

Формат команды: `action&payload\n`  
Пример: `vigenere_enc&HELLO,KEY`  
Ответ: `ENCRYPTED: RIJVS`

## Тестирование

Unit-тесты покрывают все 4 алгоритма (29 тест-кейсов, Qt Test).  
Стратегия тестирования и тест-кейсы: `docs/test_strategy.csv`, `docs/test_cases.csv`.  
Спецификация требований: `docs/requirements.txt`.
