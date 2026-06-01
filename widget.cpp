#include "widget.h"
#include <QMessageBox>
#include <QDebug>
#include <QIntValidator>
#include <ctime>
#include <QFileDialog>
#include <QDir>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{   
    if(!connectToDatabase()){
        QMessageBox::warning(this, "Предупреждение", "Не удалось подключиться к базе данных.\n" "Путь к базе данных: " + getDatabasePath());
    }
    daysCntLabel = new QLabel("Введите количество дней");
    peopleCntLabel = new QLabel("Введите количество человек");
    increasedCoeffLabel = new QLabel("Введите повышающий коэффициент");

    daysCntLineEdit = new QLineEdit();
    daysCntLineEdit->setPlaceholderText("0");
    daysCntLineEdit->setValidator(new QIntValidator(1, 365, this));

    peopleCntLineEdit = new QLineEdit();
    peopleCntLineEdit->setPlaceholderText("0");
    peopleCntLineEdit->setValidator(new QIntValidator(1, 100, this));

    increasedCoeffLineEdit = new QLineEdit();
    increasedCoeffLineEdit->setPlaceholderText("1");
    increasedCoeffLineEdit->setValidator(new QIntValidator(1, 10, this));

    calculateBtn = new QPushButton("Рассчитать");
    saveBtn = new QPushButton("Сохранить");
    // Кнопка сохранения неактивна до расчета
    saveBtn->setEnabled(false);

    daysRowBox = new QHBoxLayout();
    daysRowBox->addWidget(daysCntLabel, 3);
    daysRowBox->addWidget(daysCntLineEdit, 1);

    peopleRowBox = new QHBoxLayout();
    peopleRowBox->addWidget(peopleCntLabel, 3);
    peopleRowBox->addWidget(peopleCntLineEdit, 1);

    coeffRowBox = new QHBoxLayout();
    coeffRowBox->addWidget(increasedCoeffLabel, 3);
    coeffRowBox->addWidget(increasedCoeffLineEdit, 1);

    btnsRowBox = new QHBoxLayout();
    btnsRowBox->addWidget(calculateBtn);
    btnsRowBox->addWidget(saveBtn);

    vbox = new QVBoxLayout();
    vbox->addLayout(daysRowBox);
    vbox->addLayout(peopleRowBox);
    vbox->addLayout(coeffRowBox);
    vbox->addLayout(btnsRowBox);

    setLayout(vbox);

    connect(calculateBtn, SIGNAL(clicked()), this, SLOT(calculate()));
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(save()));
}

Widget::~Widget()
{
    if(db.isOpen())
        db.close();
}

bool Widget::connectToDatabase()
{
    QString dbPath = getDatabasePath();
    qDebug() << "Попытка подключения к БД:" << dbPath;

    // Проверяем существование файла
    if (!QFile::exists(dbPath)) {
        qDebug() << "Файл БД не найден!";

        // Пробуем найти в папке сборки
        QString buildPath = QCoreApplication::applicationDirPath() + "/hikeFoodList.db";
        if (QFile::exists(buildPath)) {
            dbPath = buildPath;
            qDebug() << "Использую БД из папки сборки:" << dbPath;
        } else {
            return false;
        }
    }

    // Подключаемся к SQLite
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Ошибка открытия БД:" << db.lastError().text();
        return false;
    }

    qDebug() << "База данных успешно открыта!";
    return true;
}

