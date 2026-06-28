#include "HeadMaterial.h"

#include <vtkActor.h>
#include <vtkMapper.h>
#include <vtkNew.h>
#include <vtkOpenGLProperty.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkShader2.h>
#include <vtkShader2Collection.h>
#include <vtkShaderProgram2.h>
#include <vtkSmartPointer.h>
#include <vtkWeakPointer.h>

#include <algorithm>
#include <array>

namespace HeadMaterial {

namespace {

const char *kPropVertexShader = R"GLSL(
#version 110

varying vec3 vNormal;
varying vec3 vViewDir;
varying vec3 vLightDir;

void propFuncVS()
{
    vNormal = normalize(gl_NormalMatrix * gl_Normal);
    vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;
    vViewDir = normalize(-ecPosition.xyz);
    vLightDir = normalize(gl_LightSource[0].position.xyz);
}
)GLSL";

const char *kPropFragmentShader = R"GLSL(
#version 110

uniform vec3 uBaseColor;
uniform vec3 uSpecColor;
uniform float uAmbient;
uniform float uDiffuse;
uniform float uSpecular;
uniform float uShininess;
uniform float uWrap;
uniform float uRimPower;
uniform float uRimStrength;
uniform float uOpacity;

varying vec3 vNormal;
varying vec3 vViewDir;
varying vec3 vLightDir;

void propFuncFS()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(vLightDir);
    vec3 V = normalize(vViewDir);
    vec3 H = normalize(L + V);

    float ndotl = dot(N, L);
    float wrap = clamp((ndotl + uWrap) / (1.0 + uWrap), 0.0, 1.0);
    float spec = pow(max(dot(N, H), 0.0), uShininess);
    float rim = pow(1.0 - max(dot(N, V), 0.0), uRimPower) * uRimStrength;

    vec3 ambient = uBaseColor * uAmbient * gl_LightProduct[0].ambient.rgb;
    vec3 diffuse = uBaseColor * wrap * uDiffuse * gl_LightSource[0].diffuse.rgb;
    vec3 specular = uSpecColor * spec * uSpecular * gl_LightSource[0].specular.rgb;
    vec3 rimLight = uSpecColor * rim;

    vec3 color = ambient + diffuse + specular + rimLight;
    gl_FragColor = vec4(color, uOpacity);
}
)GLSL";

const UniformProfile kDefaultPhong = {
    {0.42, 0.36, 0.32},
    {1.0, 0.98, 0.94},
    0.22,
    0.74,
    0.10,
    24.0,
    0.0,
    3.0,
    0.0,
};

const std::array<UniformProfile, static_cast<std::size_t>(Id::Count)> kProfiles = {{
    kDefaultPhong,
    {{0.78, 0.58, 0.48}, {1.0, 0.92, 0.85}, 0.28, 0.82, 0.12, 18.0, 0.55, 4.0, 0.18},
    {{0.88, 0.82, 0.68}, {1.0, 0.98, 0.90}, 0.22, 0.68, 0.45, 56.0, 0.15, 3.0, 0.08},
    {{0.95, 0.93, 0.90}, {1.0, 1.0, 1.0}, 0.15, 0.52, 0.72, 110.0, 0.05, 5.0, 0.12},
    {{0.82, 0.50, 0.44}, {1.0, 0.88, 0.82}, 0.32, 0.76, 0.18, 22.0, 0.65, 3.5, 0.22},
    {{0.64, 0.60, 0.56}, {0.85, 0.85, 0.88}, 0.38, 0.58, 0.04, 8.0, 0.35, 2.0, 0.06},
}};

vtkOpenGLProperty *openGlProperty(vtkActor *actor)
{
    if (!actor) {
        return nullptr;
    }

    vtkProperty *property = actor->GetProperty();
    vtkOpenGLProperty *glProperty = vtkOpenGLProperty::SafeDownCast(property);
    if (!glProperty) {
        glProperty = vtkOpenGLProperty::New();
        if (property) {
            glProperty->DeepCopy(property);
        }
        actor->SetProperty(glProperty);
        glProperty->Delete();
    }
    return glProperty;
}

void applyMapperOffset(vtkActor *actor, bool offsetCoincident)
{
    if (!actor) {
        return;
    }

    vtkMapper *mapper = actor->GetMapper();
    if (!mapper) {
        return;
    }

    if (offsetCoincident) {
        mapper->SetResolveCoincidentTopologyToPolygonOffset();
        mapper->SetResolveCoincidentTopologyPolygonOffsetParameters(2.0, 2.0);
    } else {
        mapper->SetResolveCoincidentTopologyToOff();
    }
}

void applyClassicPhong(vtkActor *actor, const UniformProfile &profile, double opacity)
{
    vtkOpenGLProperty *glProperty = openGlProperty(actor);
    if (!glProperty) {
        return;
    }

    glProperty->SetShading(0);
    glProperty->SetPropProgram(nullptr);
    glProperty->SetInterpolationToPhong();
    glProperty->SetColor(profile.baseColor[0], profile.baseColor[1], profile.baseColor[2]);
    glProperty->SetAmbient(profile.ambient);
    glProperty->SetDiffuse(profile.diffuse);
    glProperty->SetSpecular(profile.specular);
    glProperty->SetSpecularPower(profile.shininess);
    glProperty->SetSpecularColor(profile.specColor[0], profile.specColor[1], profile.specColor[2]);
    glProperty->SetOpacity(opacity);
    glProperty->BackfaceCullingOff();
    glProperty->LightingOn();
}

