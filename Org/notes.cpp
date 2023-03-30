#include "notes.h"
#include "ui_notes.h"
#include <QMessageBox>

Notes::Notes(std::vector<NotesClass> *n, std::vector<QTime> currT, bool isN, QWidget *parent) :
    QMainWindow(parent)
  , ui(new Ui::Notes)
{
    ui->setupUi(this);
    currNotes = n;
    isNew = isN;
    currTime = currT;
    if(isNew)
        ui->pushButton_2->setEnabled(false);
    else
    {
        auto it = std::find_if(currNotes->begin(), currNotes->end(), [this](NotesClass a)
        {
            return (currTime[0] == a.start && currTime[1] == a.stop);
        });
        ui->lineEdit->setText(QString::fromStdString(it->name));
        ui->plainTextEdit->setPlainText(QString::fromStdString(it->description));
        ui->timeEdit->setTime(it->start);
        ui->timeEdit_2->setTime(it->stop);
    }
    ui->pushButton->setEnabled(false);
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(clickSaveButton()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(clickDelButton()));
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(changeData()));
    connect(ui->plainTextEdit, SIGNAL(textChanged()), this, SLOT(changeData()));
    connect(ui->timeEdit, SIGNAL(timeChanged(QTime)), this, SLOT(changeData()));
    connect(ui->timeEdit_2, SIGNAL(timeChanged(QTime)), this, SLOT(changeData()));
}

Notes::~Notes()
{
    delete ui;
}

void Notes::closeEvent(QCloseEvent *event)
{
    if(ui->pushButton->isEnabled())
    {
        QMessageBox::StandardButton b =
                QMessageBox::question(this, "Выход", "Есть несохраненные изменения\nВы уверены что хотите выйти?");
        if(b == QMessageBox::StandardButton::Yes)
            event->accept();
        else
            event->ignore();
    }
    else
        event->accept();
}

void Notes::clickSaveButton()
{
    NotesClass n(ui->lineEdit->text().toStdString(), ui->plainTextEdit->toPlainText().toStdString(),
                 ui->timeEdit->time(), ui->timeEdit_2->time());
    if(n.name.empty())
        QMessageBox::warning(this, "Название пусто", "Вы не указали название");
    else if(n.description.empty())
        QMessageBox::warning(this, "Описание пусто", "Вы не указали описание");
    else if(n.start > n.stop)
        QMessageBox::warning(this, "Неправильное время", "Старт позже начала");
    else
    {
        bool check = false;
        NotesClass tmp;
        if(!isNew)
        {
            auto it = std::find_if(currNotes->begin(), currNotes->end(), [this](NotesClass a)
            {
                return (currTime[0] == a.start && currTime[1] == a.stop);
            });
            tmp = *it;
            currNotes->erase(it);
        }
        for(auto it : *currNotes)
            if(((n.start >= it.start && n.start < it.stop) || (n.stop > it.start && n.stop <= it.stop))||
                    (n.start <= it.start && n.stop >= it.stop))
            {
                check = true;
                break;
            }
        if(check)
        {
            QMessageBox::warning(this, "Поменяйте время", "Время события пересекается с существующими");
            if(!isNew)
                currNotes->push_back(tmp);
        }
        else
        {
            currNotes->push_back(n);
            ui->pushButton->setEnabled(false);
        }
        emit dataUp();
        if(!check)
        {
            currTime[0] = n.start;
            currTime[1] = n.stop;
            isNew = false;
            ui->pushButton_2->setEnabled(true);
        }
        else if(!isNew)
        {
            currTime[0] = tmp.start;
            currTime[1] = tmp.stop;
            isNew = false;
            ui->pushButton_2->setEnabled(true);
        }
    }
}

void Notes::clickDelButton()
{
    QMessageBox::StandardButton b = QMessageBox::question(this, "Удаление", "Вы уверены что хотите удалить заметку?\nВосстановление невозможно");
    if(b == QMessageBox::StandardButton::Yes)
    {
        auto it = std::find_if(currNotes->begin(), currNotes->end(), [this](NotesClass a)
        {
            return (currTime[0] == a.start && currTime[1] == a.stop);
        });
        currNotes->erase(it);
        ui->pushButton_2->setEnabled(false);
        emit dataUp();
        ui->pushButton->setEnabled(false);
        this->close();
    }
}

void Notes::changeData()
{
    ui->pushButton->setEnabled(true);
}

