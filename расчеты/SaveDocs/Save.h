#ifndef SAVE_H
#define SAVE_H

#include "HikeFoodListGenerator.h"
#include <string>
#include <fstream>

class Save
{
public:
    virtual ~Save() = 0;
};

class SaveToFile : public Save
{
public:
    SaveToFile();
    ~SaveToFile() override;

    // Сохранить в DOC (как было)
    void saveDataByDay(const HikeFoodListGenerator& mealList, const std::string& outputDir = "");
    void saveOverallData(const HikeFoodListGenerator& mealList, const std::string& outputDir = "");

    // Новые методы для PDF через LaTeX
    void saveDataByDayToPDF(const HikeFoodListGenerator& mealList, const std::string& outputDir = "");
    void saveOverallDataToPDF(const HikeFoodListGenerator& mealList, const std::string& outputDir = "");

    // Общий метод для генерации PDF
    void generatePDF(const std::string& texContent, const std::string& outputFileName);

private:
    std::string dataByDatFileName;
    std::string OverallDataFileName;

    // Вспомогательные методы для генерации LaTeX кода
    std::string generateLaTeXHeader();
    std::string generateLaTeXFooter();
    std::string escapeLaTeX(const std::string& text);
};

#endif // SAVE_H
