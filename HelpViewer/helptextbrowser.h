#ifndef HELPTEXTBROWSER_H
#define HELPTEXTBROWSER_H

#include <QTextBrowser>
#include <QHelpEngine>
#include <QWheelEvent>

class HelpTextBrowser : public QTextBrowser
{
public:
    HelpTextBrowser(QHelpEngine *helpEngine, QWidget *parent = nullptr);

    virtual void setSource(const QUrl &url) override;

    void setExternalLinksEnabled(const bool enabled)    { m_externalLinksEnabled = enabled; }
    bool externalLinksEnabled() const                   { return m_externalLinksEnabled; }

    static bool isUrlHttp(const QUrl &url);
    static bool isUrlHelp(const QUrl &url);

protected:
    virtual void wheelEvent(QWheelEvent *event) override;

private:
    QHelpEngine *m_helpEngine;
    bool m_externalLinksEnabled;

private:
    virtual QVariant loadResource (const int type, const QUrl &name) override;
};

#endif // HELPTEXTBROWSER_H
