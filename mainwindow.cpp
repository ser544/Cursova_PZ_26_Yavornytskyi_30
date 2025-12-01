#include "mainwindow.h"
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QRadioButton>
#include <iostream>
#include <sstream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    patients = new ListOfPatients();
    setupUI();
}

MainWindow::~MainWindow()
{
    delete patients;
}

void MainWindow::setupUI()
{
    // Центральний віджет
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Головний layout
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // === СЕКЦІЯ КНОПОК ===
    QGroupBox* buttonsGroup = new QGroupBox("Операції", this);
    QVBoxLayout* buttonsLayout = new QVBoxLayout(buttonsGroup);

    // Перший ряд кнопок - файлові операції та додавання
    QHBoxLayout* row1 = new QHBoxLayout();
    btnLoadFile = new QPushButton("📂 Завантажити з файлу", this);
    btnLoadAndAppendFile = new QPushButton("➕ Додати з файлу", this);
    btnSaveFile = new QPushButton("💾 Зберегти у файл", this);
    btnClearAll = new QPushButton("🗑️ Очистити таблицю", this);
    btnAddPatient = new QPushButton("➕ Додати пацієнта", this);
    btnEditPatient = new QPushButton("✏️ Редагувати пацієнта", this);
    btnDeletePatient = new QPushButton("🗑️ Видалити пацієнта", this);
    row1->addWidget(btnLoadFile);
    row1->addWidget(btnLoadAndAppendFile);
    row1->addWidget(btnSaveFile);
    row1->addWidget(btnClearAll);
    row1->addWidget(btnAddPatient);
    row1->addWidget(btnEditPatient);
    row1->addWidget(btnDeletePatient);
    row1->addStretch();

    // Другий ряд - операції над даними
    QHBoxLayout* row2 = new QHBoxLayout();
    btnSortByT = new QPushButton("1️⃣ Сортувати за t (по статях)", this);
    btnWomenHighHb = new QPushButton("2️⃣ Жінки з високим Hb", this);
    btnYoungestMan = new QPushButton("3️⃣ Наймолодший чоловік", this);
    row2->addWidget(btnSortByT);
    row2->addWidget(btnWomenHighHb);
    row2->addWidget(btnYoungestMan);

    // Третій ряд
    QHBoxLayout* row3 = new QHBoxLayout();
    btnPeopleOver40 = new QPushButton("4️⃣ Люди >40 років", this);
    btnOldestWomen = new QPushButton("5️⃣ Найстарші жінки (одне ім'я)", this);
    btnPeopleUnder28 = new QPushButton("6️⃣ Пацієнти <28 років", this);
    row3->addWidget(btnPeopleOver40);
    row3->addWidget(btnOldestWomen);
    row3->addWidget(btnPeopleUnder28);

    buttonsLayout->addLayout(row1);
    buttonsLayout->addLayout(row2);
    buttonsLayout->addLayout(row3);

    // === СЕКЦІЯ ТАБЛИЦІ ===
    QGroupBox* tableGroup = new QGroupBox("Список пацієнтів", this);
    QVBoxLayout* tableLayout = new QVBoxLayout(tableGroup);

    setupTable();
    tableLayout->addWidget(mainTable);

    // Додаємо все до головного layout
    mainLayout->addWidget(buttonsGroup);
    mainLayout->addWidget(tableGroup, 1); // 1 = stretch factor

    // Підключення сигналів
    connect(btnAddPatient, &QPushButton::clicked, this, &MainWindow::onAddPatient);
    connect(btnEditPatient, &QPushButton::clicked, this, &MainWindow::onEditPatient);
    connect(btnDeletePatient, &QPushButton::clicked, this, &MainWindow::onDeletePatient);
    connect(btnSortByT, &QPushButton::clicked, this, &MainWindow::onSortByT);
    connect(btnWomenHighHb, &QPushButton::clicked, this, &MainWindow::onWomenHighHemoglobin);
    connect(btnYoungestMan, &QPushButton::clicked, this, &MainWindow::onYoungestMan);
    connect(btnPeopleOver40, &QPushButton::clicked, this, &MainWindow::onPeopleOver40);
    connect(btnOldestWomen, &QPushButton::clicked, this, &MainWindow::onOldestWomenSameName);
    connect(btnPeopleUnder28, &QPushButton::clicked, this, &MainWindow::onPeopleUnder28);
    connect(btnLoadFile, &QPushButton::clicked, this, &MainWindow::onLoadFromFile);
    connect(btnLoadAndAppendFile, &QPushButton::clicked, this, &MainWindow::onLoadAndAppendFromFile);
    connect(btnSaveFile, &QPushButton::clicked, this, &MainWindow::onSaveToFile);
    connect(btnClearAll, &QPushButton::clicked, this, &MainWindow::onClearAll);
    // Налаштування вікна
    setWindowTitle("Система управління пацієнтами");
    resize(1200, 700);
}

