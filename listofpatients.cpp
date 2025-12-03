#include "listofpatients.h"
#include <QMessageBox>
#include <QString>

ListOfPatients::ListOfPatients(){}
ListOfPatients::ListOfPatients(Person* existingListOfPeople, int numberOfPeople){
    for(int i = 0; i < numberOfPeople; i++){
        listOfPeople.push_back(*(existingListOfPeople + i));
    }
}

ListOfPatients& ListOfPatients::operator+=(ListOfPatients& list){
    this->listOfPeople.insert(this->listOfPeople.end(), list.listOfPeople.begin(), list.listOfPeople.end());

    return *this;
}

const Person& ListOfPatients::operator[](int index) const{
    if (listOfPeople.empty()) {
        throw EmptyListException("доступ до елемента");
    }
    if (index < 0 || index >= this->length()) {
        throw IndexException(index, this->length());
    }
    return listOfPeople[index];
}

Person& ListOfPatients::operator[](int index) {
    if (listOfPeople.empty()) {
        throw EmptyListException("доступ до елемента");
    }
    if (index < 0 || index >= this->length()) {
        throw IndexException(index, this->length());
    }
    return listOfPeople[index];
}
int ListOfPatients::length() const{
    return static_cast<int>(this->listOfPeople.size());
}

ListOfPatients* ListOfPatients::getSortedBySexesAndT(Sex sex) {
    if (listOfPeople.empty()) {
        throw EmptyListException("сортування");
    }

    ListOfPatients* result = new ListOfPatients();

    for (const auto& patient : listOfPeople) {
        if (patient.getSex() == sex) {
            result->addPatient(patient);
        }
    }

    if (result->listOfPeople.empty()) {
        delete result;
        result = nullptr;
        std::string sexStr = (sex == Sex::MAN) ? "чоловіки" : "жінки";
        throw NoResultsException(sexStr + " у списку");
    }

    // Сортування bucket sort
    double minT = result->listOfPeople[0].getT();
    double maxT = result->listOfPeople[0].getT();

    for (const auto& patient : result->listOfPeople) {
        double t = patient.getT();
        if (t < minT) minT = t;
        if (t > maxT) maxT = t;
    }

    int scale = 10;
    int minIndex = int(minT * scale);
    int maxIndex = int(maxT * scale);
    int range = maxIndex - minIndex + 1;

    std::vector<std::vector<Person>> buckets(range);

    for (const auto& patient : result->listOfPeople) {
        int index = int(patient.getT() * scale) - minIndex;
        buckets[index].push_back(patient);
    }

    result->listOfPeople.clear();
    for (auto& bucket : buckets) {
        for (auto& p : bucket)
            result->addPatient(p);
    }

    return result;
}

