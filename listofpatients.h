#ifndef LISTOFPATIENTS_H
#define LISTOFPATIENTS_H

#include "person.h"
#include <vector>
#include <map>
#include <ctime>

struct ListOfPatintsAndNumber{
    Person* listOfPatients;
    int numberOfPatients;
};

class ListOfPatients{
public:
    friend class Person;

    ListOfPatients();
    ListOfPatients(Person* existingListOfPeople, int numberOfPeople);
    ListOfPatients operator=(ListOfPatients);
    ListOfPatients* getSortedBySexesAndT(Sex);
    ListOfPatients* getWomenWithHighHemoglobin() const;
    ListOfPatients* getYoungestManWithLowHemoglobin() const;
    ListOfPatients* getPeopleOlderThan40WithHighHemoglobinAndLowT() const;
    ListOfPatients* getOldestWomenWithSameNameAndNormalHemoglobin() const;
    ListOfPatients* getPeopleYoungerThan28WithNormalT() const;
    int length() const;
    const Person& operator[](int) const;
    Person& operator[](int index);
    ListOfPatients& operator+=(ListOfPatients&);
    bool addPatient(Person);
    void removePatient(int index);
    void reset();

    friend std::istream& operator>>(std::istream& in, ListOfPatients& obj);
    friend std::ostream& operator<<(std::ostream& out, const ListOfPatients& obj);
private:
    ListOfPatients* oldestWomenWithNormalHemoglobinByName(std::string name) const;
    std::vector<Person> listOfPeople;

};
unsigned getCurrentYear();

#endif // LISTOFPATIENTS_H
