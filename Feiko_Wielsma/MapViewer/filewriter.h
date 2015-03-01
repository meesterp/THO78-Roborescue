#ifndef FILEWRITER_H
#define FILEWRITER_H

#include "map.h"

class FileWriter
{
public:
    FileWriter( Map* map);
    ~FileWriter();
    void saveFile(QWidget* parent);
private:
    Map* map;

};

#endif // FILEWRITER_H
