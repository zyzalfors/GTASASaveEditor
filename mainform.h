#ifndef MAINFORM_H
#define MAINFORM_H
#include <QMainWindow>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPlainTextEdit>
#include "GTASASave.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainForm;
}
QT_END_NAMESPACE

class MainForm : public QMainWindow {
    Q_OBJECT
    public: MainForm(QWidget* parent = nullptr);
    public: ~MainForm();
    private slots: void OpenSave();    
    private slots: void UpdateSave();
    private: bool Update(std::string& _name, std::string& _val);
    private: Ui::MainForm* ui;
    private: void AddSignalSlots();
    private: void PrintSaveInfos();
    private: void GetWidgets(QCheckBox* checks[], QPlainTextEdit* texts[]);
    private: GTASASave* save = nullptr;
};
#endif
