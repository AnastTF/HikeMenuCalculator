#ifndef WIDGET_H
#define WIDGET_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QDir>
#include <memory>

// Классы для расчета рациона питания
#include "HikeFoodListGenerator.h"
#include "Meal.h"
#include "Save.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void save();
    void calculate();
    bool connectToDatabase();
    void addToDB();

private:
    QLabel* daysCntLabel;
    QLabel* peopleCntLabel;
    QLabel* increasedCoeffLabel;

    QLineEdit* daysCntLineEdit;
    QLineEdit* peopleCntLineEdit;
    QLineEdit* increasedCoeffLineEdit;

    QPushButton* saveBtn;
    QPushButton* calculateBtn;

    QVBoxLayout* vbox;
    QHBoxLayout* daysRowBox;
    QHBoxLayout* peopleRowBox;
    QHBoxLayout* coeffRowBox;
    QHBoxLayout* btnsRowBox;

    int daysCnt {0};
    int peopleCnt{0};
    int increasedCoeff{1};

    QSqlDatabase db;

    // Получить путь к базе данных
    QString getDatabasePath() const {
        return QString(DATABASE_PATH);
    }

    std::unique_ptr<HikeFoodListGenerator> mealListSQLite;
    bool calculated {false};
};
#endif // WIDGET_H
