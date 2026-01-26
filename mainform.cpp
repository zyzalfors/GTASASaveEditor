#include "mainform.h"
#include "ui_mainform.h"
#include <QFileDialog>
#include <QMessageBox>

MainForm::MainForm(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainForm) {
    this->ui->setupUi(this);
    this->setFixedSize(this->size().width(), this->size().height());
    this->AddSignalSlots();
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
    std::map<std::string, float> floats;
    this->save->GetInfos(path, bools, bytes, ints, floats);

    QCheckBox* checks[14];
    QPlainTextEdit* texts[31];
    this->GetWidgets(checks, texts);

    for(QCheckBox* check : checks) {
        check->setEnabled(true);
        check->setChecked(bools[check->text().toStdString()]);
    }

    const QString title = texts[30]->documentTitle();
    texts[30]->setPlainText(QString::fromStdString(path));
    texts[30]->setDocumentTitle(title);

    for(std::size_t i = 0; i < 30; i++) {
        QPlainTextEdit* text = texts[i];
        text->setEnabled(true);

        const QString title = text->documentTitle();
        const std::string name = title.toStdString();

        if(bytes.count(name) > 0) text->setPlainText(QString::number(bytes[name]));
        else if(ints.count(name) > 0) text->setPlainText(QString::number(ints[name]));
        else if(floats.count(name) > 0) text->setPlainText(QString::number(floats[name]));

        text->setDocumentTitle(title);
    }
}

void MainForm::GetWidgets(QCheckBox* checks[], QPlainTextEdit* texts[]) {
    checks[0] = this->ui->check1;
    checks[1] = this->ui->check2;
    checks[2] = this->ui->check3;
    checks[3] = this->ui->check4;
    checks[4] = this->ui->check5;
    checks[5] = this->ui->check6;
    checks[6] = this->ui->check7;
    checks[7] = this->ui->check8;
    checks[8] = this->ui->check9;
    checks[9] = this->ui->check10;
    checks[10] = this->ui->check11;
    checks[11] = this->ui->check12;
    checks[12] = this->ui->check13;
    checks[13] = this->ui->check14;

    texts[30] = this->ui->text0;
    texts[0] = this->ui->text1;
    texts[1] = this->ui->text2;
    texts[2] = this->ui->text3;
    texts[3] = this->ui->text4;
    texts[4] = this->ui->text5;
    texts[5] = this->ui->text6;
    texts[6] = this->ui->text7;
    texts[7] = this->ui->text8;
    texts[8] = this->ui->text9;
    texts[9] = this->ui->text10;
    texts[10] = this->ui->text11;
    texts[11] = this->ui->text12;
    texts[12] = this->ui->text13;
    texts[13] = this->ui->text14;
    texts[14] = this->ui->text15;
    texts[15] = this->ui->text16;
    texts[16] = this->ui->text17;
    texts[17] = this->ui->text18;
    texts[18] = this->ui->text19;
    texts[19] = this->ui->text20;
    texts[20] = this->ui->text21;
    texts[21] = this->ui->text22;
    texts[22] = this->ui->text23;
    texts[23] = this->ui->text24;
    texts[24] = this->ui->text25;
    texts[25] = this->ui->text26;
    texts[26] = this->ui->text27;
    texts[27] = this->ui->text28;
    texts[28] = this->ui->text29;
    texts[29] = this->ui->text30;    
}

void MainForm::OpenSave() {
    QString p = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open Save"), "/home/user", QObject::tr("GTASA Save (*.b);;All Files (*)"));
    if(p.isEmpty()) return;
    std::string path = p.toStdString();

    if(this->save != nullptr) delete this->save;

    try {
        this->save = new GTASASave(path);
        if(!this->save->CheckChecksum()) throw std::runtime_error("Invalid checksum.");
    }
    catch(const std::runtime_error& e) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(e.what());
        msgBox.exec();
    }

    if(this->save == nullptr) return;
    this->PrintSaveInfos();
    this->ui->Update->setEnabled(true);
}

bool MainForm::Update(const std::string& name, const std::string& val) {
    try {
         this->save->Update(name, val);
    }
    catch(...) {
        return false;
    }

    return true;
}

void MainForm::UpdateSave() {
    if(this->save == nullptr) return;

    QCheckBox* checks[14];
    QPlainTextEdit* texts[31];
    this->GetWidgets(checks, texts);

    for(QCheckBox* check : checks) {
        const std::string name = check->text().toStdString();
        const std::string val = check->isChecked() ? "1" : "0";
        this->Update(name, val);
    }

    std::string msg = "Save updated.";
    bool ok = false;

    for(std::size_t i = 0; i < 30; i++) {
        const QPlainTextEdit* text = texts[i];
        const std::string name = text->documentTitle().toStdString();
        const std::string val = text->toPlainText().toStdString();

        ok = this->Update(name, val);    
        if(!ok) {
            msg = "Error occurred in updating " + name + ".";
            break;
        }
    }

    if(ok) {
        this->save->UpdateChecksum();
        this->save->Write();
    }

    QMessageBox msgBox;
    msgBox.setIcon(ok ? QMessageBox::Information : QMessageBox::Critical);
    msgBox.setText(QString::fromUtf8(msg.data()));
    msgBox.exec();
}
