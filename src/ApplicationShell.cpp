#include "ApplicationShell.h"

#include "AppController.h"
#include "VtkRenderWidget.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QIcon>
#include <QQmlContext>
#include <QQmlError>
#include <QQuickItem>
#include <QQuickWidget>
#include <QResizeEvent>
#include <QShortcut>
#include <QShowEvent>
#include <QTimer>
#include <QVBoxLayout>
#include <QPalette>
#include <QVariantMap>

ApplicationShell::ApplicationShell(AppController *controller, QWidget *parent)
    : QWidget(parent)
    , m_controller(controller)
{
    setWindowTitle(QStringLiteral("EField Viewer · 八字拍电场仿真"));
    setWindowIcon(QIcon(QStringLiteral(":/icon.ico")));
    resize(1440, 900);
    setMinimumSize(1024, 680);

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    m_header = new QQuickWidget(this);
    setupQuickWidget(m_header, QUrl(QStringLiteral("qrc:/qml/HeaderBar.qml")));

    m_leftPanel = new QQuickWidget(this);
    setupQuickWidget(m_leftPanel, QUrl(QStringLiteral("qrc:/qml/components/LeftControlPanel.qml")));

    m_rightPanel = new QQuickWidget(this);
    setupQuickWidget(m_rightPanel, QUrl(QStringLiteral("qrc:/qml/components/ColorBarPanel.qml")));

    m_statusBar = new QQuickWidget(this);
    setupQuickWidget(m_statusBar, QUrl(QStringLiteral("qrc:/qml/StatusBar.qml")));

    // VTK 视口放入独立容器（与 ctm-viewer 一致），避免 Win32 子窗口影响 QML 面板
    m_viewerHost = new QWidget(this);
    m_viewerHost->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto *viewerLayout = new QVBoxLayout(m_viewerHost);
    viewerLayout->setContentsMargins(0, 0, 0, 0);
    viewerLayout->setSpacing(0);

    m_viewer = new VtkRenderWidget(m_viewerHost);
    m_viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    viewerLayout->addWidget(m_viewer);

    m_header->setFixedHeight(56);
    rootLayout->addWidget(m_header);

    auto *workspace = new QHBoxLayout();
    workspace->setContentsMargins(12, 12, 12, 12);
    workspace->setSpacing(12);

    m_leftPanel->setFixedWidth(300);
    workspace->addWidget(m_leftPanel);
    workspace->addWidget(m_viewerHost, 1);

    m_rightPanel->setFixedWidth(184);
    workspace->addWidget(m_rightPanel);

    rootLayout->addLayout(workspace, 1);

    m_statusBar->setFixedHeight(32);
    rootLayout->addWidget(m_statusBar);

    // 模态层：默认隐藏，仅在弹窗打开时显示（避免软件渲染 QQuickWidget 遮挡 3D 与侧栏）
    m_overlay = new QQuickWidget(this);
    setupQuickWidget(m_overlay, QUrl(QStringLiteral("qrc:/qml/overlays/AppOverlay.qml")), true);
    setOverlayVisible(false);

    m_openShortcut = new QShortcut(QKeySequence::Open, this);
    connect(m_openShortcut, &QShortcut::activated, m_controller, &AppController::openFileDialog);

    m_resetShortcut = new QShortcut(QKeySequence(Qt::Key_Home), this);
    connect(m_resetShortcut, &QShortcut::activated, m_controller, &AppController::resetCamera);

    applyThemeBackground();

    if (m_controller) {
        m_controller->setViewer(m_viewer);
        connect(m_controller, &AppController::themeChanged, this, &ApplicationShell::applyThemeBackground);
        connect(m_controller, &AppController::settingsRequested, this, &ApplicationShell::onSettingsRequested);
        connect(m_controller, &AppController::errorOccurred, this, &ApplicationShell::onErrorOccurred);
        connect(m_controller, &AppController::modalActiveChanged, this, &ApplicationShell::onModalActiveChanged);
        QTimer::singleShot(0, this, [this]() {
            if (m_controller) {
                m_controller->loadDefaultSimulation();
            }
        });
    }
}

