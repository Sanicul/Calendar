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
    explicit CurrentDay(std::map<std::string, std::vector<NotesClass>> *d, std::string k, QWidget *parent = nullptr);
    ~CurrentDay();

signals:
    void dataChanged();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void clickAddButton();
    void dataUpSlot();
    void clickCellItem(int row, int);

private:
    Ui::CurrentDay *ui;
    std::map<std::string, std::vector<NotesClass>> *data;
    std::vector<NotesClass> *notes;
    std::string key;
};

#endif // CURRENTDAY_H