void setShaderUniforms(vtkOpenGLProperty *property, const UniformProfile &profile, double opacity)
{
    float baseColor[3] = {
        static_cast<float>(profile.baseColor[0]),
        static_cast<float>(profile.baseColor[1]),
        static_cast<float>(profile.baseColor[2]),
    };
    float specColor[3] = {
        static_cast<float>(profile.specColor[0]),
        static_cast<float>(profile.specColor[1]),
        static_cast<float>(profile.specColor[2]),
    };
    float ambient = static_cast<float>(profile.ambient);
    float diffuse = static_cast<float>(profile.diffuse);
    float specular = static_cast<float>(profile.specular);
    float shininess = static_cast<float>(profile.shininess);
    float wrap = static_cast<float>(profile.wrap);
    float rimPower = static_cast<float>(profile.rimPower);
    float rimStrength = static_cast<float>(profile.rimStrength);
    float opacityValue = static_cast<float>(opacity);

    property->AddShaderVariable("uBaseColor", 3, baseColor);
    property->AddShaderVariable("uSpecColor", 3, specColor);
    property->AddShaderVariable("uAmbient", 1, &ambient);
    property->AddShaderVariable("uDiffuse", 1, &diffuse);
    property->AddShaderVariable("uSpecular", 1, &specular);
    property->AddShaderVariable("uShininess", 1, &shininess);
    property->AddShaderVariable("uWrap", 1, &wrap);
    property->AddShaderVariable("uRimPower", 1, &rimPower);
    property->AddShaderVariable("uRimStrength", 1, &rimStrength);
    property->AddShaderVariable("uOpacity", 1, &opacityValue);
}

vtkShaderProgram2 *sharedShaderProgram(vtkRenderWindow *renderWindow)
{
    static vtkSmartPointer<vtkShaderProgram2> program;
    static vtkWeakPointer<vtkRenderWindow> context;

    if (program && context.GetPointer() == renderWindow) {
        return program;
    }

    if (!renderWindow || !vtkShaderProgram2::IsSupported(renderWindow)) {
        return nullptr;
    }

    vtkNew<vtkShader2> vertexShader;
    vertexShader->SetContext(renderWindow);
    vertexShader->SetType(VTK_SHADER_TYPE_VERTEX);
    vertexShader->SetSourceCode(kPropVertexShader);

    vtkNew<vtkShader2> fragmentShader;
    fragmentShader->SetContext(renderWindow);
    fragmentShader->SetType(VTK_SHADER_TYPE_FRAGMENT);
    fragmentShader->SetSourceCode(kPropFragmentShader);

    program = vtkSmartPointer<vtkShaderProgram2>::New();
    program->SetContext(renderWindow);
    program->SetPrintErrors(false);
    program->GetShaders()->AddItem(vertexShader.GetPointer());
    program->GetShaders()->AddItem(fragmentShader.GetPointer());
    context = renderWindow;
    return program;
}

void applyCustomShader(vtkActor *actor,
                       vtkRenderWindow *renderWindow,
                       const UniformProfile &profile,
                       double opacity)
{
    vtkOpenGLProperty *glProperty = openGlProperty(actor);
    if (!glProperty) {
        return;
    }

    vtkShaderProgram2 *program = sharedShaderProgram(renderWindow);
    if (!program) {
        applyClassicPhong(actor, profile, opacity);
        return;
    }

    glProperty->SetInterpolationToPhong();
    glProperty->SetColor(profile.baseColor[0], profile.baseColor[1], profile.baseColor[2]);
    glProperty->SetOpacity(opacity);
    glProperty->BackfaceCullingOff();
    glProperty->LightingOn();
    glProperty->SetPropProgram(program);
    glProperty->SetShading(1);
    setShaderUniforms(glProperty, profile, opacity);
}

} // namespace

QStringList materialNames()
{
    return {
        QStringLiteral("经典 Phong"),
        QStringLiteral("次表面皮肤"),
        QStringLiteral("临床蜡质"),
        QStringLiteral("瓷白高光"),
        QStringLiteral("软组织"),
        QStringLiteral("丝绒哑光"),
    };
}

Id idFromIndex(int index)
{
    if (index < 0 || index >= static_cast<int>(Id::Count)) {
        return Id::Default;
    }
    return static_cast<Id>(index);
}

int indexFromId(Id id)
{
    return static_cast<int>(id);
}

const UniformProfile &profile(Id material)
{
    const std::size_t index = static_cast<std::size_t>(material);
    if (index >= kProfiles.size()) {
        return kProfiles[0];
    }
    return kProfiles[index];
}

void apply(vtkActor *actor, vtkRenderWindow *renderWindow, Id material, double opacity)
{
    if (!actor) {
        return;
    }

    const double clampedOpacity = std::clamp(opacity, 0.0, 1.0);
    const UniformProfile &materialProfile = profile(material);

    if (material == Id::Default) {
        applyClassicPhong(actor, materialProfile, clampedOpacity);
    } else {
        applyCustomShader(actor, renderWindow, materialProfile, clampedOpacity);
    }

    applyMapperOffset(actor, true);
}

} // namespace HeadMaterial
