#include "AppController.h"

#include "HeadMaterial.h"
#include "ModelLoader.h"
#include "VtkRenderWidget.h"

#include <QApplication>
#include <QWidget>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QVariantMap>

#include <cmath>

namespace {

AppTheme::ThemeId themeFromIndex(int index)
{
    switch (index) {
    case 1:
        return AppTheme::ThemeId::Dark;
    case 2:
        return AppTheme::ThemeId::Ocean;
    case 3:
        return AppTheme::ThemeId::Mint;
    case 0:
    default:
        return AppTheme::ThemeId::Light;
    }
}

int indexFromTheme(AppTheme::ThemeId theme)
{
    switch (theme) {
    case AppTheme::ThemeId::Dark:
        return 1;
    case AppTheme::ThemeId::Ocean:
        return 2;
    case AppTheme::ThemeId::Mint:
        return 3;
    case AppTheme::ThemeId::Light:
    default:
        return 0;
    }
}

} // namespace

AppController::AppController(QObject *parent)
    : QObject(parent)
{
}

void AppController::setViewer(VtkRenderWidget *viewer)
{
    if (m_viewer == viewer) {
        return;
    }

    if (m_viewer) {
        disconnect(m_viewer, nullptr, this, nullptr);
    }

    m_viewer = viewer;
    if (!m_viewer) {
        return;
    }

    connect(m_viewer, &VtkRenderWidget::coilPoseChanged, this, &AppController::onCoilPoseFromViewer);
    m_viewer->setHeadOpacity(m_headOpacity / 100.0);
    m_viewer->setHeadMaterialIndex(m_headMaterialIndex);
    m_viewer->setSmoothNormalsEnabled(m_smoothNormals);
    applyTheme(m_theme);
}

QString AppController::assetsRoot() const
{
    const QString bundled = QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("assets"));
    if (QDir(bundled).exists()) {
        return bundled;
    }
    return QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("../../assets"));
}

QStringList AppController::themeNames() const
{
    QStringList names;
    for (const AppTheme::UiThemeEntry &entry : AppTheme::availableThemes()) {
        names.append(entry.name);
    }
    return names;
}

QStringList AppController::headMaterialNames() const
{
    return HeadMaterial::materialNames();
}

QString AppController::themeAccent(int index) const
{
    return AppTheme::themeAccent(themeFromIndex(index));
}

QString AppController::themeSwatchGradient(int index) const
{
    return AppTheme::themeSwatchGradient(themeFromIndex(index));
}

void AppController::openSettings()
{
    emit settingsRequested();
}

void AppController::notifyModalVisible(bool visible)
{
    if (m_modalActive == visible) {
        return;
    }
    m_modalActive = visible;
    emit modalActiveChanged();
}

