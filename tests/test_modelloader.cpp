#include "ModelLoader.h"

#include "TestPaths.h"

#include <QFileInfo>
#include <QtTest>
#include <vtkPointData.h>

class ModelLoaderTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void loadModel_emptyPath_fails();
    void loadModel_unsupportedExtension_fails();
    void loadModel_missingFile_fails();
    void loadModel_triangleObj_succeeds();
    void loadModel_triangleObj_hasExpectedCounts();
    void loadModel_smoothNormalsDisabled_byDefault();
    void loadModel_smoothNormalsEnabled_computesNormals();

private:
    QString m_triangleObjPath;
};

void ModelLoaderTest::initTestCase()
{
    m_triangleObjPath = TestPaths::fixture("triangle.obj");
    QVERIFY2(QFileInfo::exists(m_triangleObjPath),
             qPrintable(QStringLiteral("Fixture missing: %1").arg(m_triangleObjPath)));
}

void ModelLoaderTest::loadModel_emptyPath_fails()
{
    ModelLoader loader;
    QString error;
    QVERIFY(!loader.loadModel(QString(), &error));
    QCOMPARE(error, QStringLiteral("文件路径为空"));
}

void ModelLoaderTest::loadModel_unsupportedExtension_fails()
{
    ModelLoader loader;
    QString error;
    QVERIFY(!loader.loadModel(QStringLiteral("model.stl"), &error));
    QCOMPARE(error, QStringLiteral("仅支持 .ctm 和 .obj 格式"));
}

void ModelLoaderTest::loadModel_missingFile_fails()
{
    ModelLoader loader;
    QString error;
    QVERIFY(!loader.loadModel(QStringLiteral("missing.obj"), &error));
    QVERIFY(!error.isEmpty());
}

void ModelLoaderTest::loadModel_triangleObj_succeeds()
{
    ModelLoader loader;
    QString error;
    QVERIFY(loader.loadModel(m_triangleObjPath, &error));
    QVERIFY(loader.polyData() != nullptr);
    QVERIFY(loader.vertexCount() > 0);
    QVERIFY(loader.triangleCount() > 0);
}

void ModelLoaderTest::loadModel_triangleObj_hasExpectedCounts()
{
    ModelLoader loader;
    loader.setSmoothNormalsEnabled(true);
    QVERIFY(loader.loadModel(m_triangleObjPath));
    QCOMPARE(loader.vertexCount(), 3);
    QCOMPARE(loader.triangleCount(), 1);
    QVERIFY(loader.hasNormals());
    QVERIFY(loader.polyData()->GetPointData()->GetNormals() != nullptr);
    QVERIFY(!loader.hasTexCoords());
}

void ModelLoaderTest::loadModel_smoothNormalsDisabled_byDefault()
{
    ModelLoader loader;
    QVERIFY(loader.loadModel(m_triangleObjPath));
    QVERIFY(!loader.hasNormals());
    QVERIFY(loader.polyData()->GetPointData()->GetNormals() == nullptr);
}

void ModelLoaderTest::loadModel_smoothNormalsEnabled_computesNormals()
{
    ModelLoader loader;
    loader.setSmoothNormalsEnabled(true);
    QVERIFY(loader.loadModel(m_triangleObjPath));
    QVERIFY(loader.hasNormals());
}

QTEST_APPLESS_MAIN(ModelLoaderTest)
#include "test_modelloader.moc"
