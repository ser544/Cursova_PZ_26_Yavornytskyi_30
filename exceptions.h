#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

// ============= БАЗОВИЙ ВИНЯТОК =============
class PatientSystemException : public std::exception {
protected:
    std::string message;
public:
    explicit PatientSystemException(const std::string& msg) : message(msg) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
    virtual ~PatientSystemException() = default;
};

// ============= 1. ВАЛІДАЦІЯ ДАНИХ =============
class ValidationException : public PatientSystemException {
public:
    explicit ValidationException(const std::string& field, const std::string& problem)
        : PatientSystemException("Помилка валідації поля '" + field + "': " + problem) {}
};

// ============= 2. ПАМ'ЯТЬ =============
class MemoryException : public PatientSystemException {
public:
    explicit MemoryException(const std::string& operation = "")
        : PatientSystemException("Помилка виділення пам'яті" +
                                 (operation.empty() ? "" : " при: " + operation)) {}
};

// ============= 3. ІНДЕКСИ ТА ДОСТУП =============
class IndexException : public PatientSystemException {
public:
    IndexException(int index, int size)
        : PatientSystemException("Некоректний індекс: " + std::to_string(index) +
                                 " (розмір списку: " + std::to_string(size) + ")") {}
};

// ============= 4. ПОРОЖНІЙ СПИСОК =============
class EmptyListException : public PatientSystemException {
public:
    explicit EmptyListException(const std::string& operation = "")
        : PatientSystemException("Список пацієнтів порожній" +
                                 (operation.empty() ? "" : ". Операція: " + operation)) {}
};

// ============= 5. НЕ ЗНАЙДЕНО РЕЗУЛЬТАТІВ =============
class NoResultsException : public PatientSystemException {
public:
    explicit NoResultsException(const std::string& criteria)
        : PatientSystemException("Не знайдено результатів за критерієм: " + criteria) {}
};

// ============= 6. ФАЙЛ НЕ ЗНАЙДЕНО =============
class FileNotFoundException : public PatientSystemException {
public:
    explicit FileNotFoundException(const std::string& filename)
        : PatientSystemException("Файл не знайдено: " + filename) {}
};

// ============= 7. ПОМИЛКА ВІДКРИТТЯ ФАЙЛУ =============
class FileOpenException : public PatientSystemException {
public:
    explicit FileOpenException(const std::string& filename)
        : PatientSystemException("Не вдалося відкрити файл: " + filename) {}
};

// ============= 8. ПОМИЛКА ЧИТАННЯ ФАЙЛУ =============
class FileReadException : public PatientSystemException {
public:
    FileReadException(const std::string& filename, int recordNumber = -1)
        : PatientSystemException("Помилка читання файлу '" + filename + "'" +
                                 (recordNumber > 0 ? " на записі №" + std::to_string(recordNumber) : "")) {}
};

// ============= 9. ПОМИЛКА ЗАПИСУ ФАЙЛУ =============
class FileWriteException : public PatientSystemException {
public:
    explicit FileWriteException(const std::string& filename)
        : PatientSystemException("Помилка запису у файл: " + filename) {}
};

// ============= 10. ПОРОЖНІЙ ФАЙЛ =============
class EmptyFileException : public PatientSystemException {
public:
    explicit EmptyFileException(const std::string& filename)
        : PatientSystemException("Файл порожній або не містить коректних даних: " + filename) {}
};

// ============= 11. ПОМИЛКА ПОТОКУ =============
class StreamException : public PatientSystemException {
public:
    explicit StreamException(const std::string& operation)
        : PatientSystemException("Помилка потоку при: " + operation) {}
};

// ============= 12. НЕ ВИБРАНО ЕЛЕМЕНТ =============
class NoSelectionException : public PatientSystemException {
public:
    explicit NoSelectionException(const std::string& what = "елемент")
        : PatientSystemException("Не вибрано " + what + " для операції") {}
};

// ============= 13. ПОМИЛКА ОПЕРАЦІЇ =============
class OperationException : public PatientSystemException {
public:
    explicit OperationException(const std::string& operation, const std::string& reason = "")
        : PatientSystemException("Помилка виконання операції '" + operation + "'" +
                                 (reason.empty() ? "" : ": " + reason)) {}
};

// ============= 14. НЕКОРЕКТНІ ДАНІ =============
class InvalidDataException : public PatientSystemException {
public:
    explicit InvalidDataException(const std::string& details)
        : PatientSystemException("Некоректні дані: " + details) {}
};

// ============= 15. КРИТИЧНА ПОМИЛКА =============
class CriticalException : public PatientSystemException {
public:
    explicit CriticalException(const std::string& details)
        : PatientSystemException("КРИТИЧНА ПОМИЛКА: " + details) {}
};

#endif // EXCEPTIONS_H
