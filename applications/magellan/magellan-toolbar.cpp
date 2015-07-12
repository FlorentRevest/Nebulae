#include "magellan-toolbar.h"

#include <QtGui/QAction>
#include <QtGui/QToolButton>

#include <QtDebug>

Toolbar::Toolbar(QWidget *parent) :
    QToolBar(parent)
{
    layout()->setSpacing(0);

    QWidget *m_w = new QWidget(this);
    m_l = new QHBoxLayout(this);
    m_l->setContentsMargins(0,0,0,0);
    m_l->setSpacing(0);
    m_w->setLayout(m_l);

    // create basic actions
    m_btn_back = new QToolButton(this);
    m_btn_next = new QToolButton(this);
    QToolButton *btn_top = new QToolButton(this);
    m_btn_back->setIcon(QIcon::fromTheme("go-previous")); m_btn_back->setText(tr("Previous folder"));
    btn_top->setIcon(QIcon::fromTheme("go-up")); btn_top->setText(tr("Parent folder"));
    m_btn_next->setIcon(QIcon::fromTheme("go-next")); m_btn_next->setText(tr("Next folder"));
    connect(m_btn_back, SIGNAL(clicked()), this, SIGNAL(backClicked()));
    connect(m_btn_next, SIGNAL(clicked()), this, SIGNAL(nextClicked()));
    connect(btn_top, SIGNAL(clicked()), this, SIGNAL(topClicked()));
    m_l->addWidget(m_btn_back);
    m_l->addWidget(btn_top);
    m_l->addWidget(m_btn_next);

    m_breadcrumb = new Breadcrumb(this);
    m_l->addWidget(m_breadcrumb, 1, Qt::AlignLeft);
    connect(m_breadcrumb, SIGNAL(urlChanged(QUrl)), this, SIGNAL(urlChanged(QUrl)));

    m_line_edit = new QLineEdit(this);
    m_line_edit->hide();
    connect(m_line_edit, SIGNAL(returnPressed()), this, SLOT(m_edit_bar_returned()));

    m_btn_switch = new QToolButton(this);
    m_btn_switch->setIcon(QIcon::fromTheme("gtk-edit")); m_btn_switch->setText(tr("Edit"));
    connect(m_btn_switch, SIGNAL(clicked()), this, SLOT(m_show_edit_bar()));
    m_l->addWidget(m_btn_switch);

    QToolButton *btn_icon = new QToolButton(this);
    QToolButton *btn_list = new QToolButton(this);
    btn_icon->setIcon(QIcon::fromTheme("view-icons")); btn_icon->setText(tr("Icon view"));
    btn_list->setIcon(QIcon::fromTheme("view-list")); btn_list->setText(tr("List view"));
    connect(btn_icon, SIGNAL(clicked()), this, SLOT(m_change_view_icons()));
    connect(btn_list, SIGNAL(clicked()), this, SLOT(m_change_view_list()));
    m_l->addWidget(btn_icon);
    m_l->addWidget(btn_list);

    addWidget(m_w);
}

void Toolbar::setUndoRedoEnabled(bool undo_enabled, bool redo_enabled)
{
    m_btn_back->setEnabled(undo_enabled);
    m_btn_next->setEnabled(redo_enabled);
}

void Toolbar::m_change_view_icons()
{
    emit viewModeChanged(QListWidget::IconMode);
}

void Toolbar::m_change_view_list()
{
    emit viewModeChanged(QListWidget::ListMode);
}

void Toolbar::m_show_edit_bar()
{
    m_l->removeWidget(m_breadcrumb);
    m_l->insertWidget(3, m_line_edit);
    m_line_edit->show();
    m_breadcrumb->hide();

    m_btn_switch->setIcon(QIcon::fromTheme("go-next"));
    m_btn_switch->setText(tr("Go"));

    disconnect(m_btn_switch, SIGNAL(clicked()), this, SLOT(m_show_edit_bar()));
    connect(m_btn_switch, SIGNAL(clicked()), this, SLOT(m_edit_bar_returned()));
}

void Toolbar::m_edit_bar_returned()
{
    m_l->removeWidget(m_line_edit);
    m_l->insertWidget(3, m_breadcrumb, 1, Qt::AlignLeft);
    m_line_edit->hide();
    m_breadcrumb->show();

    m_btn_switch->setIcon(QIcon::fromTheme("gtk-edit"));
    m_btn_switch->setText(tr("Edit"));

    disconnect(m_btn_switch, SIGNAL(clicked()), this, SLOT(m_edit_bar_returned()));
    connect(m_btn_switch, SIGNAL(clicked()), this, SLOT(m_show_edit_bar()));

    qDebug() << m_line_edit->text() << " emitted [toolbar]";
    emit urlChanged(QUrl(m_line_edit->text()));
}

void Toolbar::setUrl(QUrl url)
{
    m_line_edit->setText(url.toString());
    m_breadcrumb->setUrl(url);
}
