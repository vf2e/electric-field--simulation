#ifndef HEADMATERIAL_H
#define HEADMATERIAL_H

#include <QString>
#include <QStringList>

class vtkActor;
class vtkRenderWindow;

namespace HeadMaterial {

enum class Id {
    Default = 0,
    SssSkin,
    ClinicalWax,
    Porcelain,
    SoftTissue,
    SilkMatte,
    Count
};

struct UniformProfile {
    double baseColor[3];
    double specColor[3];
    double ambient;
    double diffuse;
    double specular;
    double shininess;
    double wrap;
    double rimPower;
    double rimStrength;
};

QStringList materialNames();
Id idFromIndex(int index);
int indexFromId(Id id);
const UniformProfile &profile(Id id);

void apply(vtkActor *actor, vtkRenderWindow *renderWindow, Id material, double opacity);

} // namespace HeadMaterial

#endif
