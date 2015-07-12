#ifndef MAGELLANTOOLBAR_H
#define MAGELLANTOOLBAR_H

#include <QtCore/QUrl>

#include <QtGui/QToolBar>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QToolButton>

#include "magellan-breadcrumb.h"

class Toolbar : public QToolBar
{
    Q_OBJECT
public:
    explicit Toolbar(QWidget *parent = 0);

    // set if previous/back buttons should be enabled
    void setUndoRedoEnabled(bool, bool);

signals:
    void backClicked();
    void nextClicked();
    void topClicked();
    void urlChanged(QUrl);
    void viewModeChanged(QListWidget::ViewMode);
    void optionsClicked();

public slots:
    void setUrl(QUrl);

private slots:
    void m_change_view_icons();
    void m_change_view_list();
    void m_show_edit_bar();
    void m_edit_bar_returned();

private:
    QHBoxLayout *m_l;
    QSpacerItem *m_spacer;
    Breadcrumb *m_breadcrumb;
    QLineEdit *m_line_edit;
    QToolButton *m_btn_switch, *m_btn_back, *m_btn_next;
};

#endif // MAGELLANTOOLBAR_H