QVariantMap AppController::themePalette(int index) const
{
    const AppTheme::ThemeId theme = themeFromIndex(index);
    QVariantMap palette;

    switch (theme) {
    case AppTheme::ThemeId::Dark:
        palette.insert(QStringLiteral("clrBg"), QStringLiteral("#0F1419"));
        palette.insert(QStringLiteral("clrCard"), QStringLiteral("#1A2332"));
        palette.insert(QStringLiteral("clrBorder"), QStringLiteral("#334155"));
        palette.insert(QStringLiteral("clrTitle"), QStringLiteral("#F1F5F9"));
        palette.insert(QStringLiteral("clrText"), QStringLiteral("#E2E8F0"));
        palette.insert(QStringLiteral("clrSubText"), QStringLiteral("#94A3B8"));
        palette.insert(QStringLiteral("clrPrimary"), QStringLiteral("#3B82F6"));
        palette.insert(QStringLiteral("clrPrimaryHover"), QStringLiteral("#2563EB"));
        palette.insert(QStringLiteral("clrFieldBg"), QStringLiteral("#1E293B"));
        palette.insert(QStringLiteral("clrFieldBorder"), QStringLiteral("#475569"));
        palette.insert(QStringLiteral("primarySoft"), QStringLiteral("#1E3A5F"));
        palette.insert(QStringLiteral("clrViewportBg1"), QStringLiteral("#FAFBFE"));
        palette.insert(QStringLiteral("clrViewportBg2"), QStringLiteral("#F0F4F8"));
        break;
    case AppTheme::ThemeId::Ocean:
        palette.insert(QStringLiteral("clrBg"), QStringLiteral("#EEF6FC"));
        palette.insert(QStringLiteral("clrCard"), QStringLiteral("#FFFFFF"));
        palette.insert(QStringLiteral("clrBorder"), QStringLiteral("#C8DDF0"));
        palette.insert(QStringLiteral("clrTitle"), QStringLiteral("#0C4A6E"));
        palette.insert(QStringLiteral("clrText"), QStringLiteral("#0C4A6E"));
        palette.insert(QStringLiteral("clrSubText"), QStringLiteral("#5B8BAE"));
        palette.insert(QStringLiteral("clrPrimary"), QStringLiteral("#0284C7"));
        palette.insert(QStringLiteral("clrPrimaryHover"), QStringLiteral("#0369A1"));
        palette.insert(QStringLiteral("clrFieldBg"), QStringLiteral("#FFFFFF"));
        palette.insert(QStringLiteral("clrFieldBorder"), QStringLiteral("#CBD5E1"));
        palette.insert(QStringLiteral("primarySoft"), QStringLiteral("#DBEAFE"));
        palette.insert(QStringLiteral("clrViewportBg1"), QStringLiteral("#EDF7FF"));
        palette.insert(QStringLiteral("clrViewportBg2"), QStringLiteral("#D6EBFA"));
        break;
    case AppTheme::ThemeId::Mint:
        palette.insert(QStringLiteral("clrBg"), QStringLiteral("#F2FAF5"));
        palette.insert(QStringLiteral("clrCard"), QStringLiteral("#FFFFFF"));
        palette.insert(QStringLiteral("clrBorder"), QStringLiteral("#C6E7D4"));
        palette.insert(QStringLiteral("clrTitle"), QStringLiteral("#14532D"));
        palette.insert(QStringLiteral("clrText"), QStringLiteral("#14532D"));
        palette.insert(QStringLiteral("clrSubText"), QStringLiteral("#5F8F72"));
        palette.insert(QStringLiteral("clrPrimary"), QStringLiteral("#16A34A"));
        palette.insert(QStringLiteral("clrPrimaryHover"), QStringLiteral("#15803D"));
        palette.insert(QStringLiteral("clrFieldBg"), QStringLiteral("#FFFFFF"));
        palette.insert(QStringLiteral("clrFieldBorder"), QStringLiteral("#CBD5E1"));
        palette.insert(QStringLiteral("primarySoft"), QStringLiteral("#DCFCE7"));
        palette.insert(QStringLiteral("clrViewportBg1"), QStringLiteral("#F6FDF8"));
        palette.insert(QStringLiteral("clrViewportBg2"), QStringLiteral("#E6F7EC"));
        break;
    case AppTheme::ThemeId::Light:
    default:
        palette.insert(QStringLiteral("clrBg"), QStringLiteral("#F5F8FD"));
        palette.insert(QStringLiteral("clrCard"), QStringLiteral("#FFFFFF"));
        palette.insert(QStringLiteral("clrBorder"), QStringLiteral("#E2E8F0"));
        palette.insert(QStringLiteral("clrTitle"), QStringLiteral("#0F172A"));
        palette.insert(QStringLiteral("clrText"), QStringLiteral("#334155"));
        palette.insert(QStringLiteral("clrSubText"), QStringLiteral("#64748B"));
        palette.insert(QStringLiteral("clrPrimary"), QStringLiteral("#3B82F6"));
        palette.insert(QStringLiteral("clrPrimaryHover"), QStringLiteral("#2563EB"));
        palette.insert(QStringLiteral("clrFieldBg"), QStringLiteral("#F8FAFC"));
        palette.insert(QStringLiteral("clrFieldBorder"), QStringLiteral("#CBD5E1"));
        palette.insert(QStringLiteral("primarySoft"), QStringLiteral("#DBEAFE"));
        palette.insert(QStringLiteral("clrViewportBg1"), QStringLiteral("#FAFBFE"));
        palette.insert(QStringLiteral("clrViewportBg2"), QStringLiteral("#F0F4F8"));
        break;
    }

    palette.insert(QStringLiteral("clrDanger"), QStringLiteral("#EF4444"));
    palette.insert(QStringLiteral("clrRoam"), QStringLiteral("#8B5CF6"));
    palette.insert(QStringLiteral("clrScrim"), QStringLiteral("#590F172A"));
    palette.insert(QStringLiteral("shadowColor"), QStringLiteral("#12000000"));
    return palette;
}

