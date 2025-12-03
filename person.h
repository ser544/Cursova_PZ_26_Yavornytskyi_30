#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>
#include <ctime>
#include "exceptions.h"

enum class Sex{
    MAN,
    WOMAN
};

enum class ValueStatus {
    LOW,
    NORMAL,
    HIGH
};

std::ostream& operator<<(std::ostream& out, const Sex& s);
std::istream& operator>>(std::istream& in, Sex& s);

class Person{
public:
    friend class ListOfPatients;

    Person();
    Person(std::string surname,
           std::string name,
           unsigned birthYear,
           Sex sex,
           double t,
           unsigned int hemoglobin);

    ~Person();

    Person(const Person& other);
    Person& operator=(const Person&);

    int getId() const;
    const std::string getSurname() const;
    const std::string getName() const;
    unsigned int getBirthYear() const;
    Sex getSex() const;
    double getT() const;
    unsigned int getHemoglobin() const;

    ValueStatus getTemperatureValueStatus() const;
    ValueStatus getHemoglobinValueStatus() const;

    unsigned getAge() const;

    void setId(int _id) const;
    void setSurname(std::string _surname) const;
    void setName(std::string _name) const;
    void setBirthYear(unsigned _birthYear) const;
    void setSex(Sex _sex) const;
    void setSex(std::string _sex) const;
    void setT(double _t) const;
    void setHemoglobin(unsigned _hemoglobin);



    friend std::istream& operator>>(std::istream& in, Person& obj);
    friend std::ostream& operator<<(std::ostream& out, const Person& obj);

private:
    int* id;
    std::string* surname;
    std::string* name;
    unsigned int* birthYear;
    Sex* sex;
    double* t;
    unsigned int* hemoglobin;
    static unsigned numberOfPeople;

    static const unsigned minHemoglobinForMan;
    static const unsigned maxHemoglobinForMan;
    static const unsigned minHemoglobinForWoman;
    static const unsigned maxHemoglobinForWoman;

    static const double minTForMan;
    static const double maxTForMan;
    static const double minTForWoman;
    static const double maxTForWoman;
};
#endif
