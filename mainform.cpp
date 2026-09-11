#include "mainform.h"
#include "ui_mainform.h"
#include <QFileDialog>
#include <QMessageBox>

MainForm::MainForm(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainForm) {
    this->ui->setupUi(this);
    this->setFixedSize(this->size().width(), this->size().height());
    this->AddSignalSlots();
}

void MainForm::InitCombos() {
    QWidget* tab = ui->tabs->widget(1);

    for(QComboBox* combo : tab->findChildren<QComboBox*>()) {
        const int i = combo->objectName().mid(5).toInt();
        combo->clear();

        for(const auto& entry : this->save->weapons[i])
            combo->addItem(QString::fromStdString(entry.first));

        combo->setEnabled(true);
    }
}

MainForm::~MainForm() {
    delete this->ui;
    if(this->save != nullptr) delete this->save;
}

void MainForm::AddSignalSlots() {
    connect(this->ui->Open, &QAction::triggered, this, &MainForm::OpenSave);
    connect(this->ui->Update, &QAction::triggered, this, &MainForm::UpdateSave);
}

void MainForm::PrintSaveInfos() {
    std::string path;
    std::map<std::string, bool> bools;
    std::map<std::string, std::uint8_t> bytes;
    std::map<std::string, std::uint32_t> ints;
    std::map<std::string, float> decs;
    std::array<std::pair<std::string, std::uint32_t>, this->save->weaponSlots> weaps;
    this->save->GetInfos(path, bools, bytes, ints, decs, weaps);

    QCheckBox* checks[16] = {};
    QPlainTextEdit* texts[39] = {};
    QComboBox* combos[13] = {};
    this->GetWidgets(checks, texts, combos);

    for(QCheckBox* check : checks) {
        check->setEnabled(true);
        check->setChecked(bools[check->text().toStdString()]);
    }

    const QString title = texts[0]->documentTitle();
    texts[0]->setPlainText(QString::fromStdString(path));
    texts[0]->setDocumentTitle(title);

    for(std::size_t i = 1; i < 31; i++) {
        QPlainTextEdit* text = texts[i];
        text->setEnabled(true);

        const QString title = text->documentTitle();
        const std::string name = title.toStdString();

        if(bytes.count(name) > 0) text->setPlainText(QString::number(bytes[name]));
        else if(ints.count(name) > 0) text->setPlainText(QString::number(ints[name]));
        else if(decs.count(name) > 0) text->setPlainText(QString::number(decs[name]));

        text->setDocumentTitle(title);
    }

    for(QComboBox* combo : combos) {
        const int i = combo->objectName().mid(5).toInt();
        combo->setCurrentIndex(combo->findText(QString::fromStdString(weaps[i].first)));
    }

    for(std::size_t i = 31; i < 39; i++) {
        QPlainTextEdit* text = texts[i];
        text->setEnabled(true);

        const QString title = text->documentTitle();
        text->setPlainText(QString::number(weaps[i - 31 + 2].second));
        text->setDocumentTitle(title);
    }
}

