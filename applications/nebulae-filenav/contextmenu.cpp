#include "filesystemview.h"
#include "listwidgetitem.h"

#include <QtDebug>

void FilesystemView::m_displayContextMenuOnVoid(QPoint p)
{
    m_context->clear();

    QAction *act_newdir = m_context->addAction(QIcon::fromTheme("folder-new"), tr("New folder"), this, SLOT(m_act_newdir()));
    act_newdir->setProperty("FilePath", m_current_directory);

    QAction *act_newfile = m_context->addAction(QIcon::fromTheme("document-new"), tr("New empty file"), this, SLOT(m_act_newfile()));
    act_newfile->setProperty("FilePath", m_current_directory);

    m_context->addSeparator();

    QAction *act_paste = m_context->addAction(QIcon::fromTheme("edit-paste"), tr("Paste here"), this, SLOT(m_act_paste()));
    act_paste->setProperty("FilePath", m_current_directory);

    m_context->addSeparator();

    QAction *act_selectall = m_context->addAction(QIcon::fromTheme("edit-select-all"), tr("Select all"), this, SLOT(m_act_selectall()));
    act_selectall->setProperty("FilePath", m_current_directory);

    m_context->popup(p + m_parent->pos());
}

void FilesystemView::m_displayContextMenuOnFile(QPoint p)
{
    m_context->clear();

    ListWidgetItem *item = static_cast<ListWidgetItem*>(m_list->itemAt(p));
    if(!item) return;

    QAction *act_openas = m_context->addAction(QIcon::fromTheme("document-open"), tr("Open as..."), this, SLOT(m_file_openas()));
    act_openas->setProperty("FilePath", item->filePath());

    m_context->addSeparator();

    QAction *act_copy = m_context->addAction(QIcon::fromTheme("edit-copy"), tr("Copy..."), this, SLOT(m_file_copy()));
    act_copy->setProperty("FilePath", item->filePath());
    QAction *act_cut = m_context->addAction(QIcon::fromTheme("edit-cut"), tr("Cut..."), this, SLOT(m_file_cut()));
    act_cut->setProperty("FilePath", item->filePath());

    m_context->addSeparator();

    QAction *act_delete = m_context->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"), this, SLOT(m_file_trash()));
    act_delete->setProperty("FilePath", item->filePath());
    QAction *act_rename = m_context->addAction(QIcon::fromTheme("edit-rename"), tr("Rename"), this, SLOT(m_file_rename()));
    act_rename->setProperty("FilePath", item->filePath());

    m_context->addSeparator();

    QAction *act_properties = m_context->addAction(QIcon::fromTheme("document-properties"), tr("Properties"), this, SLOT(m_file_properties()));
    act_properties->setProperty("FilePath", item->filePath());

    m_context->popup(p + m_parent->pos());
}

void FilesystemView::m_displayContextMenuOnDir(QPoint p)
{
    m_context->clear();

    ListWidgetItem *item = static_cast<ListWidgetItem*>(m_list->itemAt(p));
    if(!item) return;

    QAction *act_opentab = m_context->addAction(QIcon::fromTheme("tab-new"), tr("Open in new tab"), this, SLOT(m_dir_opentab()));
    act_opentab->setProperty("FilePath", item->filePath());

    m_context->addSeparator();

    QAction *act_copy = m_context->addAction(QIcon::fromTheme("edit-copy"), tr("Copy..."), this, SLOT(m_dir_copy()));
    act_copy->setProperty("FilePath", item->filePath());
    QAction *act_cut = m_context->addAction(QIcon::fromTheme("edit-cut"), tr("Cut..."), this, SLOT(m_dir_cut()));
    act_cut->setProperty("FilePath", item->filePath());
    QAction *act_pastein = m_context->addAction(QIcon::fromTheme("edit-paste"), tr("Paste in..."), this, SLOT(m_dir_pastein()));
    act_pastein->setProperty("FilePath", item->filePath());

    m_context->addSeparator();

    QAction *act_delete = m_context->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"), this, SLOT(m_dir_trash()));
    act_delete->setProperty("FilePath", item->filePath());
    QAction *act_rename = m_context->addAction(QIcon::fromTheme("edit-rename"), tr("Rename"), this, SLOT(m_dir_rename()));
    act_rename->setProperty("FilePath", item->filePath());

    m_context->addSeparator();

    QAction *act_properties = m_context->addAction(QIcon::fromTheme("document-properties"), tr("Properties"), this, SLOT(m_dir_properties()));
    act_properties->setProperty("FilePath", item->filePath());

    m_context->popup(p + m_parent->pos());
}

void FilesystemView::m_displayContextMenuWithSelection(QPoint p)
{
    m_context->clear();

    QStringList pathlist;
    foreach(QListWidgetItem *it, m_list->selectedItems())
    { // gets all the selected items and converts them into ListItemWidgets so we can get the FilePath
        ListWidgetItem *item = static_cast<ListWidgetItem*>(it);

        // the methods m_selection_copy, m_selection_cut and m_selection_trash will attempt to get back a QStringList
        // from the sender action's FilePath property, that's what we give them :
        pathlist.append(item->filePath().path());
    }

    QAction *act_copy = m_context->addAction(QIcon::fromTheme("edit-copy"), tr("Copy..."), this, SLOT(m_selection_copy()));
    act_copy->setProperty("FilePath", pathlist);
    QAction *act_cut = m_context->addAction(QIcon::fromTheme("edit-cut"), tr("Cut..."), this, SLOT(m_selection_cut()));
    act_cut->setProperty("FilePath", pathlist);

    m_context->addSeparator();

    QAction *act_delete = m_context->addAction(QIcon::fromTheme("edit-delete"), tr("Delete"), this, SLOT(m_selection_trash()));
    act_delete->setProperty("FilePath", pathlist);

    m_context->popup(p + m_parent->pos());
}

void FilesystemView::m_displayContextMenu(QPoint p)
{
    if(m_list->selectedItems().count() > 1)
    {
        m_displayContextMenuWithSelection(p);
        return;
    }

    ListWidgetItem *item = static_cast<ListWidgetItem*>(m_list->itemAt(p));

    if(!item)
        m_displayContextMenuOnVoid(p);
    else if(item->isDir())
        m_displayContextMenuOnDir(p);
    else
        m_displayContextMenuOnFile(p);
}

