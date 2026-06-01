#include "Save.h"
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <typeinfo>
#include <QString>
#include <QDebug>
#include <QProcess>

std::string makeFullPath(const std::string& dir, const std::string& filename, const std::string& extension)
{
    if (dir.empty())
        return filename + extension;

    return dir + "/" + filename + extension;
}

Save::~Save()
{}

SaveToFile::SaveToFile()
{
    dataByDatFileName = "MealByDay";
    OverallDataFileName = "OverallData";
}

SaveToFile::~SaveToFile()
{}

// Вспомогательный метод для экранирования специальных символов LaTeX
std::string SaveToFile::escapeLaTeX(const std::string& text)
{
    std::string result;
    for (char c : text) {
        switch (c) {
            case '&': result += "\\&"; break;
            case '%': result += "\\%"; break;
            case '$': result += "\\$"; break;
            case '#': result += "\\#"; break;
            case '_': result += "\\_"; break;
            case '{': result += "\\{"; break;
            case '}': result += "\\}"; break;
            case '~': result += "\\textasciitilde{}"; break;
            case '^': result += "\\textasciicircum{}"; break;
            default: result += c; break;
        }
    }
    return result;
}

// Заголовок LaTeX документа
std::string SaveToFile::generateLaTeXHeader()
{
    return R"(
\documentclass[a4paper,12pt]{article}
\usepackage[utf8]{inputenc}
\usepackage[T2A]{fontenc}
\usepackage[russian]{babel}
\usepackage{geometry}
\usepackage{longtable}
\usepackage{booktabs}
\usepackage{hyperref}

\geometry{
    a4paper,
    total={170mm,257mm},
    left=20mm,
    top=20mm,
}

\begin{document}
\newpage
)";
}

// Подвал LaTeX документа
std::string SaveToFile::generateLaTeXFooter()
{
    return "\n\\end{document}\n";
}

// Метод для генерации PDF из LaTeX кода
void SaveToFile::generatePDF(const std::string& texContent, const std::string& outputFileName)
{
    // Создаем временный .tex файл
    std::string texFileName = outputFileName + ".tex";
    std::ofstream texFile(texFileName);

    if (!texFile) {
        std::cerr << "Ошибка: не удалось создать файл " << texFileName << std::endl;
        return;
    }

    texFile << texContent;
    texFile.close();

    // Получаем директорию и имя файла
    std::string dir = outputFileName.substr(0, outputFileName.find_last_of("/"));
    std::string baseName = outputFileName.substr(outputFileName.find_last_of("/") + 1);

    // Используем QProcess для выполнения pdflatex
    QProcess process;
    process.setWorkingDirectory(QString::fromStdString(dir));

    QStringList arguments;
    arguments << "-interaction=nonstopmode" << QString::fromStdString(baseName + ".tex");

    process.start("pdflatex", arguments);

    if (!process.waitForFinished(30000)) { // 30 секунд таймаут
        std::cerr << "Ошибка при создании PDF: timeout" << std::endl;
        return;
    }

    if (process.exitCode() == 0) {
        std::cout << "PDF успешно создан: " << outputFileName << std::endl;

        // Удаляем временные файлы LaTeX
        std::system(("rm -f \"" + texFileName + "\" \"" + dir + "/" + baseName + ".aux\" \"" + dir + "/" + baseName + ".log\"").c_str());
    } else {
        std::cerr << "Ошибка при создании PDF. Убедитесь, что установлен pdflatex." << std::endl;
    }
}

