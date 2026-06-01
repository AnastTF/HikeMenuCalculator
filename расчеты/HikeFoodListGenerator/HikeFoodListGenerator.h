#ifndef HIKEFOODLISTGENERATOR_H
#define HIKEFOODLISTGENERATOR_H

#include "Meal.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <sqlite3.h>
#include <string>

class HikeFoodListGenerator{
    std::map<int, std::vector<Meal*>>                   mealListForDays;
    std::unordered_map<std::string, int>                overallMealList;

    static unsigned                                     dayCnt;
    unsigned                                            hikeDays;
    unsigned                                            peopleCnt;
    unsigned                                            increaseCoeff;
    sqlite3*                                            sqlite_db;
public:
                                                        HikeFoodListGenerator( unsigned hikeDays, unsigned peoplecnt, unsigned increaseCoeff, const std::string& dbPath = "" );
                                                        ~HikeFoodListGenerator();

    void                                                addMealToMealListForDaysSQLite();

    const std::map<int, std::vector<Meal*>>             getMealListForDays() const noexcept;
    std::unordered_map<std::string, int>                getOverallMealList() const noexcept;
    unsigned                                            getPeopleCnt() const noexcept;
    unsigned                                            getIncreasedCoeff() const noexcept;
    unsigned                                            getDayCnt() const noexcept;
    static void                                         resertDayCnt() { dayCnt = 0; }
private:
    std::vector<Meal*>                                  MealFactorySQLite( const char* tableName );

    int                                                 getRecordCnt( const char* tableName );
    std::string                                         queryForGetGenerator( const char* tableName, int mealCntInTable, int cnt );

    int                                                 getMealCntInTableSQLite( const char* tableName ) const noexcept;
    std::vector<std::string>                            getRowMealFromTableSQLite( const char* query ) const noexcept;

    int                                                 getRandomCntRecord( int min, int max );
    std::string                                         getWhereIdQuery( const char* tableName, int id );
    std::string                                         getWhereIdQuery( const char* tableName, const std::unordered_map<int, bool>& idRecords );
    void                                                addMealToOverallMeal( const char* name, int weight);

    bool                                                initSQLite(const std::string& dbPath);
    void                                                closeSQLite();
};

#endif // HIKEFOODLISTGENERATOR_H
