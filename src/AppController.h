#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include "AppTheme.h"

#include <QObject>
#include <QString>
#include <QVariantMap>

class VtkRenderWidget;

class AppController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString statusMessage READ statusMessage WRITE setStatusMessage NOTIFY statusMessageChanged)
    Q_PROPERTY(int themeIndex READ themeIndex WRITE setThemeIndex NOTIFY themeChanged)
    Q_PROPERTY(bool simulationEnabled READ simulationEnabled WRITE setSimulationEnabled NOTIFY simulationEnabledChanged)
    Q_PROPERTY(int intensity READ intensity WRITE setIntensity NOTIFY intensityChanged)
    Q_PROPERTY(int headOpacity READ headOpacity WRITE setHeadOpacity NOTIFY headOpacityChanged)
    Q_PROPERTY(int lutMin READ lutMin WRITE setLutMin NOTIFY lutRangeChanged)
    Q_PROPERTY(int lutMax READ lutMax WRITE setLutMax NOTIFY lutRangeChanged)
    Q_PROPERTY(bool fpsOverlay READ fpsOverlay WRITE setFpsOverlay NOTIFY fpsOverlayChanged)
    Q_PROPERTY(bool coilModelVisible READ coilModelVisible WRITE setCoilModelVisible NOTIFY coilModelVisibleChanged)
    Q_PROPERTY(bool roamEnabled READ roamEnabled WRITE setRoamEnabled NOTIFY roamEnabledChanged)
    Q_PROPERTY(bool coilControlsEnabled READ coilControlsEnabled NOTIFY coilControlsEnabledChanged)
    Q_PROPERTY(int coilX READ coilX WRITE setCoilX NOTIFY coilPoseChanged)
    Q_PROPERTY(int coilY READ coilY WRITE setCoilY NOTIFY coilPoseChanged)
    Q_PROPERTY(int coilZ READ coilZ WRITE setCoilZ NOTIFY coilPoseChanged)
    Q_PROPERTY(int coilRx READ coilRx WRITE setCoilRx NOTIFY coilPoseChanged)
    Q_PROPERTY(int coilRy READ coilRy WRITE setCoilRy NOTIFY coilPoseChanged)
    Q_PROPERTY(int coilRz READ coilRz WRITE setCoilRz NOTIFY coilPoseChanged)
    Q_PROPERTY(bool modalActive READ modalActive NOTIFY modalActiveChanged)

public:
    explicit AppController(QObject *parent = nullptr);

    void setViewer(VtkRenderWidget *viewer);

    QString statusMessage() const { return m_statusMessage; }
    int themeIndex() const { return static_cast<int>(m_theme); }
    bool simulationEnabled() const { return m_simulationEnabled; }
    int intensity() const { return m_intensity; }
    int headOpacity() const { return m_headOpacity; }
    int lutMin() const { return m_lutMin; }
    int lutMax() const { return m_lutMax; }
    bool fpsOverlay() const { return m_fpsOverlay; }
    bool coilModelVisible() const { return m_coilModelVisible; }
    bool roamEnabled() const { return m_roamEnabled; }
    bool coilControlsEnabled() const { return m_coilControlsEnabled; }
    int coilX() const { return m_coilX; }
    int coilY() const { return m_coilY; }
    int coilZ() const { return m_coilZ; }
    int coilRx() const { return m_coilRx; }
    int coilRy() const { return m_coilRy; }
    int coilRz() const { return m_coilRz; }
    bool modalActive() const { return m_modalActive; }

    Q_INVOKABLE bool loadDefaultSimulation();
    Q_INVOKABLE void openFileDialog();
    Q_INVOKABLE void openFile(const QString &filePath);
    Q_INVOKABLE void resetCamera();
    Q_INVOKABLE QString assetsRoot() const;
    Q_INVOKABLE QStringList themeNames() const;
    Q_INVOKABLE QString themeAccent(int index) const;
    Q_INVOKABLE QString themeSwatchGradient(int index) const;
    Q_INVOKABLE QVariantMap themePalette(int index) const;
    Q_INVOKABLE void openSettings();
    Q_INVOKABLE void notifyModalVisible(bool visible);

public slots:
    void setStatusMessage(const QString &message);
    void setThemeIndex(int index);
    void setSimulationEnabled(bool enabled);
    void setIntensity(int value);
    void setHeadOpacity(int value);
    void setLutMin(int value);
    void setLutMax(int value);
    void setFpsOverlay(bool enabled);
    void setCoilModelVisible(bool visible);
    void setRoamEnabled(bool enabled);
    void setCoilX(int value);
    void setCoilY(int value);
    void setCoilZ(int value);
    void setCoilRx(int value);
    void setCoilRy(int value);
    void setCoilRz(int value);
    void syncCoilPose();

signals:
    void statusMessageChanged();
    void themeChanged();
    void simulationEnabledChanged();
    void intensityChanged();
    void headOpacityChanged();
    void lutRangeChanged();
    void fpsOverlayChanged();
    void coilModelVisibleChanged();
    void roamEnabledChanged();
    void coilControlsEnabledChanged();
    void coilPoseChanged();
    void errorOccurred(const QString &message);
    void settingsRequested();
    void modalActiveChanged();
    void simulationReady();

private:
    void applyTheme(AppTheme::ThemeId theme);
    void applyLutRange(int minValue, int maxValue);
    void setCoilPoseInternal(int x, int y, int z, int rx, int ry, int rz, bool syncViewer);
    void onCoilPoseFromViewer(double x, double y, double z, double rx, double ry, double rz);

    VtkRenderWidget *m_viewer = nullptr;
    QString m_statusMessage;
    AppTheme::ThemeId m_theme = AppTheme::ThemeId::Light;
    bool m_simulationEnabled = true;
    int m_intensity = 50;
    int m_headOpacity = 50;
    int m_lutMin = 40;
    int m_lutMax = 150;
    bool m_fpsOverlay = true;
    bool m_coilModelVisible = false;
    bool m_roamEnabled = false;
    bool m_coilControlsEnabled = true;
    int m_coilX = 0;
    int m_coilY = 0;
    int m_coilZ = 120;
    int m_coilRx = -20;
    int m_coilRy = 0;
    int m_coilRz = 0;
    bool m_updatingCoilFromViewer = false;
    bool m_defaultSimulationLoaded = false;
    bool m_modalActive = false;
};

#endif
