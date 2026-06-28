#ifndef VTKHOST_H
#define VTKHOST_H

#include <QQuickItem>

class QWidget;
class VtkRenderWidget;

class VtkHost : public QQuickItem
{
    Q_OBJECT

public:
    explicit VtkHost(QQuickItem *parent = nullptr);
    ~VtkHost() override;

    VtkRenderWidget *viewer() const { return m_viewer; }

signals:
    void viewerReady();

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void itemChange(ItemChange change, const ItemChangeData &data) override;

private slots:
    void handleWindowChanged(QQuickWindow *window);
    void updateViewerGeometry();
    void raiseViewer();

private:
    void ensureViewer();
    void embedContainerNative();

    QWidget *m_container = nullptr;
    VtkRenderWidget *m_viewer = nullptr;
    bool m_viewerReadyEmitted = false;
};

#endif
