#ifndef NOTES_H
#define NOTES_H

#include <QMainWindow>
#include "notesclass.h"

namespace Ui {
class Notes;
}

class Notes : public QMainWindow
{
    Q_OBJECT

public:
    explicit Notes(std::vector<NotesClass> *n, std::vector<QTime> currT, bool isN, QWidget *parent = nullptr);
    ~Notes();
    void takeCurNote(NotesClass *n);

signals:
    void dataUp();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void clickSaveButton();
    void clickDelButton();
    void changeData();

private:
    Ui::Notes *ui;
    std::vector<NotesClass> *currNotes;
    std::vector<QTime> currTime;
    bool isNew;
};

#endif // NOTES_H
