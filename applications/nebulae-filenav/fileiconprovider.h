#ifndef FILEICONPROVIDER_H
#define FILEICONPROVIDER_H

#include <QFileIconProvider>

#include "nebulae-io.h"

class FileIconProvider : public QFileIconProvider
{

public:
    explicit FileIconProvider();

    QIcon icon(FileInfo info);
    QSize iconSize();

    void setIconSize(QSize);

private:
    QSize m_size;

};

#endif // FILEICONPROVIDER_H
