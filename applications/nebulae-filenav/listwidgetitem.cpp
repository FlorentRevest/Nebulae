#include "listwidgetitem.h"

ListWidgetItem::ListWidgetItem(bool isDir, const QUrl &filePath, QListWidget *parent) : QListWidgetItem(parent, Type), m_filepath(filePath), m_isdir(isDir) {}

bool ListWidgetItem::isDir()
{
    return m_isdir;
}

QUrl ListWidgetItem::filePath()
{
    return m_filepath;
}
