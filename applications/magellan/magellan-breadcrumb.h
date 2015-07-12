#ifndef MAGELLANBREADCRUMB_H
#define MAGELLANBREADCRUMB_H

#include <QtCore/QUrl>

#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>

class Breadcrumb : public QWidget
{
    Q_OBJECT
public:
    explicit Breadcrumb(QWidget *parent = 0);

signals:
    // emitted when the URL is changed by the user
    void urlChanged(QUrl);

public slots:
    void setUrl(QUrl url);

private slots:
    void m_btn_clicked();
    void m_act_clicked();

private:
    // the url to display
    QUrl m_url;
    QList<QToolButton*> m_btns;
    QHBoxLayout *m_layout;
};

#endif // MAGELLANBREADCRUMB_H
