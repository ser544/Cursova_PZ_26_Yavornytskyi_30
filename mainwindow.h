#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include "listofpatients.h"
#include "person.h"
#include <fstream>
#include <string>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Слоти для кнопок
    void onAddPatient();
    void onEditPatient();
    void onDeletePatient();
    void onSortByT();
    void onWomenHighHemoglobin();
    void onYoungestMan();
    void onPeopleOver40();
    void onOldestWomenSameName();
    void onPeopleUnder28();
    void onLoadFromFile();
    void onLoadAndAppendFromFile();
    void onSaveToFile();
    void onClearAll();

private:
    // Головна таблиця
    QTableWidget* mainTable;

    // Кнопки
    QPushButton* btnAddPatient;
    QPushButton* btnEditPatient;
    QPushButton* btnDeletePatient;
    QPushButton* btnSortByT;
    QPushButton* btnWomenHighHb;
    QPushButton* btnYoungestMan;
    QPushButton* btnPeopleOver40;
    QPushButton* btnOldestWomen;
    QPushButton* btnPeopleUnder28;
    QPushButton* btnLoadFile;
    QPushButton* btnLoadAndAppendFile;
    QPushButton* btnSaveFile;
    QPushButton* btnClearAll;

    // Дані
    ListOfPatients* patients;

    // Допоміжні методи
    void setupUI();
    void setupTable();
    void setupButtons();
    void updateMainTable();
    void showResultDialog(ListOfPatients* result, const std::string& title);
};
#endif
