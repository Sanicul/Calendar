#include "orgmain.h"
#include "ui_orgmain.h"
#include "currentday.h"
#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPainter>
#include <QTextCharFormat>

OrgMain::OrgMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OrgMain)
{
    ui->setupUi(this);
    connect(ui->calendarWidget, SIGNAL(clicked(QDate)), this, SLOT(clickedDay(QDate)));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(clickDeleteButton()));
    QFile f("Data.json");
    if (f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray data = f.readAll();
        QJsonObject obj = (QJsonDocument::fromJson(data)).object();
        for(auto it = obj.begin(); it != obj.end(); it++)
        {
            std::vector<NotesClass> n;
            QJsonArray arr = it.value().toArray();
            for(auto it1 = arr.begin(); it1 != arr.end(); it1++)
            {
                QJsonObject arrEl = it1->toObject();
                NotesClass nc(arrEl["Name"].toString().toStdString(), arrEl["Description"].toString().toStdString(),
                        QTime::fromString(arrEl["TimeStart"].toString()), QTime::fromString(arrEl["TimeStop"].toString()));
                n.push_back(nc);
            }
            notes[it.key().toStdString()] = n;
        }
    }
    repaintDays();
}

OrgMain::~OrgMain()
{
    QFile f("Data.json");
    if (f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonObject obj;
        for(auto it : notes)
        {
            if(it.second.size())
            {
                QJsonArray arr;
                for(unsigned long long i = 0; i < it.second.size(); i++)
                {
                    QJsonValue name(QString::fromStdString(it.second[i].name)), description(QString::fromStdString(it.second[i].description)),
                            timeSt(it.second[i].start.toString()), timeSp(it.second[i].stop.toString());
                    QJsonObject arrEl;
                    arrEl.insert("Name", name);
                    arrEl.insert("Description", description);
                    arrEl.insert("TimeStart", timeSt);
                    arrEl.insert("TimeStop", timeSp);
                    arr.push_back(arrEl);

                }
                obj.insert(it.first.c_str(), arr);
            }
        }
        QByteArray data = QJsonDocument(obj).toJson();
        f.write(data);
    }
    delete ui;
}

void OrgMain::clickedDay(QDate date)
{
    CurrentDay *c = new CurrentDay(&notes, date.toString().toStdString(), this);
    connect(c, SIGNAL(dataChanged()), this, SLOT(repaintDays()));
    c->setWindowTitle("Текущий день — "+date.toString());
    c->show();
}

void OrgMain::clickDeleteButton()
{
    for(auto it = notes.begin(); it != notes.end(); it++)
        it->second.clear();
    repaintDays();
    notes.clear();
}

void OrgMain::repaintDays()
{
    QFont ft = QFont();
    ft.setPointSize(14);
    QTextCharFormat tch;
    for(auto it : notes)
    {
        QDate d = QDate::fromString(QString::fromStdString(it.first));
        if(it.second.empty())
        {
            ft.setBold(false);
            ft.setItalic(false);
            tch.setFont(ft);
            tch.setFontUnderline(false);
            if(d.dayOfWeek() == 6 || d.dayOfWeek() == 7)
                tch.setForeground(QBrush("red"));
            else
                tch.setForeground(QBrush());
        }
        else
        {
            ft.setBold(true);
            ft.setItalic(true);
            tch.setFont(ft);
            tch.setFontUnderline(true);
            if(d.dayOfWeek() == 6 || d.dayOfWeek() == 7)
                tch.setForeground(QBrush("red"));
            else
                tch.setForeground(QBrush("blue"));
        }
        ui->calendarWidget->setDateTextFormat(d, tch);
    }
}

