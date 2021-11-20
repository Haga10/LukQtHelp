#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include "independentwindow.h"
#include "helptextbrowser.h"

#include <QSplitter>

class HelpWindow : public IndependentWindow
{
    Q_OBJECT

public:
    HelpWindow(QHelpEngine *helpEngine, QWidget *parent = nullptr);

    void setSource(const QUrl &source);
    void setHomeSource(const QUrl &source);
    QUrl lastSource() const;

    inline void setOpenExternalLinksEnabled(const bool enabled)     { m_helpTextBrowser->setOpenExternalLinksEnabled(enabled); }
    inline bool openExternalLinksEnabled() const                    { return m_helpTextBrowser->openExternalLinksEnabled(); }

    inline void setHorizontalSplitterSizes(const QList<int> &sizes) { m_horizontalSplitter->setSizes(sizes); }
    inline QList<int> horizontalSplitterSizes() const               { return m_horizontalSplitter->sizes(); }

private:
    QHelpEngine *m_helpEngine;
    QHelpContentModel *m_helpContentModel;
    HelpTextBrowser *m_helpTextBrowser;
    QSplitter *m_horizontalSplitter;
    QList<int> m_horizontalSplitterSizes;
    QStatusBar *m_statusBar;
    QUrl m_homeSource;

private:
    void setToolBar();
    void setWidgets();
    void _setStatusBar();
    void showHideContents();
};

#endif // HELPWINDOW_H