void ApplicationShell::setupQuickWidget(QQuickWidget *widget, const QUrl &sourceUrl, bool transparent)
{
    widget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    widget->setAttribute(Qt::WA_OpaquePaintEvent, !transparent);

    if (m_controller) {
        widget->rootContext()->setContextProperty(QStringLiteral("App"), m_controller);
    }

    QColor clearColor;
    if (transparent) {
        clearColor = Qt::transparent;
    } else {
        clearColor = QColor(m_controller
                                ? m_controller->themePalette(m_controller->themeIndex())
                                      .value(QStringLiteral("clrBg"), QStringLiteral("#F5F8FD"))
                                      .toString()
                                : QStringLiteral("#F5F8FD"));
    }
    widget->setClearColor(clearColor);

    widget->setSource(sourceUrl);

    if (widget->status() == QQuickWidget::Error) {
        qWarning() << "QML load failed:" << sourceUrl;
        for (const QQmlError &error : widget->errors()) {
            qWarning() << error.toString();
        }
    }
}

void ApplicationShell::applyThemeBackground()
{
    if (!m_controller) {
        return;
    }

    const QVariantMap themeMap = m_controller->themePalette(m_controller->themeIndex());
    const QColor bg(themeMap.value(QStringLiteral("clrBg"), QStringLiteral("#F5F8FD")).toString());
    const QColor card(themeMap.value(QStringLiteral("clrCard"), QStringLiteral("#FFFFFF")).toString());

    QPalette widgetPalette = palette();
    widgetPalette.setColor(QPalette::Window, bg);
    setPalette(widgetPalette);
    setAutoFillBackground(true);

    const auto panels = {m_header, m_leftPanel, m_rightPanel, m_statusBar};
    for (QQuickWidget *widget : panels) {
        if (!widget) {
            continue;
        }
        widget->setClearColor(bg);
        widget->setStyleSheet(QStringLiteral("background-color: %1; border: none;").arg(bg.name()));
    }

    if (m_header) {
        m_header->setClearColor(card);
        m_header->setStyleSheet(QStringLiteral("background-color: %1; border: none;").arg(card.name()));
    }
    if (m_statusBar) {
        m_statusBar->setClearColor(card);
        m_statusBar->setStyleSheet(QStringLiteral("background-color: %1; border: none;").arg(card.name()));
    }
}

void ApplicationShell::raisePanelWidgets()
{
    const auto panels = {m_header, m_leftPanel, m_rightPanel, m_statusBar};
    for (QQuickWidget *widget : panels) {
        if (widget) {
            widget->raise();
            widget->update();
        }
    }
}

void ApplicationShell::setOverlayVisible(bool visible)
{
    if (!m_overlay) {
        return;
    }

    if (visible) {
        updateOverlayGeometry();
        m_overlay->show();
        m_overlay->raise();
    } else {
        m_overlay->hide();
        raisePanelWidgets();
    }
}

void ApplicationShell::invokeOverlayMethod(const char *method,
                                           const QVariant &arg1,
                                           const QVariant &arg2)
{
    if (!m_overlay || !m_overlay->rootObject()) {
        return;
    }

    QQuickItem *rootItem = m_overlay->rootObject();
    if (!rootItem) {
        return;
    }
    QObject *root = rootItem;
    if (arg1.isValid() && arg2.isValid()) {
        QMetaObject::invokeMethod(root, method, Q_ARG(QVariant, arg1), Q_ARG(QVariant, arg2));
    } else if (arg1.isValid()) {
        QMetaObject::invokeMethod(root, method, Q_ARG(QVariant, arg1));
    } else {
        QMetaObject::invokeMethod(root, method);
    }
}

void ApplicationShell::onSettingsRequested()
{
    setOverlayVisible(true);
    QTimer::singleShot(0, this, [this]() {
        invokeOverlayMethod("openSettingsDialog");
    });
}

void ApplicationShell::onErrorOccurred(const QString &message)
{
    setOverlayVisible(true);
    QTimer::singleShot(0, this, [this, message]() {
        invokeOverlayMethod("openMessageDialog",
                            QVariant(QStringLiteral("错误")),
                            QVariant(message));
    });
}

void ApplicationShell::onModalActiveChanged()
{
    if (!m_controller) {
        return;
    }
    setOverlayVisible(m_controller->modalActive());
}

void ApplicationShell::updateOverlayGeometry()
{
    if (m_overlay) {
        m_overlay->setGeometry(rect());
    }
}

void ApplicationShell::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    updateOverlayGeometry();
    raisePanelWidgets();
    if (m_overlay && m_overlay->isVisible()) {
        m_overlay->raise();
    }
}

void ApplicationShell::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (m_overlay && m_overlay->isVisible()) {
        updateOverlayGeometry();
    }
}
