#include "helpviewer.h"

#include <QDesktopServices>



HelpViewer::HelpViewer(QWidget *parent) :
    QObject(parent),
    m_basisWidget(parent)
{
}



HelpViewer::HelpViewer(const QString &collectionFile, QWidget *parent) :
    QObject(parent),
    m_collectionFile(collectionFile),
    m_basisWidget(parent)
{
}



HelpViewer::~HelpViewer()
{
    close();
}



void HelpViewer::setWindowTitle(const QString &title)
{
    m_helpWindowTitle = title;

    if (m_helpWindow != nullptr)
        m_helpWindow->setWindowTitle(title);
}



void HelpViewer::setHomeSource(const QUrl &url)
{
    m_homeUrl = url;

    if (m_helpWindow != nullptr)
        m_helpWindow->setHomeSource(url);
}



void HelpViewer::setOpenExternalLinksEnabled(const bool enabled)
{
    m_openExternalLinksEnabled = enabled;

    if (m_helpWindow != nullptr)
        m_helpWindow->setOpenExternalLinksEnabled(enabled);
}



bool HelpViewer::open(const QUrl &url)
{
    if (!HelpTextBrowser::isUrlHttp(url)) {
        if (check(url)) {
            _open(url);
            return true;
        }
    }

    else if (m_openExternalLinksEnabled)
        return QDesktopServices::openUrl(url);

    return false;
}



bool HelpViewer::check(const QUrl &url)
{
    if (m_collectionFile.isEmpty())
        return false;

    QSharedPointer<HelpEngine> helpEngine;

    if (!m_helpEngine.isNull() && m_helpEngine->collectionFile() == m_collectionFile)
        helpEngine = m_helpEngine;

    else {
        helpEngine = QSharedPointer<HelpEngine>::create(m_collectionFile);
        if (!helpEngine->setupData())
            return false;
    }

    bool ok = false;

    if (!url.isEmpty()) {
        if (!helpEngine->isEmpty(url)) {
            m_lastValidUrl = url;
            ok = true;
        }
    }

    else if (!m_lastValidUrl.isEmpty() && !helpEngine->isEmpty(m_lastValidUrl))
        ok = true;

    else if (!m_homeUrl.isEmpty() && !helpEngine->isEmpty(m_homeUrl)) {
        m_lastValidUrl = m_homeUrl;
        ok = true;
    }

    else {
        const QUrl fileUrl = helpEngine->fileUrl(QLatin1String("index"));
        if (!fileUrl.isEmpty()) {
            m_lastValidUrl = fileUrl;
            ok = true;
        }
    }

    if (ok) {
        m_helpEngine = helpEngine;
        m_helpEngine->setCollectionFile(m_collectionFile);
        return true;
    }

    return false;
}



void HelpViewer::_open(const QUrl &url)
{
    if (m_helpWindow == nullptr) {
        m_helpWindow = new HelpWindow(m_helpEngine.data());
        m_helpWindow->setWindowState(m_helpWindowStates);
        m_helpWindow->setWindowTitle(m_helpWindowTitle);

        if (!m_helpWindowPosition.isNull()) {
            m_helpWindow->move(m_helpWindowPosition);
            m_helpWindow->setSize(m_helpWindowSize);
        }
        else
            m_helpWindow->setSize(m_helpWindowSize, m_basisWidget);

        m_helpWindow->setHorizontalSplitterSizes(m_helpWindowHorizontalSplitterSizes);
        m_helpWindow->setSource(m_lastValidUrl);
        m_helpWindow->setHomeSource(m_homeUrl);
        m_helpWindow->setOpenExternalLinksEnabled(m_openExternalLinksEnabled);

        connect(m_helpWindow, &HelpWindow::destroyed, this, [this] {
            m_helpWindowStates = m_helpWindow->windowState();
            m_helpWindowPosition = m_helpWindow->pos();
            m_helpWindowSize = m_helpWindow->size();
            m_helpWindowHorizontalSplitterSizes = m_helpWindow->horizontalSplitterSizes();
            m_lastValidUrl = m_helpWindow->lastSource().toString();
            m_helpWindow = nullptr;
            m_helpEngine = QSharedPointer<HelpEngine>(nullptr);
        });

        m_helpWindow->show();
    }

    else {
        m_helpWindow->setSource(url);
        m_helpWindow->activateWindow();
    }
}



void HelpViewer::close()
{
    if (m_helpWindow != nullptr)
        m_helpWindow->close();
}
