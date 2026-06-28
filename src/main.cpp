#include "ApplicationShell.h"
#include "AppController.h"

#include <QApplication>
#include <QIcon>
#include <QQuickWindow>
#include <QTimer>

#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // QML 侧栏使用软件 Scene Graph，避免与 VTK OpenGL 冲突导致黑屏
    QQuickWindow::setSceneGraphBackend(QStringLiteral("software"));

    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("EField Viewer"));
    app.setApplicationVersion(QStringLiteral("2.0.0"));
    app.setOrganizationName(QStringLiteral("Mag-aim"));

    const QIcon icon(QStringLiteral(":/icon.ico"));
    if (!icon.isNull()) {
        app.setWindowIcon(icon);
    }

    AppController controller;
    ApplicationShell shell(&controller);
    shell.show();

    if (argc > 1) {
        const QString filePath = QString::fromLocal8Bit(argv[1]);
        QTimer::singleShot(300, &controller, [filePath, &controller]() {
            controller.openFile(filePath);
        });
    }

    return app.exec();
}