void Widget::save()
{
    qDebug() << "Нажата кнопка save()";

    if (!calculated || !mealListSQLite) {
        QMessageBox::warning(this, "Предупреждение",
                           "Сначала выполните расчет!");
        return;
    }

    try {
        SaveToFile saveTofile;

        // Спрашиваем пользователя, в каком формате сохранить
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Формат сохранения",
                                     "Сохранить в PDF формате?\n"
                                     "(Нажмите Нет для сохранения в DOC)",
                                     QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // Спрашиваем папку для сохранения PDF
            QString saveDir = QFileDialog::getExistingDirectory(
                this,
                "Выберите папку для сохранения PDF",
                QDir::homePath()  // Начинаем с домашней папки
            );

            // Если пользователь нажал "Отмена"
            if (saveDir.isEmpty()) {
                qDebug() << "User cancelled PDF save";
                return;
            }

            qDebug() << "Saving PDF to:" << saveDir;

            // Сохраняем PDF в выбранную папку
            // ВАЖНО: Здесь нужно изменить методы saveDataByDayToPDF и saveOverallDataToPDF
            // чтобы они принимали путь. Если они не принимают путь, покажу как исправить ниже
            saveTofile.saveDataByDayToPDF(*mealListSQLite, saveDir.toStdString());
            saveTofile.saveOverallDataToPDF(*mealListSQLite, saveDir.toStdString());

            QMessageBox::information(this, "Успех",
                                   QString("Данные успешно сохранены в PDF!\n\n"
                                           "Файлы сохранены в папке:\n%1")
                                   .arg(saveDir));
        } else {
            // Спрашиваем папку для сохранения DOC
            QString saveDir = QFileDialog::getExistingDirectory(
                this,
                "Выберите папку для сохранения DOC",
                QDir::homePath()
            );

            if (saveDir.isEmpty()) {
                qDebug() << "User cancelled DOC save";
                return;
            }

            qDebug() << "Saving DOC to:" << saveDir;

            // Сохраняем DOC в выбранную папку
            saveTofile.saveDataByDay(*mealListSQLite, saveDir.toStdString());
            saveTofile.saveOverallData(*mealListSQLite, saveDir.toStdString());

            QMessageBox::information(this, "Успех",
                                   QString("Данные успешно сохранены в DOC!\n\n"
                                           "Файлы сохранены в папке:\n%1")
                                   .arg(saveDir));
        }

        qDebug() << "Файлы успешно сохранены";

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка сохранения",
                            QString("Произошла ошибка при сохранении:\n%1")
                            .arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Ошибка сохранения",
                            "Произошла неизвестная ошибка при сохранении!");
    }
}

void Widget::calculate()
{
    qDebug() << "Нажата кнопка calculate()";

    bool daysCntOk, peopleCntOk, increasedCoeffOk;

    // Получаем значения из полей ввода
    daysCnt = daysCntLineEdit->text().toInt(&daysCntOk);
    if(!daysCntOk || daysCnt < 0){
        QMessageBox::warning(this, "Ошибка ввода", "Количество дней должно быть целым числом > 0!");
        daysCntLineEdit->setFocus(); // Устанавливаем курсор в проблемное поле
        daysCntLineEdit->selectAll(); // Выделяем текст для исправления
        return;
    }

    peopleCnt = peopleCntLineEdit->text().toInt(&peopleCntOk);
    if(!peopleCntOk || peopleCnt < 0){
        QMessageBox::warning(this, "Ошибка ввода", "Количество человек должно быть целым числом > 0!");
        peopleCntLineEdit->setFocus(); // Устанавливаем курсор в проблемное поле
        peopleCntLineEdit->selectAll(); // Выделяем текст для исправления
        return;
    }

    increasedCoeff = increasedCoeffLineEdit->text().toInt(&increasedCoeffOk);
    if(!increasedCoeffOk || increasedCoeff < 1){
        QMessageBox::warning(this, "Ошибка ввода", "Коэффициент должен быть числом >= 1!");
        increasedCoeffLineEdit->setFocus(); // Устанавливаем курсор в проблемное поле
        increasedCoeffLineEdit->selectAll(); // Выделяем текст для исправления
        return;
    }

    qDebug() << "Получены значения:"
             << "дни: " << daysCnt
             << "люди: " << peopleCnt
             << "коэфф: " << increasedCoeff;

    try{
        HikeFoodListGenerator::resertDayCnt();

        mealListSQLite = std::make_unique<HikeFoodListGenerator>(daysCnt, peopleCnt, increasedCoeff, "/home/anastasia/VisualStudio/HikeMenuCalculatorGUI2/HikeMenuCalculatorGUI/database/hikeFoodList.db");
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        mealListSQLite->addMealToMealListForDaysSQLite();

        QMessageBox::information(this, "Успех", "Расчет успешно выполнен!\n"
                                 "Результаты можно сохранить в файл");
        saveBtn->setEnabled(true);
        calculated = true;
    }
    catch(const std::exception& e){
        QMessageBox::critical(this, "Ошибка расчета",
                                    QString("Произошла ошибка при расчете:\n%1").arg(e.what()));
    }
    catch(const char* msg){
        QMessageBox::critical(this, "Ошибка расчета",
                                    QString("Произошла ошибка при расчете:\n%1").arg(msg));
    }
    catch(...){
        QMessageBox::critical(this, "Ошибка расчета",
                                    "Произошла неизвестная ошибка при расчете!");
    }
}

void Widget::addToDB()
{
    qDebug() << "Нажата кнопка addToDB()";
}