void MainWindow::setupTable()
{
    mainTable = new QTableWidget(this);
    mainTable->setColumnCount(7);

    QStringList headers;
    headers << "№" << "Прізвище" << "Ім'я" << "Рік народж."
            << "Стать" << "t (°C)" << "Гемоглобін";
    mainTable->setHorizontalHeaderLabels(headers);

    // Налаштування вигляду
    mainTable->setAlternatingRowColors(true);
    mainTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    mainTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainTable->horizontalHeader()->setStretchLastSection(true);
    mainTable->verticalHeader()->setVisible(false);

    // Автоматичне розтягування колонок
    for(int i = 0; i < 7; i++) {
        mainTable->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
}

void MainWindow::updateMainTable()
{
    // ТУТ ВИ ДОДАСТЕ КОД ДЛЯ ОНОВЛЕННЯ ТАБЛИЦІ З ВАШОГО ListOfPatients
    // Використовуйте std::vector та std::string

    mainTable->clearContents();
    mainTable->setRowCount(patients->length());

    for(int i = 0; i < patients->length(); i++) {
        (*patients)[i].setId(i + 1);
        const Person& p = (*patients)[i];
        p.setId(i+1);
        mainTable->setItem(i, 0, new QTableWidgetItem(QString::number(p.getId())));
        mainTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(p.getSurname())));
        mainTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(p.getName())));
        mainTable->setItem(i, 3, new QTableWidgetItem(QString::number(p.getBirthYear())));

        std::string _sex = p.getSex() == Sex::MAN ? "Ч" : "Ж";
        mainTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(_sex)));

        // === ТЕМПЕРАТУРА ===
        QTableWidgetItem* tItem =
            new QTableWidgetItem(QString::number(p.getT(), 'f', 1));

        ValueStatus tStatus = p.getTemperatureValueStatus();
        switch(tStatus) {
        case ValueStatus::LOW:
            tItem->setBackground(QBrush(QColor(173, 216, 230))); // світло-синій
            break;
        case ValueStatus::NORMAL:
            tItem->setBackground(QBrush(QColor(144, 238, 144))); // світло-зелений
            break;
        case ValueStatus::HIGH:
            tItem->setBackground(QBrush(QColor(255, 182, 193))); // світло-червоний
            break;
        }

        mainTable->setItem(i, 5, tItem);

        // === ГЕМОГЛОБІН ===
        QTableWidgetItem* hbItem =
            new QTableWidgetItem(QString::number(p.getHemoglobin()));

        ValueStatus hbStatus = p.getHemoglobinValueStatus();
        switch(hbStatus) {
        case ValueStatus::LOW:
            hbItem->setBackground(QBrush(QColor(255, 255, 128))); // жовтий
            break;
        case ValueStatus::NORMAL:
            hbItem->setBackground(QBrush(QColor(144, 238, 144))); // світло-зелений
            break;
        case ValueStatus::HIGH:
            hbItem->setBackground(QBrush(QColor(255, 200, 120))); // помаранчевий
            break;
        }

        mainTable->setItem(i, 6, hbItem);
    }

    //std::cout << patient
}

void MainWindow::showResultDialog(ListOfPatients* result, const std::string& title)
{
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle(QString::fromStdString(title));
    dialog->resize(900, 500);

    QVBoxLayout* layout = new QVBoxLayout(dialog);

    QTableWidget* resultTable = new QTableWidget(dialog);
    resultTable->setColumnCount(7);

    QStringList headers;
    headers << "№" << "Прізвище" << "Ім'я" << "Рік народж."
            << "Стать" << "t (°C)" << "Гемоглобін";
    resultTable->setHorizontalHeaderLabels(headers);

    resultTable->setAlternatingRowColors(true);
    resultTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    resultTable->setRowCount(result->length());

    for(int i = 0; i < result->length(); i++) {
        const Person& p = (*result)[i];

        resultTable->setItem(i, 0, new QTableWidgetItem(QString::number(p.getId())));
        resultTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(p.getSurname())));
        resultTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(p.getName())));
        resultTable->setItem(i, 3, new QTableWidgetItem(QString::number(p.getBirthYear())));

        std::string _sex = p.getSex() == Sex::MAN ? "Ч" : "Ж";
        resultTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(_sex)));

        // === ТЕМПЕРАТУРА (з ValueStatus) ===
        QTableWidgetItem* tItem =
            new QTableWidgetItem(QString::number(p.getT(), 'f', 1));
        ValueStatus tStatus = p.getTemperatureValueStatus();

        switch(tStatus) {
        case ValueStatus::LOW:
            tItem->setBackground(QBrush(QColor(173, 216, 230))); // світло-синій
            break;
        case ValueStatus::NORMAL:
            tItem->setBackground(QBrush(QColor(144, 238, 144))); // світло-зелений
            break;
        case ValueStatus::HIGH:
            tItem->setBackground(QBrush(QColor(255, 182, 193))); // світло-червоний
            break;
        }

        resultTable->setItem(i, 5, tItem);

        // === ГЕМОГЛОБІН (з ValueStatus) ===
        QTableWidgetItem* hbItem =
            new QTableWidgetItem(QString::number(p.getHemoglobin()));
        ValueStatus hbStatus = p.getHemoglobinValueStatus();

        switch(hbStatus) {
        case ValueStatus::LOW:
            hbItem->setBackground(QBrush(QColor(255, 255, 128))); // жовтий
            break;
        case ValueStatus::NORMAL:
            hbItem->setBackground(QBrush(QColor(144, 238, 144))); // світло-зелений
            break;
        case ValueStatus::HIGH:
            hbItem->setBackground(QBrush(QColor(255, 200, 120))); // помаранчевий
            break;
        }

        resultTable->setItem(i, 6, hbItem);
    }

    layout->addWidget(resultTable);

    // === КНОПКИ ===
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    QPushButton* saveBtn = new QPushButton("💾 Зберегти у файл", dialog);
    QPushButton* closeBtn = new QPushButton("Закрити", dialog);

    buttonLayout->addWidget(saveBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);

    layout->addLayout(buttonLayout);

    // === ОБРОБКА ЗБЕРЕЖЕННЯ ===
    connect(saveBtn, &QPushButton::clicked, [this, result]() {
        try {
            if (result->length() == 0) {
                throw EmptyListException("збереження результатів");
            }

            QString qfilename = QFileDialog::getSaveFileName(this,
                                                             QString::fromUtf8("Зберегти результати"),
                                                             "",
                                                             "Файли даних (*.txt *.dat);;Всі файли (*)");

            if (qfilename.isEmpty()) return;

            std::filesystem::path path = qfilename.toStdWString();

            // Перевірка існування батьківської директорії
            std::filesystem::path parentPath = path.parent_path();
            if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
                throw InvalidDataException("директорія не існує: " + parentPath.string());
            }

            std::ofstream file(path);

            if (!file.is_open()) {
                throw FileOpenException(qfilename.toStdString());
            }

            if (!file.good()) {
                throw StreamException("запис у файл (потік у поганому стані)");
            }

            file << *result;

            if (file.fail()) {
                throw FileWriteException(qfilename.toStdString());
            }

            file.close();

            QMessageBox::information(this,
                                     QString::fromUtf8("Збереження"),
                                     QString::fromUtf8("Результати успішно збережено.\nЗбережено записів: ") +
                                         QString::number(result->length()));

        } catch (const PatientSystemException& e) {
            QMessageBox::critical(this,
                                  QString::fromUtf8("Помилка"),
                                  QString::fromUtf8(e.what()));
        }
    });

    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);

    dialog->exec();
    delete dialog;
}

