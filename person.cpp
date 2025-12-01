#include "person.h"

std::ostream& operator<<(std::ostream& out, const Sex& s){
    if(s == Sex::MAN){
        out << "Ч";
    }
    else if(s == Sex::WOMAN){
        out << "Ж";
    }
    else{
        //raise std::Exception;
    }
    return out;
}

std::istream& operator>>(std::istream& in, Sex& s){
    std::string sexStr;
    in >> sexStr;

    if(sexStr == "Чоловік" || sexStr == "Ч"){
        s = Sex::MAN;
    }
    else if(sexStr == "Жінка" || sexStr == "Ж"){
        s = Sex::WOMAN;
    }

    return in;
}

Person::Person(){
    numberOfPeople++;

    id = new int;
    *id = numberOfPeople;

    surname = new std::string;
    *surname = "Яворницький";

    name = new std::string;
    *name = "Сергій";

    birthYear = new unsigned;
    *birthYear = 2007;

    sex = new Sex;
    *sex = Sex::MAN;

    t = new double;
    *t = 36.6;

    hemoglobin = new unsigned;
    *hemoglobin = 150;
}

Person::Person(std::string _surname,
       std::string _name,
       unsigned _birthYear,
       Sex _sex,
       double _t,
       unsigned int _hemoglobin){

    if (_surname.empty()) {
        throw ValidationException("прізвище", "не може бути порожнім");
    }
    if (_surname.length() > 30) {
        throw ValidationException("прізвище", "занадто довге (максимум 50 символів)");
    }
    if (_name.empty()) {
            throw ValidationException("ім'я", "не може бути порожнім");
    }
    if (_name.length() > 30) {
            throw ValidationException("ім'я", "занадто довге (максимум 50 символів)");
    }

    std::time_t t_now = std::time(nullptr);
    std::tm* now = std::localtime(&t_now);
    unsigned int currentYear = now->tm_year + 1900;

    if (_birthYear < 1900 || _birthYear > currentYear) {
        throw ValidationException("рік народження",
                                  "має бути між 1900 та " + std::to_string(currentYear));
    }
    if (currentYear - _birthYear > 120) {
        throw ValidationException("вік", "не може перевищувати 120 років");
    }

    if (_t < 34.0 || _t > 45.0) {
        throw ValidationException("температура",
                                  "має бути між 34°C та 45°C (введено: " +
                                      std::to_string(_t) + "°C)");
    }

    if (_hemoglobin < 50 || _hemoglobin > 250) {
        throw ValidationException("гемоглобін",
                                  "має бути між 50 та 250 (введено: " +
                                      std::to_string(_hemoglobin) + ")");
    }
    try {
        numberOfPeople++;
        id = new int;
        *id = numberOfPeople;
        surname = new std::string(_surname);
        name = new std::string(_name);
        birthYear = new unsigned(_birthYear);
        sex = new Sex(_sex);
        t = new double(_t);
        hemoglobin = new unsigned(_hemoglobin);
    } catch (std::bad_alloc&) {
        numberOfPeople--;
        throw MemoryException("створення Person");
    }
}

/*Person::Person(const Person& other){
    //numberOfPeople++;


    id = new int;
    *id = *(other.id);

    surname = new std::string;
    *surname = *(other.surname);

    name = new std::string;
    *name = *(other.name);

    birthYear = new unsigned;
    *birthYear = *(other.birthYear);

    sex = new Sex;
    *sex = *(other.sex);

    t = new double;
    *t = *(other.t);

    hemoglobin = new unsigned;
    *hemoglobin = *(other.hemoglobin);
}*/

Person::Person(const Person& other)
{
    // Інкрементуємо лічильник, оскільки це нова сутність у векторі (хоча її логічний ID може бути тимчасовим)
    // Якщо логіка вимагає унікального ID, її слід переглянути, але для фікса подвійного free,
    // головне - створити нову пам'ять.
    Person::numberOfPeople++;

    // Створюємо нову пам'ять для кожного члена і копіюємо ЗНАЧЕННЯ
    try {
        id = new int(*other.id);
        surname = new std::string(*other.surname);
        name = new std::string(*other.name);
        birthYear = new unsigned(*other.birthYear);
        sex = new Sex(*other.sex);
        t = new double(*other.t);
        hemoglobin = new unsigned int(*other.hemoglobin);
    } catch (std::bad_alloc&) {
        // Якщо виділення пам'яті не вдалося, відкочуємо зміни
        Person::numberOfPeople--;
        throw MemoryException("конструктор копіювання Person");
    }
}

Person::~Person(){
    delete id;
    delete surname;
    delete name;
    delete birthYear;
    delete sex;
    delete t;
    delete hemoglobin;
    numberOfPeople--;
}
Person& Person::operator=(const Person& other)
{
    if (this != &other) {
        // 1. Звільнення старої пам'яті
        delete id;
        delete surname;
        delete name;
        delete birthYear;
        delete sex;
        delete t;
        delete hemoglobin;

        // 2. Виділення нової пам'яті і глибоке копіювання (з обробкою винятків)
        try {
            id = new int(*other.id);
            surname = new std::string(*other.surname);
            name = new std::string(*other.name);
            birthYear = new unsigned(*other.birthYear);
            sex = new Sex(*other.sex);
            t = new double(*other.t);
            hemoglobin = new unsigned int(*other.hemoglobin);
        } catch (std::bad_alloc&) {
            // У разі помилки, необхідно залишити об'єкт у валідному, хоч і порожньому, стані.
            // Найкращим рішенням є "copy-and-swap idiom", але для швидкої фікса
            // просто прокидаємо виняток.
            throw MemoryException("оператор присвоєння Person");
        }
    }
    return *this;
}