// Сохранение по дням в PDF
void SaveToFile::saveDataByDayToPDF(const HikeFoodListGenerator& mealList, const std::string& outputDir)
{
    std::string filePath = makeFullPath(outputDir, dataByDatFileName, ""); // .pdf
    qDebug() << "Saving PDF to:" << QString::fromStdString(filePath);

    std::string texContent;

    texContent += generateLaTeXHeader();

    // Информация о походе
    texContent += "\\section*{Информация о походе}\n";
    texContent += "\\begin{itemize}\n";
    texContent += "\\item Количество дней: " + std::to_string(mealList.getDayCnt()) + "\n";
    texContent += "\\item Количество человек: " + std::to_string(mealList.getPeopleCnt()) + "\n";
    texContent += "\\item Повышающий коэффициент: " + std::to_string(mealList.getIncreasedCoeff()) + "\n";
    texContent += "\\end{itemize}\n\n";
    auto list = mealList.getMealListForDays();

    // Для каждого дня создаем таблицу
    for (const auto& day : list) {
        texContent += "\\section*{" + std::to_string(day.first) + " день}\n\n";

        texContent += "\\begin{longtable}{|p{0.7\\linewidth}|r|}\n";
        texContent += "\\hline\n";
        texContent += "\\textbf{Продукт} & \\textbf{Вес (г)} \\\\\n";
        texContent += "\\hline\n";
        texContent += "\\endfirsthead\n\n";

        texContent += "\\hline\n";
        texContent += "\\textbf{Продукт} & \\textbf{Вес (г)} \\\\\n";
        texContent += "\\hline\n";
        texContent += "\\endhead\n\n";

        // Разделяем объекты по типам
        std::vector<const Meal*> mainMeals;      // Breakfast, Lunch, Dinner
        std::vector<const Meal*> breadMeals;     // Bread
        std::vector<const Meal*> withBreadMeals; // WithBread
        std::vector<const Meal*> sweetMeals;     // Sweet
        std::vector<const Meal*> snackMeals;     // Snack

        for (const auto* meal : day.second) {
            std::string mealName = meal->getMealName();

            // Определяем тип по названию или можно использовать dynamic_cast
            if (dynamic_cast<const Bread*>(meal)) {
                breadMeals.push_back(meal);
            }
            else if (dynamic_cast<const PatesForBread*>(meal)) {
                withBreadMeals.push_back(meal);
            }
            else if (dynamic_cast<const Sweet*>(meal)) {
                sweetMeals.push_back(meal);
            }
            else if (dynamic_cast<const Snack*>(meal)) {
                snackMeals.push_back(meal);
            }
            else {
                mainMeals.push_back(meal);
            }
        }

        // Сначала выводим основные приемы пищи
        for (const auto* meal : mainMeals) {
            texContent += "\\multicolumn{2}{|c|}{\\textbf{"
                        + escapeLaTeX(meal->getMealName()) + "}} \\\\\n";
            texContent += "\\hline\n";

            auto ingredients = meal->getResourse();
            for (const auto& ingredient : ingredients) {
                texContent += escapeLaTeX(ingredient.first) + " & "
                            + std::to_string(ingredient.second * mealList.getPeopleCnt()) + " \\\\\n";
                texContent += "\\hline\n";
            }
        }

        // Выводим сгруппированный хлеб
        if (!breadMeals.empty()) {
            texContent += "\\multicolumn{2}{|c|}{\\textbf{Хлеб}} \\\\\n";
            texContent += "\\hline\n";

            for (const auto* meal : breadMeals) {
                auto ingredients = meal->getResourse();
                for (const auto& ingredient : ingredients) {
                    texContent += escapeLaTeX(ingredient.first) + " & "
                                + std::to_string(ingredient.second * mealList.getPeopleCnt()) + " \\\\\n";
                    texContent += "\\hline\n";
                }
            }
        }

        // Выводим сгруппированные намазки
        if (!withBreadMeals.empty()) {
            texContent += "\\multicolumn{2}{|c|}{\\textbf{Помазки на хлеб}} \\\\\n";
            texContent += "\\hline\n";

            for (const auto* meal : withBreadMeals) {
                auto ingredients = meal->getResourse();
                for (const auto& ingredient : ingredients) {
                    texContent += escapeLaTeX(ingredient.first) + " & "
                                + std::to_string(ingredient.second * mealList.getPeopleCnt()) + " \\\\\n";
                    texContent += "\\hline\n";
                }
            }
        }

        // Выводим сгруппированные сладости
        if (!sweetMeals.empty()) {
            texContent += "\\multicolumn{2}{|c|}{\\textbf{Сладкое}} \\\\\n";
            texContent += "\\hline\n";

            for (const auto* meal : sweetMeals) {
                auto ingredients = meal->getResourse();
                for (const auto& ingredient : ingredients) {
                    texContent += escapeLaTeX(ingredient.first) + " & "
                                + std::to_string(ingredient.second * mealList.getPeopleCnt()) + " \\\\\n";
                    texContent += "\\hline\n";
                }
            }
        }

        // Выводим сгруппированные перекусы
        if (!snackMeals.empty()) {
            texContent += "\\multicolumn{2}{|c|}{\\textbf{Перекусы}} \\\\\n";
            texContent += "\\hline\n";

            for (const auto* meal : snackMeals) {
                auto ingredients = meal->getResourse();
                for (const auto& ingredient : ingredients) {
                    texContent += escapeLaTeX(ingredient.first) + " & "
                                + std::to_string(ingredient.second * mealList.getPeopleCnt()) + " \\\\\n";
                    texContent += "\\hline\n";
                }
            }
        }

        texContent += "\\end{longtable}\n\n";
        texContent += "\\newpage\n";
    }

    texContent += generateLaTeXFooter();

    // Генерируем PDF
    generatePDF(texContent, filePath);
}