// === СЛОТИ ДЛЯ КНОПОК ===

void MainWindow::onAddPatient() {
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Додати нового пацієнта");

    QFormLayout* formLayout = new QFormLayout(dialog);

    QLineEdit* surnameEdit = new QLineEdit(dialog);
    QLineEdit* nameEdit = new QLineEdit(dialog);
    QSpinBox* birthYearSpin = new QSpinBox(dialog);
    birthYearSpin->setRange(1900, 2025);
    birthYearSpin->setValue(2000);

    QComboBox* sexCombo = new QComboBox(dialog);
    sexCombo->addItem("Чоловік", "M");
    sexCombo->addItem("Жінка", "W");

    QDoubleSpinBox* tSpin = new QDoubleSpinBox(dialog);
    tSpin->setRange(35.0, 42.0);
    tSpin->setValue(36.6);
    tSpin->setDecimals(1);
    tSpin->setSingleStep(0.1);

    QSpinBox* hemoglobinSpin = new QSpinBox(dialog);
    hemoglobinSpin->setRange(50, 250);
    hemoglobinSpin->setValue(140);

    formLayout->addRow("Прізвище:", surnameEdit);
    formLayout->addRow("Ім'я:", nameEdit);
    formLayout->addRow("Рік народження:", birthYearSpin);
    formLayout->addRow("Стать:", sexCombo);
    formLayout->addRow("Температура (°C):", tSpin);
    formLayout->addRow("Гемоглобін:", hemoglobinSpin);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
    formLayout->addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

    if (dialog->exec() == QDialog::Accepted) {
        try {
            std::string surname = surnameEdit->text().trimmed().toStdString();
            std::string name = nameEdit->text().trimmed().toStdString();

            // Валідація прізвища
            if (surname.empty()) {
                throw ValidationException("Прізвище", "не може бути порожнім");
            }

            // Валідація імені
            if (name.empty()) {
                throw ValidationException("Ім'я", "не може бути порожнім");
            }

            unsigned birthYear = birthYearSpin->value();
            Sex sex = (sexCombo->currentData().toString() == "M") ? Sex::MAN : Sex::WOMAN;
            double t = tSpin->value();
            unsigned hemoglobin = hemoglobinSpin->value();

            Person newPatient(surname, name, birthYear, sex, t, hemoglobin);
            patients->addPatient(newPatient);
            updateMainTable();

            QMessageBox::information(this,
                                     QString::fromUtf8("Успіх"),
                                     QString::fromUtf8("Пацієнта успішно додано"));

        } catch (const PatientSystemException& e) {
            QMessageBox::warning(this,
                                 QString::fromUtf8("Помилка"),
                                 QString::fromUtf8(e.what()));
        }
    }

    delete dialog;
}