Person& Person::operator=(Person&& other) noexcept
{
    if (this != &other) {
        delete id;
        delete surname;
        delete name;
        delete birthYear;
        delete sex;
        delete t;
        delete hemoglobin;

        id = other.id;
        surname = other.surname;
        name = other.name;
        birthYear = other.birthYear;
        sex = other.sex;
        t = other.t;
        hemoglobin = other.hemoglobin;

        other.id = nullptr;
        other.surname = nullptr;
        other.name = nullptr;
        other.birthYear = nullptr;
        other.sex = nullptr;
        other.t = nullptr;
        other.hemoglobin = nullptr;
    }
    return *this;
}

Person::Person(Person&& other) noexcept
{
    id = other.id;
    surname = other.surname;
    name = other.name;
    birthYear = other.birthYear;
    sex = other.sex;
    t = other.t;
    hemoglobin = other.hemoglobin;

    other.id = nullptr;
    other.surname = nullptr;
    other.name = nullptr;
    other.birthYear = nullptr;
    other.sex = nullptr;
    other.t = nullptr;
    other.hemoglobin = nullptr;
}
/*
Person& Person::operator=(const Person& other){
    if (this == &other) {
        return *this;  // Захист від самоприсвоєння
    }
    try {
        // Створюємо нові об'єкти
        int* new_id = new int;
        *new_id = *(other.id);

        std::string* new_surname = new std::string;
        *new_surname = *(other.surname);

        std::string* new_name = new std::string;
        *new_name = *(other.name);

        unsigned* new_birthYear = new unsigned;
        *new_birthYear = *(other.birthYear);

        Sex* new_sex = new Sex;
        *new_sex = *(other.sex);

        double* new_t = new double;
        *new_t = *(other.t);

        unsigned* new_hemoglobin = new unsigned;
        *new_hemoglobin = *(other.hemoglobin);

        // Видаляємо старі
        delete id;
        delete surname;
        delete name;
        delete birthYear;
        delete sex;
        delete t;
        delete hemoglobin;

        // Присвоюємо нові
        id = new_id;
        surname = new_surname;
        name = new_name;
        birthYear = new_birthYear;
        sex = new_sex;
        t = new_t;
        hemoglobin = new_hemoglobin;

    } catch (std::bad_alloc&) {
        throw MemoryException("присвоєння Person");
    }
    return *this;
}
*/
int Person::getId() const{

    return *(this->id);
}

const std::string Person::getSurname() const{
    return *(this->surname);
}

const std::string Person::getName() const{
    return *(this->name);
}

unsigned int Person::getBirthYear() const{
    return *(this->birthYear);
}

Sex Person::getSex() const{
    return *(this->sex);
}

double Person::getT() const {
    return *(this->t);
}

unsigned int Person::getHemoglobin() const {
    return *(this->hemoglobin);
}

ValueStatus Person::getTemperatureValueStatus() const{
    if((*(this->t) > this->maxTForMan && *(this->sex) == Sex::MAN) ||
        (*(this->t) > this->maxTForWoman && *(this->sex) == Sex::WOMAN)) return ValueStatus::HIGH;
    else if((*(this->t) < this->minTForMan && *(this->sex) == Sex::MAN) ||
        (*(this->t) < this->minTForWoman && *(this->sex) == Sex::WOMAN)) return ValueStatus::LOW;
    else return ValueStatus::NORMAL;
}

ValueStatus Person::getHemoglobinValueStatus() const{
    if((*(this->hemoglobin) > this->maxHemoglobinForMan && *(this->sex) == Sex::MAN) ||
        (*(this->hemoglobin) > this->maxHemoglobinForWoman && *(this->sex) == Sex::WOMAN)) return ValueStatus::HIGH;
    else if((*(this->hemoglobin) < this->minHemoglobinForMan && *(this->sex) == Sex::MAN) ||
             (*(this->hemoglobin) < this->minHemoglobinForWoman && *(this->sex) == Sex::WOMAN)) return ValueStatus::LOW;
    else return ValueStatus::NORMAL;
}

unsigned Person::getAge() const{
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    return static_cast<unsigned>(now->tm_year + 1900) - *(this->birthYear);
}