// Сохранение общего списка в PDF
void SaveToFile::saveOverallDataToPDF(const HikeFoodListGenerator& mealList, const std::string& outputDir)
{
    std::string filePath = makeFullPath(outputDir, OverallDataFileName, ""); // .pdf
    qDebug() << "Saving PDF to:" << QString::fromStdString(filePath);

    std::string texContent;

    texContent += generateLaTeXHeader();

    // Информация о походе
    texContent += "\\section*{Информация о походе}\n";
    texContent += "\\begin{itemize}\n";
    texContent += "\\item Количество дней: " + std::to_string(mealList.getDayCnt()) + "\n";
    texContent += "\\item Количество человек: " + std::to_string(mealList.getPeopleCnt()) + "\n";
    texContent += "\\item Повышающий коэффициент: " + std::to_string(mealList.getIncreasedCoeff()) + "\n";
    texContent += "\\end{itemize}\n\n";

    texContent += "\\section*{Общий список продуктов}\n\n";

    texContent += "\\begin{longtable}{|c|p{0.6\\linewidth}|r|}\n";
    texContent += "\\hline\n";
    texContent += "\\textbf{№} & \\textbf{Продукт} & \\textbf{Общий вес (г)} \\\\\n";
    texContent += "\\hline\n";
    texContent += "\\endfirsthead\n\n";

    texContent += "\\hline\n";
    texContent += "\\textbf{№} & \\textbf{Продукт} & \\textbf{Общий вес (г)} \\\\\n";
    texContent += "\\hline\n";
    texContent += "\\endhead\n\n";

    auto list = mealList.getOverallMealList();
    unsigned i = 0;

    for (const auto& item : list) {
        texContent += std::to_string(++i) + " & "
                    + escapeLaTeX(item.first) + " & "
                    + std::to_string(item.second * mealList.getPeopleCnt()) + " \\\\\n";
        texContent += "\\hline\n";
    }

    texContent += "\\end{longtable}\n";
    texContent += generateLaTeXFooter();

    // Генерируем PDF
    generatePDF(texContent, filePath);
}

// Оригинальные методы для DOC (оставляем для совместимости)
void SaveToFile::saveDataByDay(const HikeFoodListGenerator& mealList, const std::string& outputDir)
{
    std::string filePath = makeFullPath(outputDir, dataByDatFileName, ".doc");

    qDebug() << "Saving to:" << QString::fromStdString(filePath);

    std::ofstream file(filePath);

    if (!file) {
        std::cout << "Error: Unable to open file" << dataByDatFileName << "!" << std::endl;
        return;
    }

    auto list = mealList.getMealListForDays();

    file << "РАЦИОН ПИТАНИЯ ДЛЯ ПОХОДА\n";
    file << "КОЛИЧЕСТВО ДНЕЙ: " << mealList.getDayCnt() << std::endl;
    file << "КОЛИЧЕСТВО ЧЕЛОВЕК: " << mealList.getPeopleCnt() << std::endl;
    file << std::string(67, '-') << std::endl;
    file << std::left << std::setw(55) << "ПРОДУКТ"
         << std::left << std::setw(15) << "ВЕС (г)" << std::endl;
    file << std::string(67, '-') << std::endl;

    for (auto iter = list.begin(); iter != list.end(); ++iter) {
        file << (*iter).first << " день:\n";
        unsigned i = 0;
        for (const auto dayMealList : (*iter).second) {
            file << ++i << ". " << dayMealList->getMealName() << ":\n";
            auto ingredients = dayMealList->getResourse();
            for (const auto& ingredient : ingredients) {
                auto sz = ingredient.first.length() / 2;
                file << std::left << std::setw(50 + sz) << ingredient.first
                     << std::left << std::setw(15) << ingredient.second * mealList.getPeopleCnt() << std::endl;
            }
        }
        file << "\n";
        file << std::string(67, '-') << std::endl;
    }

    file.close();
}

void SaveToFile::saveOverallData(const HikeFoodListGenerator& mealList, const std::string& outputDir)
{
    std::string filePath = makeFullPath(outputDir, OverallDataFileName, ".doc");

    qDebug() << "Saving to:" << QString::fromStdString(filePath);

    std::ofstream file(filePath);

    if (!file) {
        std::cout << "Error: Unable to open file" << OverallDataFileName << "!" << std::endl;
        return;
    }

    auto list = mealList.getOverallMealList();
    unsigned i = 0;
    file << "ОБЩИЙ СПИСОК ПРОДУКТОВ" << std::endl;
    file << "КОЛИЧЕСТВО ДНЕЙ: " << mealList.getDayCnt() << std::endl;
    file << "КОЛИЧЕСТВО ЧЕЛОВЕК: " << mealList.getPeopleCnt() << std::endl;
    file << std::string(67, '-') << std::endl;

    file << std::left << std::setw(5) << "№"
         << std::left << std::setw(55) << "ПРОДУКТ"
         << std::left << std::setw(15) << "ОБЩИЙ ВЕС (г)" << std::endl;

    file << std::string(67, '-') << std::endl;

    for (auto iter = list.begin(); iter != list.end(); ++iter) {
        auto sz = (*iter).first.length() / 2;
        file << std::left << std::setw(4) << ++i
             << std::left << std::setw(50 + sz) << (*iter).first
             << std::left << std::setw(15) << (*iter).second * mealList.getPeopleCnt() << std::endl;
    }
    file.close();
}
