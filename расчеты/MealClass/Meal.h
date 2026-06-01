#ifndef __MEAL_H_
#define __MEAL_H_
#include <unordered_map>
#include <string>

class Meal{
    std::string                             nameDish;
    std::unordered_map<std::string , int>    resourse;
public:
                                            Meal( std::string  name );
    virtual                                 ~Meal();

    void                                    addToResourse(  std::string  ingredient, int weight );
    std::unordered_map<std::string , int>    getResourse() const noexcept;

    std::string                              getMealName() const noexcept;
};

class Breakfast: public Meal{
public:
                                            Breakfast( std::string  name );
                                            ~Breakfast();
};

class Lunch: public Meal{
public:
                                            Lunch( std::string  name );
                                            ~Lunch();
};

class Dinner: public Meal{
public:
                                            Dinner( std::string  name );
                                            ~Dinner();
};

class Bread: public Meal{
public:
                                            Bread( std::string  name );
                                            ~Bread();
};

class PatesForBread: public Meal{
public:
                                            PatesForBread( std::string  name );
                                            ~PatesForBread();
};

class Sweet: public Meal{
public:
                                            Sweet( std::string  name );
                                            ~Sweet();
};


class Snack: public Meal{
public:
                                            Snack( std::string  name );
                                            ~Snack();
};

#endif
