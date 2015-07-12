#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>
#include <fileref.h>
#include <tag.h>
#include "../../include/contentplugin.h"

class TagEditorPlugin : public ContentPlugin
{
    Q_OBJECT
    Q_INTERFACES(ContentPlugin)

public:
    TagEditorPlugin();
    virtual QString name() const;
    virtual QIcon icon() const;
    virtual void setMusicPath(QString);

public slots:
    void reinitialise();
    void save();

private:
    QString m_title, m_artist, m_album, m_genre, m_comment;
    int m_year, m_track;
    QLabel *label, *label_2, *label_3, *label_4, *label_5, *label_6, *label_7;
    QLineEdit *titleEdit, *albumEdit, *artistEdit, *commentEdit, *genreEdit;
    QSpinBox *trackEdit, *yearEdit;
    QSpacerItem *horizontalSpacer;
    QPushButton *saveButton, *reinitialiseButton;
    TagLib::FileRef *f;
};

#endif // WIDGET_H
