#include "notesclass.h"

NotesClass::NotesClass(std::string n, std::string d, QTime st, QTime sp)
{
    name = n;
    description = d;
    start = st;
    stop = sp;
}
