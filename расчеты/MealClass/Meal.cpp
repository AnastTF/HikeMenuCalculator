#include "Meal.h"

Meal::Meal( std::string  name ): nameDish( name )
{}

Meal::~Meal()
{}

void Meal::addToResourse( std::string  ingredient, int weight )
{
    resourse.insert( { ingredient, weight } );
}

std::unordered_map<std::string , int> Meal::getResourse() const noexcept
{
    return resourse;
}

std::string  Meal::getMealName() const noexcept
{
    return nameDish;
}

Breakfast::Breakfast( std::string  name ): Meal( name )
{}

Breakfast::~Breakfast()
{}

Lunch::Lunch( std::string  name ): Meal( name )
{}

Lunch::~Lunch()
{}

Dinner::Dinner( std::string  name ): Meal( name )
{}

Dinner::~Dinner()
{}

Bread::Bread( std::string  name ): Meal( name )
{}

Bread::~Bread()
{}

PatesForBread::PatesForBread( std::string  name ): Meal( name )
{}

PatesForBread::~PatesForBread()
{}

Sweet::Sweet( std::string  name ): Meal( name )
{}

Sweet::~Sweet()
{}

Snack::Snack( std::string  name ): Meal( name )
{}

Snack::~Snack()
{}