void Person::setId(int _id) const{
    *(this->id) = _id;
}
void Person::setSurname(std::string _surname) const{
    if (_surname.empty()) {
        throw ValidationException("прізвище", "не може бути порожнім");
    }
    if (_surname.length() > 30) {
        throw ValidationException("прізвище", "занадто довге (вжито: " + std::to_string(_surname.length()) + " елементів; має бути не більше 30)");
    }
    *(this->surname) = _surname;
}
void Person::setName(std::string _name) const{
    if (_name.empty()) {
        throw ValidationException("ім'я", "не може бути порожнім");
    }
    if (_name.length() > 30) {
        throw ValidationException("ім'я", "занадто довге (вжито: " + std::to_string(_name.length()) + " елементів; має бути не більше 30)");
    }
    *(this->name) = _name;
}
void Person::setBirthYear(unsigned _birthYear) const{
    std::time_t t_now = std::time(nullptr);
    std::tm* now = std::localtime(&t_now);
    unsigned int currentYear = now->tm_year + 1900;

    if (_birthYear < 1900 || _birthYear > currentYear) {
        throw ValidationException("рік народження",
                                  "має бути між 1900 та " + std::to_string(currentYear));
    }
    *(this->birthYear) = _birthYear;
}
void Person::setSex(Sex _sex) const{
    *(this->sex) = _sex;
}
void Person::setSex(std::string _sex) const{
    if (_sex != "Ч" && _sex != "Чоловік" && _sex != "Ж" && _sex != "Жінка") {
        throw ValidationException("стать",
                                  "має бути 'Ч', 'Чоловік', 'Ж' або 'Жінка'");
    }
    if (_sex == "Ч" || _sex == "Чоловік") {
        *sex = Sex::MAN;
    } else {
        *sex = Sex::WOMAN;
    }
}
void Person::setT(double _t) const{
    if (_t < 34.0 || _t > 45.0) {
        throw ValidationException("температура", "має бути між 34°C та 45°C");
    }
    *(this->t) = _t;
}
void Person::setHemoglobin(unsigned _hemoglobin){
    if (_hemoglobin < 50 || _hemoglobin > 250) {
        throw ValidationException("гемоглобін", "має бути між 50 та 250");
    }
    *(this->hemoglobin) = _hemoglobin;
}

std::istream& operator>>(std::istream& in, Person& obj){
    /*if (!in.good()) {
        throw StreamException("читання Person (потік у поганому стані)");
    }*/

    int fileId;
    std::string surnameStr, nameStr, sexStr;
    unsigned birthYearVal, hemoglobinVal;
    double tVal;

    in >> fileId >> surnameStr >> nameStr >> birthYearVal >> sexStr >> tVal >> hemoglobinVal;

    if (in.fail() && !in.eof()) {
        throw StreamException("читання даних Person");
    }

    // Якщо досягли кінця файлу, це нормально
    if (in.eof()) {
        return in;
    }

    // Валідація прочитаних даних
    if (surnameStr.empty()) {
        throw InvalidDataException("прізвище порожнє у файлі");
    }
    if (nameStr.empty()) {
        throw InvalidDataException("ім'я порожнє у файлі");
    }
    if (birthYearVal < 1900 || birthYearVal > 2025) {
        throw InvalidDataException("некоректний рік народження: " + std::to_string(birthYearVal));
    }
    if (tVal < 34.0 || tVal > 45.0) {
        throw InvalidDataException("некоректна температура: " + std::to_string(tVal));
    }
    if (hemoglobinVal < 50 || hemoglobinVal > 250) {
        throw InvalidDataException("некоректний гемоглобін: " + std::to_string(hemoglobinVal));
    }

    Person::numberOfPeople++;
    try {
        if (!obj.id) obj.id = new int;
        *(obj.id) = Person::numberOfPeople;

        if (!obj.surname) obj.surname = new std::string;
        *(obj.surname) = surnameStr;

        if (!obj.name) obj.name = new std::string;
        *(obj.name) = nameStr;

        if (!obj.birthYear) obj.birthYear = new unsigned;
        *(obj.birthYear) = birthYearVal;

        if (!obj.sex) obj.sex = new Sex;
        if (sexStr == "Ч" || sexStr == "Чоловік") {
            *(obj.sex) = Sex::MAN;
        } else if (sexStr == "Ж" || sexStr == "Жінка") {
            *(obj.sex) = Sex::WOMAN;
        } else {
            throw InvalidDataException("некоректна стать: " + sexStr);
        }

        if (!obj.t) obj.t = new double;
        *obj.t = tVal;

        if (!obj.hemoglobin) obj.hemoglobin = new unsigned;
        *obj.hemoglobin = hemoglobinVal;

    } catch (std::bad_alloc&) {
        Person::numberOfPeople--;
        throw MemoryException("читання Person з потоку");
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const Person& obj){
    out << *(obj.id) << " " << *(obj.surname) << " " << *(obj.name) << " " << *(obj.birthYear) << " " << *(obj.sex) << " " << *(obj.t) << " " << *(obj.hemoglobin);

    return out;
}


unsigned Person::numberOfPeople = 0;


unsigned Person::minHemoglobinForMan = 110;
unsigned Person::maxHemoglobinForMan = 160;
unsigned Person::minHemoglobinForWoman = 120;
unsigned Person::maxHemoglobinForWoman = 140;

double Person::minTForMan = 36.1;
double Person::maxTForMan = 37.2;
double Person::minTForWoman = 36.3;
double Person::maxTForWoman = 37.4;