void MainForm::GetWidgets(QCheckBox* checks[], QPlainTextEdit* texts[], QComboBox* combos[]) {
    QWidget* tab1 = ui->tabs->widget(0);

    for(QCheckBox* check : tab1->findChildren<QCheckBox*>()) {
        if(check->text() == "Cheated") checks[0] = check;
        else if(check->text() == "Riot") checks[1] = check;
        else if(check->text() == "Uncensored") checks[2] = check;
        else if(check->text() == "Taxi nitro") checks[3] = check;
        else if(check->text() == "Prostitutes pay") checks[4] = check;
        else if(check->text() == "Free bomb") checks[5] = check;
        else if(check->text() == "Free respray") checks[6] = check;
        else if(check->text() == "Respray disabled") checks[7] = check;
        else if(check->text() == "Lose stuff on wasted") checks[8] = check;
        else if(check->text() == "Lose stuff on busted") checks[9] = check;
        else if(check->text() == "Infinite sprint") checks[10] = check;
        else if(check->text() == "Fast reload") checks[11] = check;
        else if(check->text() == "Fireproof") checks[12] = check;
        else if(check->text() == "Drive-by") checks[13] = check;
        else if(check->text() == "Roadblocked SF") checks[14] = check;
        else if(check->text() == "Roadblocked LV") checks[15] = check;
    }

    for(QPlainTextEdit* text : tab1->findChildren<QPlainTextEdit*>()) {
        if(text->documentTitle() == "Path") texts[0] = text;
        else if(text->documentTitle() == "Version") texts[1] = text;
        else if(text->documentTitle() == "Health") texts[2] = text;
        else if(text->documentTitle() == "Max health") texts[3] = text;
        else if(text->documentTitle() == "Armor") texts[4] = text;
        else if(text->documentTitle() == "Max armor") texts[5] = text;
        else if(text->documentTitle() == "Money") texts[6] = text;
        else if(text->documentTitle() == "Money on screen") texts[7] = text;
        else if(text->documentTitle() == "Fat") texts[8] = text;
        else if(text->documentTitle() == "Stamina") texts[9] = text;
        else if(text->documentTitle() == "Muscle") texts[10] = text;
        else if(text->documentTitle() == "Respect") texts[11] = text;
        else if(text->documentTitle() == "Sex appeal") texts[12] = text;
        else if(text->documentTitle() == "Luck") texts[13] = text;
        else if(text->documentTitle() == "Pistol skill") texts[14] = text;
        else if(text->documentTitle() == "Silenced pistol skill") texts[15] = text;
        else if(text->documentTitle() == "Desert Eagle skill") texts[16] = text;
        else if(text->documentTitle() == "Shotgun skill") texts[17] = text;
        else if(text->documentTitle() == "Sawn-off shotgun skill") texts[18] = text;
        else if(text->documentTitle() == "Combat shotgun skill") texts[19] = text;
        else if(text->documentTitle() == "Machine pistol skill") texts[20] = text;
        else if(text->documentTitle() == "SMG skill") texts[21] = text;
        else if(text->documentTitle() == "AK47 skill") texts[22] = text;
        else if(text->documentTitle() == "M4 skill") texts[23] = text;
        else if(text->documentTitle() == "Gambling skill") texts[24] = text;
        else if(text->documentTitle() == "Driving skill") texts[25] = text;
        else if(text->documentTitle() == "Flying skill") texts[26] = text;
        else if(text->documentTitle() == "Lung capacity") texts[27] = text;
        else if(text->documentTitle() == "Bike skill") texts[28] = text;
        else if(text->documentTitle() == "Cycling skill") texts[29] = text;
        else if(text->documentTitle() == "Cheats used") texts[30] = text;
    }

    QWidget* tab2 = ui->tabs->widget(1);

    for(QComboBox* combo : tab2->findChildren<QComboBox*>()) {
        const int i = combo->objectName().mid(5).toInt();
        combos[i] = combo;
    }

    for(QPlainTextEdit* text : tab2->findChildren<QPlainTextEdit*>()) {
        if(text->documentTitle() == "Slot2 Ammo") texts[31] = text;
        else if(text->documentTitle() == "Slot3 Ammo") texts[32] = text;
        else if(text->documentTitle() == "Slot4 Ammo") texts[33] = text;
        else if(text->documentTitle() == "Slot5 Ammo") texts[34] = text;
        else if(text->documentTitle() == "Slot6 Ammo") texts[35] = text;
        else if(text->documentTitle() == "Slot7 Ammo") texts[36] = text;
        else if(text->documentTitle() == "Slot8 Ammo") texts[37] = text;
        else if(text->documentTitle() == "Slot9 Ammo") texts[38] = text;
    }
}

void MainForm::OpenSave() {
    QString p = QFileDialog::getOpenFileName(this, QObject::tr("Open Save"), QString(), QObject::tr("GTASA Save (*.b);;All Files (*)"));
    if(p.isEmpty()) return;
    std::string path = p.toStdString();

    bool ok = true;
    if(this->save != nullptr) delete this->save;

    try {
        this->save = new GTASASave(path);

        if(!this->save->ValidChecksum()) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("Invalid save checksum. Update save to fix it");
            msgBox.exec();
        }
    }
    catch(const std::runtime_error& e) {
        ok = false;
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(e.what());
        msgBox.exec();
    }

    if(!ok) return;

    this->InitCombos();
    this->PrintSaveInfos();
    this->ui->Update->setEnabled(true);
}

void MainForm::UpdateSave() {
    if(this->save == nullptr) return;

    QCheckBox* checks[16] = {};
    QPlainTextEdit* texts[39] = {};
    QComboBox* combos[13] = {};
    this->GetWidgets(checks, texts, combos);

    bool ok = true;
    std::string msg = "Save updated.";

    try {
        for(QCheckBox* check : checks) {
            const std::string name = check->text().toStdString();
            const std::string val = check->isChecked() ? "1" : "0";
            this->save->UpdateValue(name, val);
        }

        for(std::size_t i = 1; i < 31; i++) {
            const QPlainTextEdit* text = texts[i];
            const std::string name = text->documentTitle().toStdString();
            const std::string val = text->toPlainText().toStdString();
            this->save->UpdateValue(name, val);
        }

        std::array<std::pair<std::string, std::uint32_t>, this->save->weaponSlots> weaps = {};

        int i = 0;
        for(QComboBox* combo : combos)
            weaps[i++].first = combo->currentText().toStdString();

        for(std::size_t i = 31; i < 39; i++)
            weaps[i - 31 + 2].second = std::stoul(texts[i]->toPlainText().toStdString());

        this->save->UpdateWeapons(weaps);
        this->save->Write();
    }
    catch(const std::exception& e) {
        ok = false;
        msg = e.what();
    }

    QMessageBox msgBox;
    msgBox.setIcon(ok ? QMessageBox::Information : QMessageBox::Critical);
    msgBox.setText(QString::fromStdString(msg));
    msgBox.exec();
}