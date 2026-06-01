#include "HikeFoodListGenerator.h"
#include <iostream>
#include <string>
#include <ctime>
#include <unordered_map>
#include <cstdlib>
#include <cstring>
#include <exception>


unsigned HikeFoodListGenerator::dayCnt { 0 };

HikeFoodListGenerator::HikeFoodListGenerator(  unsigned hikeDays, unsigned peopleCnt, unsigned increaseCoeff, const std::string& dbPath ): hikeDays( hikeDays ), peopleCnt( peopleCnt ), increaseCoeff( increaseCoeff )
{
    // Инициализируем генератор случайных чисел
    srand(static_cast<unsigned>(time(nullptr)));

    // Инициализируем SQLite с переданным путем
    if (!initSQLite(dbPath)) {
        throw std::runtime_error("Не удалось инициализировать SQLite базу данных по пути: " + dbPath);
    }
}

HikeFoodListGenerator::~HikeFoodListGenerator()
{
    for(auto& day: mealListForDays){
        for(auto* meal: day.second)
            delete meal;
        day.second.clear();
    }

    mealListForDays.clear();

    overallMealList.clear();

    closeSQLite();
}

bool HikeFoodListGenerator::initSQLite(const std::string& dbPath)
{
    std::string actualPath = dbPath;

    if (actualPath.empty()) {
       // Пробуем относительный путь
       actualPath = "../database/hikeFoodList.db";
    }

    if(sqlite3_open(actualPath.c_str(), &sqlite_db) != SQLITE_OK){
        std::cerr << "НЕ могу открыть базу данных" << std::endl;

        // Пробуем альтернативный путь - текущая директория
        actualPath = "hikeFoodList.db";

        sqlite_db = nullptr;
        return false;
    }

    // Проверим все таблицы с их точными именами
    sqlite3_stmt* stmt;
    const char* check_query = "SELECT name, sql FROM sqlite_master WHERE type='table';";

    if(sqlite3_prepare_v2(sqlite_db, check_query, -1, &stmt, nullptr) == SQLITE_OK) {
        std::cout << "Существующие таблицы и их структура:" << std::endl;
        while(sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* name = sqlite3_column_text(stmt, 0);
            const unsigned char* sql = sqlite3_column_text(stmt, 1);
            std::cout << "Имя таблицы: '" << name << "'" << std::endl;
            std::cout << "SQL: " << sql << std::endl;
            std::cout << "------------------------" << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Не удалось получить список таблиц: " << sqlite3_errmsg(sqlite_db) << std::endl;
    }

    // Включаем поддержку внешних ключей
    sqlite3_exec(sqlite_db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
    return true;
}

void HikeFoodListGenerator::closeSQLite()
{
    if(sqlite_db){
        sqlite3_close(sqlite_db);
        sqlite_db = nullptr;
    }
}

void HikeFoodListGenerator::addMealToMealListForDaysSQLite()
{
    if(!sqlite_db){
            throw "SQLite база данных не инициализирована!";
        }

        while(dayCnt++ != hikeDays){
            // Основные приемы пищи - по одному объекту с несколькими ингредиентами
            auto breakfastMeals = MealFactorySQLite("Breakfast");
            if(!breakfastMeals.empty())
                mealListForDays[dayCnt].push_back(breakfastMeals[0]);
            else
                std::cout << "WARNING: Breakfast is EMPTY for day " << dayCnt << std::endl;

            auto lunchMeals = MealFactorySQLite("Lunch");
            if(!lunchMeals.empty())
                mealListForDays[dayCnt].push_back(lunchMeals[0]);
            else
                std::cout << "WARNING: Lunch is EMPTY for day " << dayCnt << std::endl;

            auto dinnerMeals = MealFactorySQLite("Dinner");
            if(!dinnerMeals.empty())
                mealListForDays[dayCnt].push_back(dinnerMeals[0]);
            else
                std::cout << "WARNING: Dinner is EMPTY for day " << dayCnt << std::endl;

            // Bread - несколько объектов, каждый с одним ингредиентом
            auto breadMeals = MealFactorySQLite("Bread");
            for(auto* meal : breadMeals) {
                mealListForDays[dayCnt].push_back(meal);
            }

            // WithBread - несколько объектов
            auto withBreadMeals = MealFactorySQLite("WithBread");
            for(auto* meal : withBreadMeals) {
                mealListForDays[dayCnt].push_back(meal);
            }

            // Sweet - несколько объектов
            auto sweetMeals = MealFactorySQLite("Sweet");
            for(auto* meal : sweetMeals) {
                mealListForDays[dayCnt].push_back(meal);
            }

            // Snack - несколько объектов
            auto snackMeals = MealFactorySQLite("Snack");
            for(auto* meal : snackMeals) {
                mealListForDays[dayCnt].push_back(meal);
            }
        }
}

const std::map<int, std::vector<Meal*>> HikeFoodListGenerator::getMealListForDays() const noexcept
{
    return mealListForDays;
}

std::unordered_map<std::string, int> HikeFoodListGenerator::getOverallMealList() const noexcept
{
    return overallMealList;
}

unsigned HikeFoodListGenerator::getPeopleCnt() const noexcept
{
    return peopleCnt;
}
unsigned HikeFoodListGenerator::getIncreasedCoeff() const noexcept
{
    return increaseCoeff;
}

unsigned HikeFoodListGenerator::getDayCnt() const noexcept
{
    return hikeDays;
}

std::vector<Meal*> HikeFoodListGenerator::MealFactorySQLite( const char* tableName )
{
    std::vector<Meal*> meals;

    int mealCntInTable = getMealCntInTableSQLite(tableName);
    int neededCnt = getRecordCnt(tableName);

    std::cout << "MealFactorySQLite for " << tableName
                  << ": total=" << mealCntInTable
                  << ", needed=" << neededCnt << std::endl;

    if (mealCntInTable <= 0) {
            std::cerr << "ERROR: Table " << tableName << " is empty!" << std::endl;
            return meals;
        }

    // Выбираем нужное количество случайных ID
    std::vector<int> selectedIds;
    for(int i = 0; i < neededCnt; i++) {
        int id = getRandomCntRecord(1, mealCntInTable);
        selectedIds.push_back(id);
        std::cout << "  Selected ID: " << id << std::endl;
    }

    // Для основных приемов пищи (Breakfast, Lunch, Dinner) - создаем ОДИН объект
    if(strcmp(tableName, "Breakfast") == 0 ||
       strcmp(tableName, "Lunch") == 0 ||
       strcmp(tableName, "Dinner") == 0)
    {
        // Берем первый ID для создания объекта
        int id = selectedIds[0];
        std::cout << "  Fetching row for ID: " << id << std::endl;
        auto row = getRowMealFromTableSQLite(getWhereIdQuery(tableName, id).c_str());
        if(row.empty()) {
                    std::cerr << "  ERROR: Row is empty for ID " << id << std::endl;
                    return meals;
                }
        std::cout << "  Row size: " << row.size() << std::endl;
        std::cout << "  Name: " << row[1] << std::endl;

        if(row.empty()) return meals;

        Meal* meal = nullptr;
        if(strcmp(tableName, "Breakfast") == 0)
            meal = new Breakfast(row[1].c_str());
        else if(strcmp(tableName, "Lunch") == 0)
            meal = new Lunch(row[1].c_str());
        else if(strcmp(tableName, "Dinner") == 0)
            meal = new Dinner(row[1].c_str());

        if(!meal) {
                    std::cerr << "  ERROR: Failed to create meal object" << std::endl;
                    return meals;
                }

        // Добавляем ВСЕ ингредиенты из этой записи
        int ingredientCount = 0;
        for(size_t i = 2; i < row.size(); i++) {
            if(row[i].empty()){
                std::cout << "  Column " << i << " is empty, skipping" << std::endl;
                continue;
             }

            int productId = std::atoi(row[i].c_str());
            std::cout << "  Column " << i << ": productId = " << productId << std::endl;
            if(productId <= 0){
                std::cout << "  productId <= 0, skipping" << std::endl;
                continue;
            }

            auto curr_row = getRowMealFromTableSQLite(getWhereIdQuery("CalorieTable", productId).c_str());
            if(curr_row.size() > 5) {
                int weight = std::atoi(curr_row[3].c_str());
                std::cout << "    Adding ingredient: " << curr_row[5] << " weight: " << weight << std::endl;
                meal->addToResourse(curr_row[5].c_str(), weight);
                addMealToOverallMeal(curr_row[5].c_str(), weight);
                ingredientCount++;
            }
        }

        std::cout << "  Total ingredients added: " << ingredientCount << std::endl;

        if(ingredientCount > 0) {
                    meals.push_back(meal);
                    std::cout << "  Meal added successfully" << std::endl;
                } else {
                    std::cerr << "  WARNING: No ingredients added, deleting meal" << std::endl;
                    delete meal;
                }
    }
    else {
        // Для Bread, WithBread, Sweet, Snack - создаем ПО ОДНОМУ объекту на каждый ID
        for(int id : selectedIds) {
            auto row = getRowMealFromTableSQLite(getWhereIdQuery(tableName, id).c_str());
            if(row.empty()) continue;

            Meal* meal = nullptr;

            if(strcmp(tableName, "Bread") == 0)
                meal = new Bread(row[1].c_str());
            else if(strcmp(tableName, "WithBread") == 0)
                meal = new PatesForBread(row[1].c_str());
            else if(strcmp(tableName, "Sweet") == 0)
                meal = new Sweet(row[1].c_str());
            else if(strcmp(tableName, "Snack") == 0)
                meal = new Snack(row[1].c_str());

            if(meal) {
                // Добавляем один ингредиент
                int productId = std::atoi(row[2].c_str());
                auto curr_row = getRowMealFromTableSQLite(getWhereIdQuery("CalorieTable", productId).c_str());

                if(curr_row.size() > 5) {
                    int weight = std::atoi(curr_row[3].c_str());
                    meal->addToResourse(curr_row[5].c_str(), weight);
                    addMealToOverallMeal(curr_row[5].c_str(), weight);
                }

                meals.push_back(meal);
            }
        }
    }

    return meals;
}

int HikeFoodListGenerator::getRecordCnt( const char* tableName )
{
    if( strcmp(tableName,"Bread") == 0 || strcmp(tableName, "WithBread") == 0 || strcmp(tableName, "Sweet") == 0 )
        return 3;
    else if( strcmp(tableName, "Snack") == 0 )
        return 2;
    return 1;
}

int HikeFoodListGenerator::getMealCntInTableSQLite( const char* tableName ) const noexcept
{
    if(!sqlite_db) {
        std::cerr << "SQLite база не инициализирована!" << std::endl;
        return 0;
    }

    std::string query = "SELECT COUNT(*) FROM \"";
    query += tableName;
    query += "\";";

    sqlite3_stmt* stmt;
    int cnt = 0;

    if(sqlite3_prepare_v2(sqlite_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK){
        if(sqlite3_step(stmt) == SQLITE_ROW)
            cnt = sqlite3_column_int(stmt, 0);

        sqlite3_finalize(stmt);
    }

    return cnt;
}

std::vector<std::string> HikeFoodListGenerator::getRowMealFromTableSQLite( const char* query ) const noexcept
{
    std::vector<std::string> result;
    if (!sqlite_db) return result;

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(sqlite_db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        return result;
    }

    int cols = sqlite3_column_count(stmt);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        for (int i = 0; i < cols; i++) {
            const unsigned char* text = sqlite3_column_text(stmt, i);
            result.push_back(text ? reinterpret_cast<const char*>(text) : "");
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

std::string HikeFoodListGenerator::queryForGetGenerator( const char* tableName, int mealCntInTable, int cnt )
{
    std::unordered_map<int, bool> records;
    while(records.size() != static_cast<size_t>(cnt)){
        int id = getRandomCntRecord(1, mealCntInTable);
        auto iter = records.find( id );
        if( iter != records.end() )
            continue;
        records[id] = true;
    }

    return getWhereIdQuery( tableName, records );
}

int HikeFoodListGenerator::getRandomCntRecord( int min, int max )
{
    return rand() % (max - min + 1) + min;
}

std::string HikeFoodListGenerator::getWhereIdQuery( const char* tableName, int id )
{
    std::string query = "SELECT * FROM \"";
    query += tableName;
    query += "\" WHERE id = ";

    query += std::to_string(id);
    query += ";";

    return query;
}

std::string HikeFoodListGenerator::getWhereIdQuery( const char* tableName, const std::unordered_map<int, bool>& idRecords )
{
    std::string query = "SELECT * FROM \"";
    query += tableName;
    query += "\" WHERE id IN (";

    unsigned i = 0;
    for( auto iter = idRecords.begin(); iter != idRecords.end(); ++iter, i++ ){
        query += std::to_string((*iter).first);
        if( i != idRecords.size() - 1 )
            query += ", ";
    }

    query += ");";

    return query;
}

void HikeFoodListGenerator::addMealToOverallMeal( const char* name, int weight)
{
    auto elem = overallMealList.find( name );

    if( elem != overallMealList.end() )
        overallMealList[ name ] += weight;
    else
        overallMealList[ name ] = weight;
}






