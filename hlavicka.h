//
// Created by tucnak on 8.4.26.
//

#ifndef HLAVICKA_H
#define HLAVICKA_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QLineEdit>
#include <QComboBox>
#include <QScrollArea>
#include <QStackedWidget>
#include <QCheckBox>
#include <QList>

// Struktura pro uchování nápoje v seznamu
struct DrinkRow {
    QWidget* widget;
    QString name;
    QLineEdit* sizeEntry;
    QComboBox* multiplierCombo;
    QCheckBox* checkbox;
};

class AlkoKalkulacka : public QMainWindow {
    Q_OBJECT

public:
    AlkoKalkulacka(QWidget *parent = nullptr);

    private slots:
        void addDrink();
    void calculateStatus();
    void showHistory();
    void showHelp() { pages->setCurrentIndex(1); }
    void showMain() { pages->setCurrentIndex(0); }
    void updateWeightLabel(int val);

private:
    void setupUI();
    QWidget* createTitleBox(QString text);
    void createDrinkRow(QString name, QString size);

    QStackedWidget *pages;
    QList<DrinkRow> drinksList;

    // UI prvky pro logiku
    QRadioButton *radioMuz, *radioZena;
    QSlider *weightSlider;
    QLineEdit *weightEntry;
    QComboBox *comboType, *comboPct;
    QVBoxLayout *scrollLayout;
};

#endif //HLAVICKA_H
