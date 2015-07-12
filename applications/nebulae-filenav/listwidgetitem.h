#ifndef LISTWIDGETITEM_H
#define LISTWIDGETITEM_H

#include <QUrl>
#include <QListWidgetItem>

class ListWidgetItem : public QListWidgetItem
{
public:
    ListWidgetItem(bool, const QUrl&, QListWidget*);

public:
    bool isDir();
    QUrl filePath();

private:
    QUrl m_filepath;
    bool m_isdir;
};

#endif // LISTWIDGETITEM_H