void MainWindow::onEditPatient() {
    int selectedRow = mainTable->currentRow();

    if (selectedRow < 0) {
        try {
            throw NoSelectionException("пацієнта");
        } catch (const NoSelectionException& e) {
            QMessageBox::warning(this, QString::fromUtf8("Попередження"),
                                 QString::fromUtf8(e.what()));
            return;
        }
    }

    try {
        // Перевірка валідності індексу
        if (selectedRow >= patients->length()) {
            throw IndexException(selectedRow, patients->length());
        }

        const Person& currentPatient = (*patients)[selectedRow];

        QDialog* dialog = new QDialog(this);
        dialog->setWindowTitle("Редагувати пацієнта");

        QFormLayout* formLayout = new QFormLayout(dialog);

        QLineEdit* surnameEdit = new QLineEdit(QString::fromStdString(currentPatient.getSurname()), dialog);
        QLineEdit* nameEdit = new QLineEdit(QString::fromStdString(currentPatient.getName()), dialog);

        QSpinBox* birthYearSpin = new QSpinBox(dialog);
        birthYearSpin->setRange(1900, 2025);
        birthYearSpin->setValue(currentPatient.getBirthYear());

        QComboBox* sexCombo = new QComboBox(dialog);
        sexCombo->addItem("Чоловік", "M");
        sexCombo->addItem("Жінка", "W");
        sexCombo->setCurrentIndex(currentPatient.getSex() == Sex::MAN ? 0 : 1);

        QDoubleSpinBox* tSpin = new QDoubleSpinBox(dialog);
        tSpin->setRange(35.0, 42.0);
        tSpin->setValue(currentPatient.getT());
        tSpin->setDecimals(1);
        tSpin->setSingleStep(0.1);

        QSpinBox* hemoglobinSpin = new QSpinBox(dialog);
        hemoglobinSpin->setRange(50, 250);
        hemoglobinSpin->setValue(currentPatient.getHemoglobin());

        formLayout->addRow("Прізвище:", surnameEdit);
        formLayout->addRow("Ім'я:", nameEdit);
        formLayout->addRow("Рік народження:", birthYearSpin);
        formLayout->addRow("Стать:", sexCombo);
        formLayout->addRow("Температура (°C):", tSpin);
        formLayout->addRow("Гемоглобін:", hemoglobinSpin);

        QDialogButtonBox* buttonBox = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
        formLayout->addRow(buttonBox);

        connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

        if (dialog->exec() == QDialog::Accepted) {
            std::string surname = surnameEdit->text().trimmed().toStdString();
            std::string name = nameEdit->text().trimmed().toStdString();

            // Валідація
            if (surname.empty()) {
                throw ValidationException("Прізвище", "не може бути порожнім");
            }

            if (name.empty()) {
                throw ValidationException("Ім'я", "не може бути порожнім");
            }

            unsigned birthYear = birthYearSpin->value();
            Sex sex = (sexCombo->currentData().toString() == "M") ? Sex::MAN : Sex::WOMAN;
            double t = tSpin->value();
            unsigned hemoglobin = hemoglobinSpin->value();

            Person& patientRef = (*patients)[selectedRow];
            int oldId = patientRef.getId();

            patientRef.setSurname(surname);
            patientRef.setName(name);
            patientRef.setBirthYear(birthYear);
            patientRef.setSex(sex);
            patientRef.setT(t);
            patientRef.setHemoglobin(hemoglobin);
            patientRef.setId(oldId);

            updateMainTable();

            QMessageBox::information(this,
                                     QString::fromUtf8("Успіх"),
                                     QString::fromUtf8("Дані пацієнта успішно оновлено"));
        }

        delete dialog;

    } catch (const PatientSystemException& e) {
        QMessageBox::critical(this, QString::fromUtf8("Помилка"),
                              QString::fromUtf8(e.what()));
    }
}

