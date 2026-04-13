#include "hlavicka.h"
#include <QMessageBox>
#include <QStyle>

AlkoKalkulacka::AlkoKalkulacka(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Alkohol Kalkulačka");
    setMinimumSize(650, 600);
    setupUI();
}

void AlkoKalkulacka::setupUI() {
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *rootLayout = new QVBoxLayout(central);

    // Horní Titulek
    QLabel *title = new QLabel("Alkohol Kalkulačka");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("background-color: #E6DFEF; color: #333333; font: bold 16px; padding: 10px; border-radius: 10px;");
    rootLayout->addWidget(title);

    pages = new QStackedWidget();
    rootLayout->addWidget(pages);

    // --- STRÁNKA 1: KALKULAČKA ---
    QWidget *mainPage = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(mainPage);

    // Levý sloupec
    QVBoxLayout *leftCol = new QVBoxLayout();
    leftCol->addWidget(createTitleBox("Pohlaví"));
    radioMuz = new QRadioButton("Muž"); radioMuz->setChecked(true);
    radioZena = new QRadioButton("Žena");
    QHBoxLayout *gLayout = new QHBoxLayout();
    gLayout->addWidget(radioMuz); gLayout->addWidget(radioZena);
    leftCol->addLayout(gLayout);

    leftCol->addSpacing(20);
    leftCol->addWidget(createTitleBox("Váha"));
    weightEntry = new QLineEdit("75"); weightEntry->setReadOnly(true);
    weightSlider = new QSlider(Qt::Horizontal);
    weightSlider->setRange(30, 150); weightSlider->setValue(75);
    leftCol->addWidget(weightEntry);
    leftCol->addWidget(weightSlider);
    leftCol->addStretch();

    // Pravý sloupec
    QVBoxLayout *rightCol = new QVBoxLayout();
    rightCol->addWidget(createTitleBox("Vypité nápoje"));
    
    QHBoxLayout *addDrinkLayout = new QHBoxLayout();
    comboType = new QComboBox(); comboType->addItems({"Pivo", "Víno", "Destilát"});
    comboPct = new QComboBox(); comboPct->addItems({"5%", "10%", "12%", "40%"});
    addDrinkLayout->addWidget(comboType); addDrinkLayout->addWidget(comboPct);
    rightCol->addLayout(addDrinkLayout);

    QPushButton *btnAdd = new QPushButton("Přidat");
    btnAdd->setStyleSheet("background-color: #D1BFFF; color: white; font-weight: bold; padding: 5px;");
    rightCol->addWidget(btnAdd);

    QScrollArea *scroll = new QScrollArea();
    QWidget *scrollContent = new QWidget();
    scrollLayout = new QVBoxLayout(scrollContent);
    scroll->setWidget(scrollContent);
    scroll->setWidgetResizable(true);
    rightCol->addWidget(scroll);

    mainLayout->addLayout(leftCol, 1);
    mainLayout->addLayout(rightCol, 1);
    pages->addWidget(mainPage);

    // --- STRÁNKA 2: NÁPOVĚDA ---
    QWidget *helpPage = new QWidget();
    QVBoxLayout *helpLayout = new QVBoxLayout(helpPage);
    QLabel *helpText = new QLabel("Widmarkův vzorec:\npromile = alkohol(g) / (váha(kg) * faktor)\n\nFaktor muž: 0.7\nFaktor žena: 0.6");
    helpText->setAlignment(Qt::AlignCenter);
    helpLayout->addWidget(helpText);
    QPushButton *btnBack = new QPushButton("Zpět");
    helpLayout->addWidget(btnBack);
    pages->addWidget(helpPage);

    // Spodní tlačítka
    QPushButton *btnCalc = new QPushButton("Vypočítat stav pijáka");
    btnCalc->setStyleSheet("background-color: #52438A; color: white; font: bold 18px; padding: 15px; border-radius: 25px;");
    rootLayout->addWidget(btnCalc);

    QHBoxLayout *bottomNav = new QHBoxLayout();
    QPushButton *btnHist = new QPushButton("Historie");
    QPushButton *btnHelp = new QPushButton("Nápověda");
    bottomNav->addWidget(btnHist);
    bottomNav->addStretch();
    bottomNav->addWidget(btnHelp);
    rootLayout->addLayout(bottomNav);

    // Propojení (Signals & Slots)
    connect(weightSlider, &QSlider::valueChanged, this, &AlkoKalkulacka::updateWeightLabel);
    connect(btnAdd, &QPushButton::clicked, this, &AlkoKalkulacka::addDrink);
    connect(btnCalc, &QPushButton::clicked, this, &AlkoKalkulacka::calculateStatus);
    connect(btnHelp, &QPushButton::clicked, this, &AlkoKalkulacka::showHelp);
    connect(btnBack, &QPushButton::clicked, this, &AlkoKalkulacka::showMain);
    connect(btnHist, &QPushButton::clicked, this, &AlkoKalkulacka::showHistory);
}

QWidget* AlkoKalkulacka::createTitleBox(QString text) {
    QLabel *l = new QLabel(text);
    l->setStyleSheet("border: 1px solid #D1BFFF; font-weight: bold; padding: 5px; border-radius: 5px;");
    l->setAlignment(Qt::AlignCenter);
    return l;
}

void AlkoKalkulacka::updateWeightLabel(int val) {
    weightEntry->setText(QString::number(val));
}

void AlkoKalkulacka::addDrink() {
    createDrinkRow(comboType->currentText() + " " + comboPct->currentText(), 
                   comboType->currentText() == "Pivo" ? "0.5" : "0.2");
}

void AlkoKalkulacka::createDrinkRow(QString name, QString size) {
    QWidget *row = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(row);
    
    QLabel *lbl = new QLabel(name);
    QLineEdit *edit = new QLineEdit(size);
    QComboBox *mult = new QComboBox(); mult->addItems({"", "2x", "3x"});
    QCheckBox *check = new QCheckBox(); check->setChecked(true);

    layout->addWidget(lbl);
    layout->addWidget(edit);
    layout->addWidget(mult);
    layout->addWidget(check);

    scrollLayout->insertWidget(0, row); // Přidá na začátek seznamu
    drinksList.append({row, name, edit, mult, check});
}

void AlkoKalkulacka::calculateStatus() {
    double weight = weightEntry->text().toDouble();
    double r = radioMuz->isChecked() ? 0.7 : 0.6;
    double alcoholG = 0.0;

    for (const auto& d : drinksList) {
        if (d.checkbox->isChecked()) {
            double pct = d.name.split(" ").last().replace("%", "").toDouble() / 100.0;
            double volumeMl = d.sizeEntry->text().toDouble() * 1000.0;
            int m = d.multiplierCombo->currentText().replace("x", "").toInt();
            if (m == 0) m = 1;
            alcoholG += volumeMl * pct * 0.8 * m;
        }
    }

    double promile = alcoholG / (weight * r);
    QMessageBox::information(this, "Výsledek", QString("Odhadovaná hladina: %1 ‰").arg(QString::number(promile, 'f', 2)));
}

void AlkoKalkulacka::showHistory() {
    QString hist = "Seznam nápojů:\n";
    for(const auto& d : drinksList) {
        hist += QString("- %1 (%2l)\n").arg(d.name, d.sizeEntry->text());
    }
    QMessageBox::about(this, "Historie", hist.isEmpty() ? "Prázdno." : hist);
}