bool AppController::loadDefaultSimulation()
{
    if (m_defaultSimulationLoaded) {
        return true;
    }

    if (!m_viewer) {
        emit errorOccurred(QStringLiteral("3D 视口尚未就绪"));
        return false;
    }

    m_viewer->ensureInitialized();
    applyTheme(m_theme);

    const QString root = assetsRoot();
    if (!QDir(root).exists()) {
        emit errorOccurred(QStringLiteral("assets 目录不存在: %1").arg(root));
        return false;
    }

    QString error;
    if (!m_viewer->loadSimulationScene(root, &error)) {
        emit errorOccurred(error.isEmpty() ? QStringLiteral("仿真场景加载失败") : error);
        return false;
    }

    const auto pose = m_viewer->defaultCoilPose();
    setCoilPoseInternal(static_cast<int>(std::lround(pose.x)),
                        static_cast<int>(std::lround(pose.y)),
                        static_cast<int>(std::lround(pose.z)),
                        static_cast<int>(std::lround(pose.rx)),
                        static_cast<int>(std::lround(pose.ry)),
                        static_cast<int>(std::lround(pose.rz)),
                        false);
    m_viewer->setCoilModelVisible(false);
    m_coilModelVisible = false;
    emit coilModelVisibleChanged();
    m_viewer->setFpsOverlayVisible(m_fpsOverlay);
    applyLutRange(m_lutMin, m_lutMax);
    m_viewer->resetCamera();
    setStatusMessage(QStringLiteral("电场仿真就绪 · 蓝红连续热力图"));
    m_defaultSimulationLoaded = true;
    emit simulationReady();
    return true;
}

void AppController::openFileDialog()
{
    QWidget *parentWidget = nullptr;
    const auto topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget *widget : topLevelWidgets) {
        if (widget->isVisible()) {
            parentWidget = widget;
            break;
        }
    }

    const QString filePath = QFileDialog::getOpenFileName(
        parentWidget,
        QStringLiteral("打开模型"),
        QString(),
        QStringLiteral("3D 模型 (*.ctm *.obj);;所有文件 (*.*)"));
    if (!filePath.isEmpty()) {
        openFile(filePath);
    }
}

void AppController::openFile(const QString &filePath)
{
    if (!m_viewer) {
        emit errorOccurred(QStringLiteral("3D 视口尚未就绪"));
        return;
    }

    m_viewer->ensureInitialized();

    ModelLoader loader;
    loader.setSmoothNormalsEnabled(m_smoothNormals);
    QString error;
    if (!loader.loadModel(filePath, &error)) {
        emit errorOccurred(error.isEmpty() ? QStringLiteral("模型加载失败") : error);
        return;
    }

    m_lastUserModelPath = filePath;

    m_viewer->setModel(loader.polyData());
    const QString name = QFileInfo(filePath).fileName();
    setStatusMessage(QStringLiteral("附加模型 %1  ·  顶点 %2  ·  三角面 %3")
                         .arg(name)
                         .arg(loader.vertexCount())
                         .arg(loader.triangleCount()));
}

void AppController::resetCamera()
{
    if (m_viewer) {
        m_viewer->resetCamera();
    }
}

void AppController::setStatusMessage(const QString &message)
{
    if (m_statusMessage == message) {
        return;
    }
    m_statusMessage = message;
    emit statusMessageChanged();
}

void AppController::setThemeIndex(int index)
{
    const AppTheme::ThemeId theme = themeFromIndex(index);
    if (m_theme == theme) {
        return;
    }
    m_theme = theme;
    applyTheme(theme);
    emit themeChanged();
}

void AppController::applyTheme(AppTheme::ThemeId theme)
{
    if (!m_viewer) {
        return;
    }
    const AppTheme::ViewportColors colors = AppTheme::viewportColors(theme);
    m_viewer->setViewportColors(const_cast<double *>(colors.bg1), const_cast<double *>(colors.bg2));
}

void AppController::setSimulationEnabled(bool enabled)
{
    if (m_simulationEnabled == enabled) {
        return;
    }
    m_simulationEnabled = enabled;
    if (m_viewer) {
        m_viewer->setSimulationEnabled(enabled);
    }
    emit simulationEnabledChanged();
}

void AppController::setIntensity(int value)
{
    value = std::clamp(value, 0, 100);
    if (m_intensity == value) {
        return;
    }
    m_intensity = value;
    if (m_viewer) {
        m_viewer->setSimulateIntensity(value);
    }
    emit intensityChanged();
}

void AppController::setHeadOpacity(int value)
{
    value = std::clamp(value, 0, 100);
    if (m_headOpacity == value) {
        return;
    }
    m_headOpacity = value;
    if (m_viewer) {
        m_viewer->setHeadOpacity(value / 100.0);
    }
    emit headOpacityChanged();
}

void AppController::setHeadMaterialIndex(int index)
{
    index = std::clamp(index, 0, HeadMaterial::materialNames().size() - 1);
    if (m_headMaterialIndex == index) {
        return;
    }
    m_headMaterialIndex = index;
    if (m_viewer) {
        m_viewer->setHeadMaterialIndex(index);
    }
    emit headMaterialIndexChanged();
}