void MainWindow::onDeletePatient() {
    int selectedRow = mainTable->currentRow();

    if (selectedRow < 0) {
        try {
            throw NoSelectionException("пацієнта");
        } catch (const NoSelectionException& e) {
            QMessageBox::warning(this, QString::fromUtf8("Попередження"),
                                 QString::fromUtf8(e.what()));
            return;
        }
    }

    try {
        // Перевірка валідності індексу
        if (selectedRow >= patients->length()) {
            throw IndexException(selectedRow, patients->length());
        }

        const Person& patient = (*patients)[selectedRow];
        QString patientInfo = QString::fromStdString(patient.getSurname() + " " + patient.getName());

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,
                                      QString::fromUtf8("Підтвердження"),
                                      QString::fromUtf8("Ви дійсно хочете видалити пацієнта:\n") +
                                          patientInfo + "?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            patients->removePatient(selectedRow);
            updateMainTable();

            QMessageBox::information(this,
                                     QString::fromUtf8("Успіх"),
                                     QString::fromUtf8("Пацієнта успішно видалено"));
        }

    } catch (const PatientSystemException& e) {
        QMessageBox::critical(this, QString::fromUtf8("Помилка"),
                              QString::fromUtf8(e.what()));
    }
}
void MainWindow::onClearAll() {
    if (patients->length() == 0) {
        QMessageBox::information(this,
                                 QString::fromUtf8("Інформація"),
                                 QString::fromUtf8("Таблиця вже порожня"));
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  QString::fromUtf8("Підтвердження"),
                                  QString::fromUtf8("Ви дійсно хочете видалити ВСІ дані?\n"
                                                    "Всього записів: ") + QString::number(patients->length()),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        patients->reset();
        updateMainTable();

        QMessageBox::information(this,
                                 QString::fromUtf8("Успіх"),
                                 QString::fromUtf8("Всі дані успішно видалено"));
    }
}
/*
void MainWindow::onWomenHighHemoglobin() {
    try {
        if (patients->length() == 0) {
            throw EmptyListException("пошук жінок з високим гемоглобіном");
        }

        ListOfPatients* result = patients->getWomenWithHighHemoglobin();

        if (result == nullptr || result->length() == 0) {
            if (result != nullptr) delete result;
            throw NoResultsException("жінки з підвищеним гемоглобіном");
        }

        showResultDialog(result, "Жінки з підвищеним гемоглобіном");
        delete result;

    } catch (const NoResultsException& e) {
        QMessageBox::information(this, QString::fromUtf8("Інформація"),
                                 QString::fromUtf8(e.what()));
    } catch (const EmptyListException& e) {
        QMessageBox::warning(this, QString::fromUtf8("Попередження"),
                             QString::fromUtf8(e.what()));
    } catch (const PatientSystemException& e) {
        QMessageBox::critical(this, QString::fromUtf8("Помилка"),
                              QString::fromUtf8(e.what()));
    }
}
*/
void MainWindow::onWomenHighHemoglobin() {
    std::cout << "=== onWomenHighHemoglobin START ===" << std::endl;
    try {
        std::cout << "Checking if list is empty..." << std::endl;
        if (patients->length() == 0) {
            std::cout << "List is empty, throwing exception" << std::endl;
            throw EmptyListException("пошук жінок з високим гемоглобіном");
        }

        std::cout << "Calling getWomenWithHighHemoglobin()..." << std::endl;
        ListOfPatients* result = patients->getWomenWithHighHemoglobin();
        std::cout << "Returned from getWomenWithHighHemoglobin()" << std::endl;

        if (result == nullptr) {
            std::cout << "Result is NULL!" << std::endl;
            throw NoResultsException("жінки з підвищеним гемоглобіном");
        }

        std::cout << "Result length: " << result->length() << std::endl;

        if (result->length() == 0) {
            std::cout << "Result is empty, deleting and throwing" << std::endl;
            delete result;
            throw NoResultsException("жінки з підвищеним гемоглобіном");
        }

        std::cout << "Calling showResultDialog..." << std::endl;
        showResultDialog(result, "Жінки з підвищеним гемоглобіном");
        std::cout << "Returned from showResultDialog, deleting result..." << std::endl;
        delete result;
        std::cout << "=== onWomenHighHemoglobin SUCCESS ===" << std::endl;

    } catch (const NoResultsException& e) {
        std::cout << "Caught NoResultsException: " << e.what() << std::endl;
        QMessageBox::information(this, QString::fromUtf8("Інформація"),
                                 QString::fromUtf8(e.what()));
    } catch (const EmptyListException& e) {
        std::cout << "Caught EmptyListException: " << e.what() << std::endl;
        QMessageBox::warning(this, QString::fromUtf8("Попередження"),
                             QString::fromUtf8(e.what()));
    } catch (const PatientSystemException& e) {
        std::cout << "Caught PatientSystemException: " << e.what() << std::endl;
        QMessageBox::critical(this, QString::fromUtf8("Помилка"),
                              QString::fromUtf8(e.what()));
    } catch (const std::exception& e) {
        std::cout << "Caught std::exception: " << e.what() << std::endl;
        QMessageBox::critical(this, QString::fromUtf8("Помилка"),
                              QString("Стандартна помилка: ") + e.what());
    } catch (...) {
        std::cout << "Caught unknown exception!" << std::endl;
        QMessageBox::critical(this, QString::fromUtf8("Помилка"),
                              QString::fromUtf8("Невідома помилка"));
    }
    std::cout << "=== onWomenHighHemoglobin END ===" << std::endl;
}
void MainWindow::onSortByT() {
    try {
        if (patients->length() == 0) {
            throw EmptyListException("сортування за температурою");
        }

        QDialog* dialog = new QDialog(this);
        dialog->setWindowTitle("Сортування за температурою");
        dialog->resize(1000, 700);

        QVBoxLayout* mainLayout = new QVBoxLayout(dialog);

        QGroupBox* radioGroup = new QGroupBox("Оберіть стать:", dialog);
        QHBoxLayout* radioLayout = new QHBoxLayout(radioGroup);

        QRadioButton* menRadio = new QRadioButton("👨 Чоловіки", radioGroup);
        QRadioButton* womenRadio = new QRadioButton("👩 Жінки", radioGroup);
        menRadio->setChecked(true);

        QFont radioFont;
        radioFont.setPointSize(11);
        menRadio->setFont(radioFont);
        womenRadio->setFont(radioFont);

        radioLayout->addWidget(menRadio);
        radioLayout->addWidget(womenRadio);
        radioLayout->addStretch();

        mainLayout->addWidget(radioGroup);

        QTableWidget* resultTable = new QTableWidget(dialog);
        resultTable->setColumnCount(7);

        QStringList headers;
        headers << "№" << "Прізвище" << "Ім'я" << "Рік народж."
                << "Стать" << "t (°C)" << "Гемоглобін";
        resultTable->setHorizontalHeaderLabels(headers);

        resultTable->setAlternatingRowColors(true);
        resultTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

        for(int i = 0; i < 7; i++) {
            resultTable->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        }

        mainLayout->addWidget(resultTable);

        QLabel* statsLabel = new QLabel(dialog);
        QFont statsFont;
        statsFont.setPointSize(10);
        statsLabel->setFont(statsFont);
        statsLabel->setStyleSheet("QLabel { padding: 10px; background-color: #f0f0f0; border-radius: 5px; }");
        mainLayout->addWidget(statsLabel);

        // ✅ Зберігаємо поточний відсортований список
        ListOfPatients* currentSorted = nullptr;
        Sex currentSex = Sex::MAN;

        auto updateTable = [this, resultTable, statsLabel, dialog, &currentSorted, &currentSex](Sex sex) {
            try {
                // Видаляємо попередній результат
                if (currentSorted != nullptr) {
                    delete currentSorted;
                    currentSorted = nullptr;
                }

                currentSorted = patients->getSortedBySexesAndT(sex);
                currentSex = sex;

                if (currentSorted == nullptr || currentSorted->length() == 0) {
                    if (currentSorted != nullptr) delete currentSorted;
                    currentSorted = nullptr;
                    QString sexStr = (sex == Sex::MAN) ? "чоловіків" : "жінок";
                    throw NoResultsException("пацієнти статі: " + sexStr.toStdString());
                }

                resultTable->clearContents();
                resultTable->setRowCount(currentSorted->length());

                double minT = 999.0, maxT = 0.0, sumT = 0.0;

                for(int i = 0; i < currentSorted->length(); i++) {
                    const Person& p = (*currentSorted)[i];

                    resultTable->setItem(i, 0, new QTableWidgetItem(QString::number(p.getId())));
                    resultTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(p.getSurname())));
                    resultTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(p.getName())));
                    resultTable->setItem(i, 3, new QTableWidgetItem(QString::number(p.getBirthYear())));

                    std::string _sex = p.getSex() == Sex::MAN ? "Ч" : "Ж";
                    resultTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(_sex)));

                    QTableWidgetItem* tItem = new QTableWidgetItem(QString::number(p.getT(), 'f', 1));
                    ValueStatus tStatus = p.getTemperatureValueStatus();

                    switch(tStatus) {
                    case ValueStatus::LOW:
                        tItem->setBackground(QBrush(QColor(173, 216, 230)));
                        break;
                    case ValueStatus::NORMAL:
                        tItem->setBackground(QBrush(QColor(144, 238, 144)));
                        break;
                    case ValueStatus::HIGH:
                        tItem->setBackground(QBrush(QColor(255, 182, 193)));
                        break;
                    }

                    resultTable->setItem(i, 5, tItem);

                    QTableWidgetItem* hbItem = new QTableWidgetItem(QString::number(p.getHemoglobin()));
                    ValueStatus hbStatus = p.getHemoglobinValueStatus();

                    switch(hbStatus) {
                    case ValueStatus::LOW:
                        hbItem->setBackground(QBrush(QColor(255, 255, 128)));
                        break;
                    case ValueStatus::NORMAL:
                        hbItem->setBackground(QBrush(QColor(144, 238, 144)));
                        break;
                    case ValueStatus::HIGH:
                        hbItem->setBackground(QBrush(QColor(255, 200, 120)));
                        break;
                    }

                    resultTable->setItem(i, 6, hbItem);

                    double t = p.getT();
                    if(t < minT) minT = t;
                    if(t > maxT) maxT = t;
                    sumT += t;
                }

                if(currentSorted->length() > 0) {
                    double avgT = sumT / currentSorted->length();
                    QString sexStr = (sex == Sex::MAN) ? "чоловіків" : "жінок";
                    statsLabel->setText(QString("📊 Статистика для %1: Всього: %2 | Min t: %3°C | Max t: %4°C | Середня t: %5°C")
                                            .arg(sexStr)
                                            .arg(currentSorted->length())
                                            .arg(minT, 0, 'f', 1)
                                            .arg(maxT, 0, 'f', 1)
                                            .arg(avgT, 0, 'f', 1));
                }

            } catch (const NoResultsException& e) {
                statsLabel->setText(QString("⚠️ ") + QString::fromUtf8(e.what()));
                resultTable->clearContents();
                resultTable->setRowCount(0);
                QMessageBox::information(dialog,
                                         QString::fromUtf8("Інформація"),
                                         QString::fromUtf8(e.what()));
            } catch (const PatientSystemException& e) {
                statsLabel->setText(QString("❌ Помилка: ") + QString::fromUtf8(e.what()));
                resultTable->clearContents();
                resultTable->setRowCount(0);
                QMessageBox::critical(dialog,
                                      QString::fromUtf8("Помилка"),
                                      QString::fromUtf8(e.what()));
            }
        };

        updateTable(Sex::MAN);

        connect(menRadio, &QRadioButton::toggled, [updateTable](bool checked) {
            if(checked) updateTable(Sex::MAN);
        });

        connect(womenRadio, &QRadioButton::toggled, [updateTable](bool checked) {
            if(checked) updateTable(Sex::WOMAN);
        });

        // ✅ КНОПКИ З ЗБЕРЕЖЕННЯМ
        QHBoxLayout* buttonLayout = new QHBoxLayout();

        QPushButton* saveBtn = new QPushButton("💾 Зберегти у файл", dialog);
        QPushButton* closeBtn = new QPushButton("Закрити", dialog);
        closeBtn->setMinimumHeight(35);
        saveBtn->setMinimumHeight(35);

        buttonLayout->addWidget(saveBtn);
        buttonLayout->addStretch();
        buttonLayout->addWidget(closeBtn);
        mainLayout->addLayout(buttonLayout);

        // ✅ ОБРОБКА ЗБЕРЕЖЕННЯ
        connect(saveBtn, &QPushButton::clicked, [this, &currentSorted, &currentSex]() {
            try {
                if (currentSorted == nullptr || currentSorted->length() == 0) {
                    throw EmptyListException("збереження результатів");
                }

                QString qfilename = QFileDialog::getSaveFileName(this,
                                                                 QString::fromUtf8("Зберегти результати"),
                                                                 "",
                                                                 "Файли даних (*.txt *.dat);;Всі файли (*)");

                if (qfilename.isEmpty()) return;

                std::filesystem::path path = qfilename.toStdWString();

                std::filesystem::path parentPath = path.parent_path();
                if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
                    throw InvalidDataException("директорія не існує: " + parentPath.string());
                }

                std::ofstream file(path);

                if (!file.is_open()) {
                    throw FileOpenException(qfilename.toStdString());
                }

                if (!file.good()) {
                    throw StreamException("запис у файл (потік у поганому стані)");
                }

                file << *currentSorted;

                if (file.fail()) {
                    throw FileWriteException(qfilename.toStdString());
                }

                file.close();

                QString sexStr = (currentSex == Sex::MAN) ? "чоловіків" : "жінок";
                QMessageBox::information(this,
                                         QString::fromUtf8("Збереження"),
                                         QString::fromUtf8("Результати для ") + sexStr +
                                             QString::fromUtf8(" успішно збережено.\nЗбережено записів: ") +
                                             QString::number(currentSorted->length()));

            } catch (const PatientSystemException& e) {
                QMessageBox::critical(this,
                                      QString::fromUtf8("Помилка"),
                                      QString::fromUtf8(e.what()));
            }
        });

        connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);

        dialog->exec();

        // Очищення пам'яті
        if (currentSorted != nullptr) {
            delete currentSorted;
            currentSorted = nullptr;
        }

        delete dialog;

    } catch (const EmptyListException& e) {
        QMessageBox::warning(this, QString::fromUtf8("Попередження"),
                             QString::fromUtf8(e.what()));
    } catch (const PatientSystemException& e) {
        QMessageBox::critical(this, QString::fromUtf8("Помилка"),
                              QString::fromUtf8(e.what()));
    }
}

