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
    if(this->save) delete this->save;
}

void MainForm::AddSignalSlots() {
    connect(this->ui->Open, &QAction::triggered, this, &MainForm::OpenSave);
    connect(this->ui->Update, &QAction::triggered, this, &MainForm::UpdateSave);
}

void MainForm::PrintSaveInfos() {
    if(!this->save) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Unable to open save.");
        msgBox.exec();
        return;
    }

    std::map<std::string, bool> _bools;
    std::map<std::string, uint8_t> _bytes;
    std::map<std::string, uint32_t> _ints;
    std::map<std::string, float> _floats;
    this->save->GetInfos(_bools, _bytes, _ints, _floats);

    QCheckBox* checks[14];
    QPlainTextEdit* texts[30];
    this->GetWidgets(checks, texts);

    for(QCheckBox* check : checks) {
        check->setEnabled(true);
        check->setChecked(_bools[check->text().toStdString()]);
    }

    for(QPlainTextEdit* text : texts) {
        text->setEnabled(true);
        QString title = text->documentTitle();
        std::string name = title.toStdString();

        if(_bytes.count(name) > 0) text->setPlainText(QString::number(_bytes[name]));
        else if(_ints.count(name) > 0) text->setPlainText(QString::number(_ints[name]));
        else if(_floats.count(name) > 0) text->setPlainText(QString::number(_floats[name]));
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
    QString _path = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open Save"), "/home/user", QObject::tr("GTA SA Save (*.b)"));
    if(_path.isEmpty()) return;
    std::string path = _path.toStdString();

    if(this->save) delete this->save;
    this->save = new GTASASave(path);
    this->PrintSaveInfos();
    this->ui->Update->setEnabled(true);
}

bool MainForm::Update(std::string& _name, std::string& _val) {
    try {
         this->save->Update(_name, _val);
    }
    catch(...) {
        return false;
    }
    return true;
}

void MainForm::UpdateSave() {
    if(!this->save) return;

    QCheckBox* checks[14];
    QPlainTextEdit* texts[30];
    this->GetWidgets(checks, texts);

    for(QCheckBox* check : checks) {
        std::string name = check->text().toStdString();
        std::string checked = check->isChecked() ? "1" : "0";
        this->Update(name, checked);
    }

    std::string msg = "Save updated.";
    bool ok = false;

    for(QPlainTextEdit* text : texts) {
        std::string name = text->documentTitle().toStdString();
        std::string val = text->toPlainText().toStdString();
        ok = this->Update(name, val);
        if(!ok) {
            msg = "Error occured in updating " + name + ".";
            break;
        }
    }

    if(ok) {
        this->save->UpdateChecksum();
        this->save->Write();
    }

    QMessageBox msgBox;
    if(ok) msgBox.setIcon(QMessageBox::Information);
    else msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(QString::fromUtf8(msg.c_str()));
    msgBox.exec();
}
