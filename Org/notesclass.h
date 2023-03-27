#ifndef NOTESCLASS_H
#define NOTESCLASS_H

#include<QDate>
#include<QTime>

class NotesClass
{
public:
    NotesClass() = default;
    NotesClass(std::string n, std::string d, QTime st, QTime sp);
    std::string name, description;
    QTime start, stop;
};

#endif // NOTESCLASS_H