void MainWindow::onYoungestMan() {
    try {
        if (patients->length() == 0) {
            throw EmptyListException("пошук наймолодшого чоловіка");
        }

        ListOfPatients* result = patients->getYoungestManWithLowHemoglobin();

        if (result == nullptr || result->length() == 0) {
            if (result != nullptr) delete result;
            throw NoResultsException("наймолодший чоловік з нормальною t та зниженим Hb");
        }

        showResultDialog(result, "Наймолодший чоловік з нормальною t та зниженим Hb");
        delete result;

    } catch (const NoResultsException& e) {
        QMessageBox::information(this, QString::fromUtf8("Інформація"),
                                 QString::fromUtf8(e.what()));
    } catch (const EmptyListException& e) {
        QMessageBox::warning(this, QString::fromUtf8("Попередження"),
                             QString::fromUtf8(e.what()));
    } catch (const PatientSystemException& e) {
        QMessageBox::critical(this, QString::fromUtf8("Помилка"),
                              QString::fromUtf8(e.what()));
    }
}

void MainWindow::onPeopleOver40() {
    try {
        if (patients->length() == 0) {
            throw EmptyListException("пошук людей старше 40 років");
        }

        ListOfPatients* result = patients->getPeopleOlderThan40WithHighHemoglobinAndLowT();

        if (result == nullptr || result->length() == 0) {
            if (result != nullptr) delete result;
            throw NoResultsException("люди старші 40 років з високим Hb та низькою t");
        }

        showResultDialog(result, "Люди старші 40 років з високим Hb та низькою t");
        delete result;

    } catch (const NoResultsException& e) {
        QMessageBox::information(this, QString::fromUtf8("Інформація"),
                                 QString::fromUtf8(e.what()));
    } catch (const EmptyListException& e) {
        QMessageBox::warning(this, QString::fromUtf8("Попередження"),
                             QString::fromUtf8(e.what()));
    } catch (const PatientSystemException& e) {
        QMessageBox::critical(this, QString::fromUtf8("Помилка"),
                              QString::fromUtf8(e.what()));
    }
}

