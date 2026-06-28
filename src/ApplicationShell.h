#ifndef APPLICATIONSHELL_H
#define APPLICATIONSHELL_H

#include <QWidget>
#include <QVariant>

class AppController;
class QQuickWidget;
class QShortcut;
class VtkRenderWidget;

class ApplicationShell : public QWidget
{
    Q_OBJECT

public:
    explicit ApplicationShell(AppController *controller, QWidget *parent = nullptr);

    VtkRenderWidget *viewer() const { return m_viewer; }

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onSettingsRequested();
    void onErrorOccurred(const QString &message);
    void onModalActiveChanged();

private:
    void setupQuickWidget(QQuickWidget *widget, const QUrl &sourceUrl, bool transparent = false);
    void applyThemeBackground();
    void updateOverlayGeometry();
    void setOverlayVisible(bool visible);
    void raisePanelWidgets();
    void invokeOverlayMethod(const char *method, const QVariant &arg1 = QVariant(), const QVariant &arg2 = QVariant());

    AppController *m_controller = nullptr;
    QWidget *m_viewerHost = nullptr;
    VtkRenderWidget *m_viewer = nullptr;
    QQuickWidget *m_header = nullptr;
    QQuickWidget *m_leftPanel = nullptr;
    QQuickWidget *m_rightPanel = nullptr;
    QQuickWidget *m_statusBar = nullptr;
    QQuickWidget *m_overlay = nullptr;
    QShortcut *m_openShortcut = nullptr;
    QShortcut *m_resetShortcut = nullptr;
};

#endif
