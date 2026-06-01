QT       += core gui sql
QT       += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Путь к базе данных (определяем макрос)
DB_PATH = $$PWD/database/hikeFoodList.db
DEFINES += DATABASE_PATH=\\\"$$DB_PATH\\\"

# Пути для поиска заголовочных файлов
INCLUDEPATH += $$PWD/расчеты
INCLUDEPATH += $$PWD/расчеты/HikeFoodListGenerator
INCLUDEPATH += $$PWD/расчеты/MealClass
INCLUDEPATH += $$PWD/расчеты/SaveDocs

SOURCES += \
    main.cpp \
    widget.cpp \
    расчеты/HikeFoodListGenerator/HikeFoodListGenerator.cpp \
    расчеты/MealClass/Meal.cpp \
    расчеты/SaveDocs/Save.cpp

HEADERS += \
    widget.h \
    расчеты/HikeFoodListGenerator/HikeFoodListGenerator.h \
    расчеты/MealClass/Meal.h \
    расчеты/SaveDocs/Save.h

# Подключаем SQLite библиотеку
unix:!macx {
    LIBS += -lsqlite3
    # Копируем базу данных в папку сборки
    QMAKE_POST_LINK += cp $$DB_PATH $$OUT_PWD/ $$escape_expand(\n\t)
}

# Для отладки - выводим путь к базе
message("Database path: $$DB_PATH")

RESOURCES += \
    resources.qrc