void MainWindow::onOldestWomenSameName() {
    try {
        if (patients->length() == 0) {
            throw EmptyListException("пошук найстарших жінок");
        }

        ListOfPatients* result = patients->getOldestWomenWithSameNameAndNormalHemoglobin();

        if (result == nullptr || result->length() == 0) {
            if (result != nullptr) delete result;
            throw NoResultsException("найстарші жінки з однаковим ім'ям та нормальним гемоглобіном");
        }

        showResultDialog(result, "Найстарші жінки з однаковим ім'ям та нормальним Hb");
        delete result;

    } catch (const NoResultsException& e) {
        QMessageBox::information(this, QString::fromUtf8("Інформація"),
                                 QString::fromUtf8(e.what()));
    } catch (const EmptyListException& e) {
        QMessageBox::warning(this, QString::fromUtf8("Попередження"),
                             QString::fromUtf8(e.what()));
    } catch (const PatientSystemException& e) {
        QMessageBox::critical(this, QString::fromUtf8("Помилка"),
                              QString::fromUtf8(e.what()));
    }
}

void MainWindow::onPeopleUnder28() {
    try {
        if (patients->length() == 0) {
            throw EmptyListException("пошук пацієнтів молодше 28 років");
        }

        ListOfPatients* result = patients->getPeopleYoungerThan28WithNormalT();

        if (result == nullptr || result->length() == 0) {
            if (result != nullptr) delete result;
            throw NoResultsException("пацієнти молодші 28 років з нормальною t");
        }

        // Виводимо повідомлення для кожного
        for(int i = 0; i < result->length(); i++) {
            std::string surname = result->operator[](i).getSurname();
            std::string message = "---" + surname + "--- так триматись!";
            std::cout << message << std::endl;
        }
        for(int i = 0; i < result->length(); i++){
            QMessageBox::information(this, QString::fromUtf8("Інформація"),
                                     QString::fromUtf8((*result)[i].getSurname() + ", так триматися!"));
        }
        delete result;

    } catch (const NoResultsException& e) {
        QMessageBox::information(this, QString::fromUtf8("Інформація"),
                                 QString::fromUtf8(e.what()));
    } catch (const EmptyListException& e) {
        QMessageBox::warning(this, QString::fromUtf8("Попередження"),
                             QString::fromUtf8(e.what()));
    } catch (const PatientSystemException& e) {
        QMessageBox::critical(this, QString::fromUtf8("Помилка"),
                              QString::fromUtf8(e.what()));
    }
}

