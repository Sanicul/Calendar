#include "orgmain.h"
#include "ui_orgmain.h"
#include "currentday.h"
#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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
}

OrgMain::~OrgMain()
{
    QFile f("Data.json");
    if (f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonObject obj;
        for(auto it : notes)
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
        QByteArray data = QJsonDocument(obj).toJson();
        f.write(data);
    }
    delete ui;
}

void OrgMain::clickedDay(QDate date)
{
    CurrentDay *c = new CurrentDay(&(notes[date.toString().toStdString()]), this);
    c->show();
}

void OrgMain::clickDeleteButton()
{
    notes.clear();
}

