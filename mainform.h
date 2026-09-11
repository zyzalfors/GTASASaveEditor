#ifndef MAINFORM_H
#define MAINFORM_H
#include <QMainWindow>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QComboBox>
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
    private: Ui::MainForm* ui;
    private: void Clear();
    private: void AddSignalSlots();
    private: void PrintSaveInfos(const bool& clear);
    private: void GetWidgets(QCheckBox* checks[], QPlainTextEdit* texts[], QComboBox* combos[]);
    private: GTASASave* save = nullptr;
};
#endif