ListOfPatients* ListOfPatients::getWomenWithHighHemoglobin() const{
    if (listOfPeople.empty()) {
        throw EmptyListException("пошук жінок з високим гемоглобіном");
    }

    ListOfPatients* result = new ListOfPatients();

    for (const auto& patient : listOfPeople) {
        if (patient.getSex() == Sex::WOMAN &&
            patient.getHemoglobinValueStatus() == ValueStatus::HIGH) {
            result->addPatient(patient);
        }
    }

    if (result->length() == 0) {
        delete result;
        result = nullptr;  // ✅ Обнуляємо, щоб не було подвійного видалення
        throw NoResultsException("жінки з високим гемоглобіном");
    }

    return result;
}
ListOfPatients* ListOfPatients::getYoungestManWithLowHemoglobin() const{
    if (listOfPeople.empty()) {
        throw EmptyListException("пошук наймолодшого чоловіка з низьким гемоглобіном");
    }

    ListOfPatients* result = new ListOfPatients();
    int youngestAge = -1;

    for (const auto& patient : listOfPeople) {
        if (patient.getSex() == Sex::MAN &&
            patient.getHemoglobinValueStatus() == ValueStatus::LOW) {
            if (int(patient.getAge()) > youngestAge) {
                youngestAge = patient.getAge();
            }
        }
    }

    if (youngestAge == -1) {
        delete result;
        result = nullptr;
        throw NoResultsException("чоловіки з низьким гемоглобіном");
    }

    for (const auto& patient : listOfPeople) {
        if (patient.getSex() == Sex::MAN &&
            patient.getAge() == unsigned(youngestAge) &&
            patient.getHemoglobinValueStatus() == ValueStatus::LOW) {
            result->addPatient(patient);
        }
    }

    return result;
}
ListOfPatients* ListOfPatients::getPeopleOlderThan40WithHighHemoglobinAndLowT() const{
    if (listOfPeople.empty()) {
        throw EmptyListException("пошук людей старших 40 з високим гемоглобіном і низькою температурою");
    }

    ListOfPatients* result = new ListOfPatients();

    for (const auto& patient : listOfPeople) {
        if (patient.getAge() > 40 &&
            patient.getHemoglobinValueStatus() == ValueStatus::HIGH &&
            patient.getTemperatureValueStatus() == ValueStatus::LOW) {
            result->addPatient(patient);
        }
    }

    if (result->length() == 0) {
        delete result;
        result = nullptr;
        throw NoResultsException("люди старші 40 років з високим Hb та низькою t");
    }

    return result;
}

ListOfPatients* ListOfPatients::getOldestWomenWithSameNameAndNormalHemoglobin() const{
    if (this->length() == 0) {
        throw EmptyListException("пошук найстарших жінок з однаковим ім'ям і нормальним гемоглобіном");
    }

    ListOfPatients* result = new ListOfPatients();
    std::map<std::string, int> nameFrequency;

    // Рахуємо частоту імен жінок
    for (int i = 0; i < this->length(); i++) {
        const Person& patient = (*this)[i];
        if (patient.getSex() == Sex::WOMAN) {
            nameFrequency[patient.getName()]++;
        }
    }

    // Знаходимо імена, що повторюються
    std::vector<std::string> repeatedNames;
    for (const auto& p : nameFrequency) {
        if (p.second > 1) {
            repeatedNames.push_back(p.first);
        }
    }

    // Якщо немає імен що повторюються
    if (repeatedNames.empty()) {
        delete result;
        result = nullptr;
        throw NoResultsException("жінки з однаковими іменами");
    }

    // Для кожного імені що повторюється знаходимо найстарших
    for (size_t idx = 0; idx < repeatedNames.size(); idx++) {
        const std::string& name = repeatedNames[idx];

        ListOfPatients* oldest = nullptr;
        try {
            oldest = oldestWomenWithNormalHemoglobinByName(name);

            // Додаємо пацієнтів з oldest до result
            if (oldest != nullptr && oldest->length() > 0) {
                for (int i = 0; i < oldest->length(); i++) {
                    result->addPatient((*oldest)[i]);
                }
            }

        } catch (const NoResultsException&) {
            // Пропускаємо це ім'я
        } catch (const PatientSystemException&) {
            // Пропускаємо це ім'я
        }

        // Завжди видаляємо oldest
        if (oldest != nullptr) {
            delete oldest;
            oldest = nullptr;
        }
    }

    // Фінальна перевірка
    if (result->length() == 0) {
        delete result;
        result = nullptr;
        throw NoResultsException("найстарші жінки з однаковим ім'ям та нормальним гемоглобіном");
    }

    return result;
}

ListOfPatients* ListOfPatients::getPeopleYoungerThan28WithNormalT() const{
    if (listOfPeople.empty()) {
        throw EmptyListException("пошук людей молодших 28 з нормальною температурою");
    }

    ListOfPatients* result = new ListOfPatients();

    for (const auto& patient : listOfPeople) {
        if (patient.getAge() < 28 &&
            patient.getTemperatureValueStatus() == ValueStatus::NORMAL) {
            result->addPatient(patient);
        }
    }

    if (result->length() == 0) {
        delete result;
        result = nullptr;
        throw NoResultsException("люди молодші 28 років з нормальною температурою");
    }

    return result;
}