void AppController::setSmoothNormals(bool enabled)
{
    if (m_smoothNormals == enabled) {
        return;
    }
    m_smoothNormals = enabled;
    if (m_viewer) {
        m_viewer->setSmoothNormalsEnabled(enabled);
    }
    if (!m_lastUserModelPath.isEmpty()) {
        openFile(m_lastUserModelPath);
    }
    emit smoothNormalsChanged();
}

void AppController::setLutMin(int value)
{
    value = std::clamp(value, 1, 998);
    if (value >= m_lutMax) {
        value = m_lutMax - 1;
    }
    if (m_lutMin == value) {
        return;
    }
    m_lutMin = value;
    applyLutRange(m_lutMin, m_lutMax);
    emit lutRangeChanged();
}

void AppController::setLutMax(int value)
{
    value = std::clamp(value, 2, 999);
    if (value <= m_lutMin) {
        value = m_lutMin + 1;
    }
    if (m_lutMax == value) {
        return;
    }
    m_lutMax = value;
    applyLutRange(m_lutMin, m_lutMax);
    emit lutRangeChanged();
}

void AppController::applyLutRange(int minValue, int maxValue)
{
    if (m_viewer) {
        m_viewer->setLutRange(minValue, maxValue);
    }
}

void AppController::setFpsOverlay(bool enabled)
{
    if (m_fpsOverlay == enabled) {
        return;
    }
    m_fpsOverlay = enabled;
    if (m_viewer) {
        m_viewer->setFpsOverlayVisible(enabled);
    }
    emit fpsOverlayChanged();
}

void AppController::setCoilModelVisible(bool visible)
{
    if (m_coilModelVisible == visible) {
        return;
    }
    m_coilModelVisible = visible;
    if (m_viewer) {
        m_viewer->setCoilModelVisible(visible);
    }
    emit coilModelVisibleChanged();
}

void AppController::setRoamEnabled(bool enabled)
{
    if (m_roamEnabled == enabled) {
        return;
    }
    m_roamEnabled = enabled;
    if (m_viewer) {
        m_viewer->setRoamEnabled(enabled);
    }
    m_coilControlsEnabled = !enabled;
    emit roamEnabledChanged();
    emit coilControlsEnabledChanged();
    setStatusMessage(enabled ? QStringLiteral("漫游模式已开启 · 线圈拍在头模附近随机运动")
                             : QStringLiteral("漫游模式已关闭 · 线圈拍已复位"));
}

void AppController::setCoilX(int value) { setCoilPoseInternal(value, m_coilY, m_coilZ, m_coilRx, m_coilRy, m_coilRz, true); }
void AppController::setCoilY(int value) { setCoilPoseInternal(m_coilX, value, m_coilZ, m_coilRx, m_coilRy, m_coilRz, true); }
void AppController::setCoilZ(int value) { setCoilPoseInternal(m_coilX, m_coilY, value, m_coilRx, m_coilRy, m_coilRz, true); }
void AppController::setCoilRx(int value) { setCoilPoseInternal(m_coilX, m_coilY, m_coilZ, value, m_coilRy, m_coilRz, true); }
void AppController::setCoilRy(int value) { setCoilPoseInternal(m_coilX, m_coilY, m_coilZ, m_coilRx, value, m_coilRz, true); }
void AppController::setCoilRz(int value) { setCoilPoseInternal(m_coilX, m_coilY, m_coilZ, m_coilRx, m_coilRy, value, true); }

void AppController::setCoilPoseInternal(int x, int y, int z, int rx, int ry, int rz, bool syncViewer)
{
    const bool changed = x != m_coilX || y != m_coilY || z != m_coilZ
                         || rx != m_coilRx || ry != m_coilRy || rz != m_coilRz;
    if (!changed) {
        return;
    }

    m_coilX = x;
    m_coilY = y;
    m_coilZ = z;
    m_coilRx = rx;
    m_coilRy = ry;
    m_coilRz = rz;

    if (syncViewer && m_viewer && !m_updatingCoilFromViewer) {
        m_viewer->setCoilPose(x, y, z, rx, ry, rz);
    }
    emit coilPoseChanged();
}

void AppController::syncCoilPose()
{
    if (m_viewer) {
        m_viewer->setCoilPose(m_coilX, m_coilY, m_coilZ, m_coilRx, m_coilRy, m_coilRz);
    }
}

void AppController::onCoilPoseFromViewer(double x, double y, double z, double rx, double ry, double rz)
{
    m_updatingCoilFromViewer = true;
    setCoilPoseInternal(static_cast<int>(std::lround(x)),
                        static_cast<int>(std::lround(y)),
                        static_cast<int>(std::lround(z)),
                        static_cast<int>(std::lround(rx)),
                        static_cast<int>(std::lround(ry)),
                        static_cast<int>(std::lround(rz)),
                        false);
    m_updatingCoilFromViewer = false;
}
