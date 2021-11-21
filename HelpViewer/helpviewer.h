#ifndef HELPVIEWER_H
#define HELPVIEWER_H

#include "helpengine.h"
#include "helpwindow.h"

class HelpViewer : public QObject
{
    Q_OBJECT

public:
    explicit HelpViewer(QWidget *parent = nullptr);
    explicit HelpViewer(const QString &collectionFile, QWidget *parent = nullptr);

    ~HelpViewer();

    inline void setBasisWidget(QWidget *widget = nullptr)   { m_basisWidget = widget; }
    inline QWidget *basisWidget() const                     { return m_basisWidget; }

    void setWindowTitle(const QString &title);
    inline QString windowTitle() const                      { return m_helpWindowTitle; }

    void setCollectionFile(const QString &collectionFile);
    inline QString collectionFile() const                   { return m_collectionFile; }

    void setHomeSource(const QUrl &url);
    inline QUrl homeSource() const                          { return m_homeUrl; }

    void setOpenExternalLinksEnabled(const bool enabled);
    inline bool openExternalLinksEnabled() const            { return m_openExternalLinksEnabled; }

    bool open(const QUrl &url = QUrl());
    void close();

private:
    HelpEngine *m_helpEngine = nullptr;
    QString m_collectionFile;
    bool m_collectionFileChanged = false;

    QWidget *m_basisWidget;

    HelpWindow *m_helpWindow = nullptr;
    Qt::WindowStates m_helpWindowStates;
    QString m_helpWindowTitle;
    QPoint m_helpWindowPosition;
    QSize m_helpWindowSize = QSize(960, 600);
    QList<int> m_helpWindowHorizontalSplitterSizes = { 200, 600 };
    bool m_openExternalLinksEnabled = true;

    QUrl m_homeUrl;
    QUrl m_lastValidUrl;

private:
    bool check(const QUrl &source);
    void _open(const QUrl &source);
};

#endif // HELPVIEWER_H