bool ListOfPatients::addPatient(Person patient){
    try {
        patient.setId(this->length() + 1);
        listOfPeople.push_back(patient);
        return true;
    } catch (std::bad_alloc&) {
        throw MemoryException("додавання пацієнта");
    } catch (const PatientSystemException&) {
        throw;
    } catch (...) {
        throw OperationException("додавання пацієнта", "невідома помилка");
    }

}

void ListOfPatients::removePatient(int index){
    if (listOfPeople.empty()) {
        throw EmptyListException("видалення пацієнта");
    }
    if (index < 0 || index >= this->length()) {
        throw IndexException(index, this->length());
    }

    try {
        listOfPeople.erase(listOfPeople.begin() + index);

        for (int i = index; i < this->length(); i++) {
            listOfPeople[i].setId(i + 1);
        }
    } catch (const PatientSystemException&) {
        throw;
    } catch (...) {
        throw OperationException("видалення пацієнта", "невідома помилка");
    }
}

void ListOfPatients::reset(){
    std::vector<Person>().swap(this->listOfPeople);
}

ListOfPatients* ListOfPatients::oldestWomenWithNormalHemoglobinByName(std::string name) const {
    if (name.empty()) {
        throw ValidationException("ім'я для пошуку", "не може бути порожнім");
    }

    if (this->length() == 0) {
        throw EmptyListException("пошук найстарших жінок за ім'ям");
    }

    ListOfPatients* result = new ListOfPatients();
    int maxAge = -1;

    // Знаходимо максимальний вік
    for (int i = 0; i < this->length(); i++) {
        const Person& patient = (*this)[i];
        if (patient.getSex() == Sex::WOMAN &&
            patient.getName() == name &&
            patient.getHemoglobinValueStatus() == ValueStatus::NORMAL)
        {
            int currentAge = static_cast<int>(patient.getAge());
            if (currentAge > maxAge) {
                maxAge = currentAge;
            }
        }
    }

    // Якщо не знайдено
    if (maxAge == -1) {
        delete result;
        result = nullptr;
        throw NoResultsException("жінки з ім'ям '" + name + "' та нормальним гемоглобіном");
    }

    // Додаємо всіх найстарших
    for (int i = 0; i < this->length(); i++) {
        const Person& patient = (*this)[i];
        if (patient.getSex() == Sex::WOMAN &&
            patient.getName() == name &&
            patient.getHemoglobinValueStatus() == ValueStatus::NORMAL &&
            static_cast<int>(patient.getAge()) == maxAge)
        {
            result->addPatient(patient);
        }
    }

    return result;
}

std::istream& operator>>(std::istream& in, ListOfPatients& obj){
    if (!in.good()) {
        throw StreamException("читання списку (потік у поганому стані)");
    }

    int recordsRead = 0;

    while (true) {
        Person p;
        in >> p;
        // Якщо досягли кінця файлу або помилка читання
        if (in.eof()) {
            break;
        }

        if (in.fail()) {
            // Якщо це не кінець файлу, значить помилка
            if (!in.eof()) {
                throw FileReadException("поточний файл", recordsRead + 1);
            }
            break;
        }

        try {
            obj.addPatient(p);
            recordsRead++;
        } catch (const PatientSystemException&) {
            throw;
        }
    }

    in.clear();
    return in;
}

std::ostream& operator<<(std::ostream& out, const ListOfPatients& obj){
    if (!out.good()) {
        throw StreamException("запис списку (потік у поганому стані)");
    }

    try {
        for (int i = 0; i < obj.length(); i++) {
            out << obj[i] << "\n";
            if (out.fail()) {
                throw StreamException("запис пацієнта №" + std::to_string(i + 1));
            }
        }
    } catch (const PatientSystemException&) {
        throw;
    } catch (...) {
        throw StreamException("запис у потік (невідома помилка)");
    }
    return out;
}
