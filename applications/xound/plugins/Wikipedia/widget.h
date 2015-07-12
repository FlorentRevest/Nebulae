#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>
#include <QtWebKit>
#include <QTranslator>
#include <QTabWidget>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include "include/contentplugin.h"
#include "include/core.h"

class WikiPlugin : public ContentPlugin
{
    Q_OBJECT
    Q_INTERFACES(ContentPlugin)

public:
    WikiPlugin();
    virtual QString name() const;
    virtual QIcon icon() const;

private slots:
    void updatePages();

private:
    QWebView *m_artistWebView, *m_albumWebView, *m_titleWebView;
    QTabWidget *m_tabWidget;
    Core *m_c;
};

#endif // WIDGET_H
