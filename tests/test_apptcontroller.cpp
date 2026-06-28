#include "AppController.h"

#include <QtTest>

class AppControllerTest : public QObject
{
    Q_OBJECT

private slots:
    void themeNames_hasFourEntries();
    void headMaterialNames_hasSixEntries();
    void themePalette_containsRequiredKeys();
    void setLutMin_clampsBelowMax();
    void setLutMax_clampsAboveMin();
    void setLutMin_enforcesGlobalBounds();
    void openSettings_emitsSignal();
    void notifyModalVisible_togglesState();
};

void AppControllerTest::themeNames_hasFourEntries()
{
    AppController controller;
    QCOMPARE(controller.themeNames().size(), 4);
}

void AppControllerTest::headMaterialNames_hasSixEntries()
{
    AppController controller;
    QCOMPARE(controller.headMaterialNames().size(), 6);
    QCOMPARE(controller.headMaterialNames().first(), QStringLiteral("经典 Phong"));
}

void AppControllerTest::themePalette_containsRequiredKeys()
{
    AppController controller;
    const QVariantMap palette = controller.themePalette(0);

    const QStringList requiredKeys = {
        QStringLiteral("clrBg"),
        QStringLiteral("clrCard"),
        QStringLiteral("clrBorder"),
        QStringLiteral("clrTitle"),
        QStringLiteral("clrText"),
        QStringLiteral("clrSubText"),
        QStringLiteral("clrPrimary"),
        QStringLiteral("clrScrim"),
    };

    for (const QString &key : requiredKeys) {
        QVERIFY2(palette.contains(key), qPrintable(key));
        QVERIFY2(!palette.value(key).toString().isEmpty(), qPrintable(key));
    }
}

void AppControllerTest::setLutMin_clampsBelowMax()
{
    AppController controller;
    controller.setLutMax(100);
    controller.setLutMin(150);
    QCOMPARE(controller.lutMin(), 99);
    QCOMPARE(controller.lutMax(), 100);
}

void AppControllerTest::setLutMax_clampsAboveMin()
{
    AppController controller;
    controller.setLutMin(80);
    controller.setLutMax(50);
    QCOMPARE(controller.lutMax(), 81);
    QCOMPARE(controller.lutMin(), 80);
}

void AppControllerTest::setLutMin_enforcesGlobalBounds()
{
    AppController controller;
    controller.setLutMin(-10);
    QCOMPARE(controller.lutMin(), 1);

    controller.setLutMax(2000);
    QCOMPARE(controller.lutMax(), 999);
}

void AppControllerTest::openSettings_emitsSignal()
{
    AppController controller;
    QSignalSpy spy(&controller, &AppController::settingsRequested);
    controller.openSettings();
    QCOMPARE(spy.count(), 1);
}

void AppControllerTest::notifyModalVisible_togglesState()
{
    AppController controller;
    QSignalSpy spy(&controller, &AppController::modalActiveChanged);

    QVERIFY(!controller.modalActive());
    controller.notifyModalVisible(true);
    QCOMPARE(spy.count(), 1);
    QVERIFY(controller.modalActive());

    controller.notifyModalVisible(true);
    QCOMPARE(spy.count(), 1);

    controller.notifyModalVisible(false);
    QCOMPARE(spy.count(), 2);
    QVERIFY(!controller.modalActive());
}

QTEST_APPLESS_MAIN(AppControllerTest)
#include "test_apptcontroller.moc"
