#include "currentday.h"
#include "ui_currentday.h"
#include <fstream>
#include "notes.h"

CurrentDay::CurrentDay(std::map<std::string, std::vector<NotesClass>> *d, std::string k, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CurrentDay)
{
    ui->setupUi(this);
    data = d;
    key = k;
    notes = &((*data)[key]);
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(clickAddButton()));
    connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(clickCellItem(int, int)));
    dataUpSlot();
}

CurrentDay::~CurrentDay()
{
    delete ui;
}

void CurrentDay::closeEvent(QCloseEvent *event)
{
    emit dataChanged();
}

void CurrentDay::clickAddButton()
{
    Notes *n = new Notes(notes, std::vector<QTime>(2, QTime()), true, this);
    n->setWindowTitle(this->windowTitle());
    connect(n, SIGNAL(dataUp()), this, SLOT(dataUpSlot()));
    n->show();
}

void CurrentDay::dataUpSlot()
{
    std::sort(notes->begin(), notes->end(), [](NotesClass a, NotesClass b)
    {
        return (a.start <= b.start);
    });
    ui->tableWidget->setRowCount(notes->size());
    for(unsigned long long i = 0; i < notes->size(); i++)
    {
        QTableWidgetItem *tblN = new QTableWidgetItem(QString::fromStdString((*notes)[i].name.c_str()));
        QTableWidgetItem *tblT = new QTableWidgetItem((*notes)[i].start.toString()+"-"+(*notes)[i].stop.toString());
        ui->tableWidget->setItem(i, 0, tblN);
        ui->tableWidget->setItem(i, 1, tblT);
    }
}

void CurrentDay::clickCellItem(int row, int)
{
    std::vector<QTime> curr;
    curr.push_back((notes->begin()+row)->start);
    curr.push_back((notes->begin()+row)->stop);
    Notes *n = new Notes(notes, curr, false, this);
    n->setWindowTitle(this->windowTitle());
    connect(n, SIGNAL(dataUp()), this, SLOT(dataUpSlot()));
    n->show();
}
