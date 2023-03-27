#ifndef CURRENTDAY_H
#define CURRENTDAY_H

#include <QMainWindow>
#include "notesclass.h"

namespace Ui {
class CurrentDay;
}

class CurrentDay : public QMainWindow
{
    Q_OBJECT

public:
    explicit CurrentDay(std::vector<NotesClass> *n, QWidget *parent = nullptr);
    ~CurrentDay();

private slots:
    void clickAddButton();
    void dataUpSlot();
    void clickCellItem(int row, int);

private:
    Ui::CurrentDay *ui;
    std::vector<NotesClass> *notes;
    QDate curDate;
};

#endif // CURRENTDAY_H
