#ifndef ORGMAIN_H
#define ORGMAIN_H

#include <QMainWindow>
#include <QDate>
#include "notesclass.h"

QT_BEGIN_NAMESPACE
namespace Ui { class OrgMain; }
QT_END_NAMESPACE

class OrgMain : public QMainWindow
{
    Q_OBJECT

public:
    OrgMain(QWidget *parent = nullptr);
    ~OrgMain();

private slots:
    void clickedDay(QDate date);
    void clickDeleteButton();

private:
    Ui::OrgMain *ui;
    std::map<std::string, std::vector<NotesClass>> notes;
};
#endif // ORGMAIN_H