void MainWindow::onLoadFromFile() {
    QString qfilename = QFileDialog::getOpenFileName(this,
                                                     QString::fromUtf8("Відкрити файл"),
                                                     "",
                                                     "Файли даних (*.txt *.dat);;Всі файли (*)");

    if (qfilename.isEmpty()) return;

    try {
        std::filesystem::path path = qfilename.toStdWString();

        // Перевірка існування файлу
        if (!std::filesystem::exists(path)) {
            throw FileNotFoundException(qfilename.toStdString());
        }

        // Перевірка типу файлу
        if (!std::filesystem::is_regular_file(path)) {
            throw InvalidDataException("це не звичайний файл");
        }

        std::ifstream file(path);

        // Перевірка відкриття файлу
        if (!file.is_open()) {
            throw FileOpenException(qfilename.toStdString());
        }

        // Перевірка на порожній файл
        file.seekg(0, std::ios::end);
        if (file.tellg() == 0) {
            throw EmptyFileException(qfilename.toStdString());
        }
        file.seekg(0, std::ios::beg);

        // Очищення попередніх даних та завантаження
        patients->reset();
        file >> (*patients);

        // Перевірка чи є дані після завантаження
        if (patients->length() == 0) {
            throw EmptyFileException(qfilename.toStdString());
        }

        file.close();
        updateMainTable();

        QMessageBox::information(this,
                                 QString::fromUtf8("Завантаження"),
                                 QString::fromUtf8("Файл успішно завантажено.\nЗавантажено записів: ") +
                                     QString::number(patients->length()));

    } catch (const PatientSystemException& e) {
        QMessageBox::critical(this,
                              QString::fromUtf8("Помилка"),
                              QString::fromUtf8(e.what()));
    }
}

void MainWindow::onLoadAndAppendFromFile() {
    QString qfilename = QFileDialog::getOpenFileName(this,
                                                     QString::fromUtf8("Додати з файлу"),
                                                     "",
                                                     "Файли даних (*.txt *.dat);;Всі файли (*)");

    if (qfilename.isEmpty()) return;

    try {
        std::filesystem::path path = qfilename.toStdWString();

        if (!std::filesystem::exists(path)) {
            throw FileNotFoundException(qfilename.toStdString());
        }

        if (!std::filesystem::is_regular_file(path)) {
            throw InvalidDataException("це не звичайний файл");
        }

        std::ifstream file(path);

        if (!file.is_open()) {
            throw FileOpenException(qfilename.toStdString());
        }

        file.seekg(0, std::ios::end);
        if (file.tellg() == 0) {
            throw EmptyFileException(qfilename.toStdString());
        }
        file.seekg(0, std::ios::beg);

        int oldLength = patients->length();

        file >> (*patients);

        int newRecords = patients->length() - oldLength;

        if (newRecords == 0) {
            throw InvalidDataException("файл не містить коректних записів");
        }

        file.close();
        updateMainTable();

        QMessageBox::information(this,
                                 QString::fromUtf8("Завантаження"),
                                 QString::fromUtf8("Дані успішно додано.\nДодано записів: ") +
                                     QString::number(newRecords));

    } catch (const PatientSystemException& e) {
        QMessageBox::critical(this,
                              QString::fromUtf8("Помилка"),
                              QString::fromUtf8(e.what()));
    }
}

void MainWindow::onSaveToFile() {
    try {
        if (patients->length() == 0) {
            throw EmptyListException("збереження у файл");
        }

        QString qfilename = QFileDialog::getSaveFileName(this,
                                                         QString::fromUtf8("Зберегти файл"),
                                                         "",
                                                         "Файли даних (*.txt *.dat);;Всі файли (*)");

        if (qfilename.isEmpty()) return;

        std::filesystem::path path = qfilename.toStdWString();

        // Перевірка існування батьківської директорії
        std::filesystem::path parentPath = path.parent_path();
        if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
            throw InvalidDataException("директорія не існує: " + parentPath.string());
        }

        std::ofstream file(path);

        if (!file.is_open()) {
            throw FileOpenException(qfilename.toStdString());
        }

        if (!file.good()) {
            throw StreamException("запис у файл (потік у поганому стані)");
        }

        file << *patients;

        if (file.fail()) {
            throw FileWriteException(qfilename.toStdString());
        }

        file.close();

        QMessageBox::information(this,
                                 QString::fromUtf8("Збереження"),
                                 QString::fromUtf8("Файл успішно збережено.\nЗбережено записів: ") +
                                     QString::number(patients->length()));

    } catch (const PatientSystemException& e) {
        QMessageBox::critical(this,
                              QString::fromUtf8("Помилка"),
                              QString::fromUtf8(e.what()));
    }
}
