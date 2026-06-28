#include "VtkHost.h"
#include "VtkRenderWidget.h"

#include <QQuickWindow>
#include <QTimer>
#include <QVBoxLayout>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

VtkHost::VtkHost(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setClip(true);
    connect(this, &QQuickItem::windowChanged, this, &VtkHost::handleWindowChanged);
}

VtkHost::~VtkHost()
{
    delete m_container;
    m_container = nullptr;
    m_viewer = nullptr;
}

void VtkHost::embedContainerNative()
{
    if (!m_container || !window()) {
        return;
    }

#ifdef Q_OS_WIN
    window()->winId();
    m_container->winId();

    const HWND parentHwnd = reinterpret_cast<HWND>(window()->winId());
    const HWND childHwnd = reinterpret_cast<HWND>(m_container->winId());
    if (!parentHwnd || !childHwnd) {
        return;
    }

    SetParent(childHwnd, parentHwnd);

    LONG style = GetWindowLongPtr(childHwnd, GWL_STYLE);
    style &= ~(WS_POPUP | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
    style |= WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    SetWindowLongPtr(childHwnd, GWL_STYLE, style);

    SetWindowPos(childHwnd,
                 HWND_TOP,
                 0,
                 0,
                 0,
                 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_FRAMECHANGED);
#endif

    m_container->show();
}

void VtkHost::ensureViewer()
{
    if (m_container || !window()) {
        return;
    }

    m_container = new QWidget(nullptr);
    m_container->setAttribute(Qt::WA_NativeWindow, true);
    m_container->setAttribute(Qt::WA_DontCreateNativeAncestors, true);
    m_container->setAutoFillBackground(false);

    auto *layout = new QVBoxLayout(m_container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_viewer = new VtkRenderWidget(m_container);
    layout->addWidget(m_viewer);

    embedContainerNative();
    updateViewerGeometry();

    connect(window(), &QQuickWindow::xChanged, this, &VtkHost::updateViewerGeometry);
    connect(window(), &QQuickWindow::yChanged, this, &VtkHost::updateViewerGeometry);
    connect(window(), &QQuickWindow::widthChanged, this, &VtkHost::updateViewerGeometry);
    connect(window(), &QQuickWindow::heightChanged, this, &VtkHost::updateViewerGeometry);
    connect(window(), &QQuickWindow::visibleChanged, this, &VtkHost::updateViewerGeometry);
    connect(window(), &QQuickWindow::afterRendering, this, &VtkHost::raiseViewer, Qt::DirectConnection);

    if (!m_viewerReadyEmitted) {
        m_viewerReadyEmitted = true;
        QTimer::singleShot(50, this, [this]() {
            updateViewerGeometry();
            emit viewerReady();
        });
    }
}

void VtkHost::handleWindowChanged(QQuickWindow *window)
{
    Q_UNUSED(window)
    ensureViewer();
}

void VtkHost::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    updateViewerGeometry();
}

void VtkHost::itemChange(ItemChange change, const ItemChangeData &data)
{
    if (change == ItemSceneChange) {
        handleWindowChanged(data.window);
    }
    QQuickItem::itemChange(change, data);
}

void VtkHost::updateViewerGeometry()
{
    if (!m_container || !window()) {
        return;
    }

    const QPointF scenePos = mapToScene(QPointF(0, 0));
    const int x = qRound(scenePos.x());
    const int y = qRound(scenePos.y());
    const int w = qMax(1, int(width()));
    const int h = qMax(1, int(height()));

    m_container->setGeometry(x, y, w, h);

#ifdef Q_OS_WIN
    const HWND hwnd = reinterpret_cast<HWND>(m_container->winId());
    if (hwnd) {
        MoveWindow(hwnd, x, y, w, h, TRUE);
    }
#endif
}

void VtkHost::raiseViewer()
{
    if (!m_container || !window()) {
        return;
    }

#ifdef Q_OS_WIN
    const HWND hwnd = reinterpret_cast<HWND>(m_container->winId());
    if (hwnd) {
        SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
#endif
}
