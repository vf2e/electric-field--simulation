# EField Viewer — VTK 环境配置指南

本文档说明 **本项目**（`efield-viewer`）如何获取、配置、构建与运行 VTK 8.0 三维渲染环境。  
适用于 Windows x64 + MSVC + Qt 5.15 开发/打包场景。

> 相关文档：[ONBOARDING.md](./ONBOARDING.md) · [TECHNICAL.md](./TECHNICAL.md) · [TESTING.md](./TESTING.md)

---

## 1. 总览

| 项目 | 说明 |
|------|------|
| VTK 版本 | **8.0**（固定，与 magaim-qt 预编译库一致） |
| 来源 | **magaim-qt** 仓库内的 `3dLib/VTK64` |
| 平台 | **Windows x64 only**（使用 `vtkWin32OpenGLRenderWindow`） |
| 配套库 | **OpenCTM**（`3dLib/openctm`，加载 `.ctm` 模型） |
| Qt 集成 | **不**使用 `QVTKOpenGLWidget`；VTK 以 Win32 子窗口嵌入 `VtkRenderWidget` |
| 构建系统 | CMake 3.16+，通过 `MAGIMAIM_QT_ROOT` 定位依赖 |

```
magaim-qt/
└── 3dLib/
    ├── VTK64/          ← VTK 头文件、.lib、.dll
    │   ├── include/vtk-8.0/
    │   ├── lib/
    │   └── bin/        ← 运行时 DLL（构建后复制到 exe 旁）
    └── openctm/        ← openctm.lib / openctm.dll
```

---

## 2. 前置条件

### 2.1 必需软件

| 组件 | 推荐版本 | 用途 |
|------|----------|------|
| Windows | 10 / 11 x64 | VTK Win32 渲染 |
| Visual Studio | 2019 或 2022（MSVC x64） | 与 VTK 预编译库 ABI 一致 |
| CMake | ≥ 3.16 | 工程配置 |
| Qt | **5.15.2 msvc2019_64** | UI（Quick + Widgets + QuickWidgets） |
| magaim-qt | 含 `3dLib/VTK64` | VTK + OpenCTM 二进制 |

### 2.2 Qt 模块

CMake 通过 `find_package(Qt5 ... Quick Widgets QuickWidgets)` 引入：

- `Qt5::Quick` / `Qt5::QuickWidgets` — QML 侧栏与弹窗
- `Qt5::Widgets` — `ApplicationShell`、`VtkRenderWidget` 宿主

安装 Qt 时需勾选 **MSVC 2019 64-bit** 及 **Qt Quick** 相关组件。

### 2.3 magaim-qt 获取

VTK **未随本仓库分发**，需单独准备 magaim-qt 目录（团队内部或同步自现有工作区）。

默认 CMake 缓存路径（可覆盖）：

```
D:/2025WorkSpace/magaim-qt
```

换机器后只需指向你的 magaim-qt 根目录，**不必**改源码。

---

## 3. 目录与 CMake 变量

### 3.1 关键 CMake 变量

定义于根目录 `CMakeLists.txt`：

| 变量 | 默认值 / 推导 | 含义 |
|------|----------------|------|
| `MAGIMAIM_QT_ROOT` | `D:/2025WorkSpace/magaim-qt` | magaim-qt 根目录（**首要配置项**） |
| `VTK_ROOT` | `${MAGIMAIM_QT_ROOT}/3dLib/VTK64` | VTK 安装根 |
| `VTK_VERSION` | `8.0` | 库名后缀 |
| `OPENCTM_DIR` | `${MAGIMAIM_QT_ROOT}/3dLib/openctm` | OpenCTM |
| `VTK_INCLUDE_DIR` | `${VTK_ROOT}/include/vtk-8.0` | 头文件 |
| `VTK_LIB_DIR` | `${VTK_ROOT}/lib` | 链接库 |
| `VTK_BIN_DIR` | `${VTK_ROOT}/bin` | 运行时 DLL |

配置阶段会校验：

- `${VTK_INCLUDE_DIR}/vtkPolyData.h` 存在
- `${OPENCTM_DIR}/openctm.lib` 存在

任一缺失则 CMake **FATAL_ERROR**，避免链接到错误版本。

### 3.2 链接的 VTK 模块

项目显式链接以下库（见 `CMakeLists.txt` 中 `EFIELD_VIEWER_VTK_LIBS`）：

| 类别 | 库名示例 |
|------|----------|
| 渲染 | `vtkRenderingOpenGL-8.0`, `vtkRenderingCore-8.0`, `vtkRenderingFreeType-8.0`, `vtkRenderingAnnotation-8.0` |
| 交互 | `vtkInteractionStyle-8.0` |
| IO | `vtkIOGeometry-8.0`, `vtkIOCore-8.0`, `vtkIOLegacy-8.0` |
| 滤镜 | `vtkFiltersCore/General/Geometry/Sources-8.0` |
| 通用 | `vtkCommon*` 系列, `vtksys-8.0`, `vtkzlib-8.0` 等 |

另链接：`openctm`、`opengl32`（Windows）。

### 3.3 运行时 DLL 自动复制

Release/Debug 构建完成后，CMake **POST_BUILD** 会：

1. 复制 `assets/` → `exe 目录/assets/`
2. 复制 `openctm.dll` → exe 目录
3. 复制 `VTK64/bin/*.dll` → exe 目录

因此 **Qt Creator 构建目录** 下通常可直接双击运行，无需手动拷 DLL。

---

## 4. 首次配置步骤

### 4.1 命令行（推荐验证）

```powershell
cd "C:\path\to\electric-field-simulation"

cmake -S . -B build `
  -DCMAKE_PREFIX_PATH="C:\qt5.15.2\5.15.2\msvc2019_64" `
  -DMAGIMAIM_QT_ROOT="D:/2025WorkSpace/magaim-qt" `
  -DBUILD_TESTING=ON

cmake --build build --config Release
```

成功标志：

```
build/Release/efield-viewer.exe
build/Release/assets/          # 模型与场数据
build/Release/*.dll            # VTK + openctm
```

运行：

```powershell
.\build\Release\efield-viewer.exe
```

### 4.2 Qt Creator

1. **打开项目**：选择根目录 `CMakeLists.txt`
2. **Kit**：Desktop Qt 5.15.2 MSVC2019 64bit
3. **CMake 配置**（项目 → 构建设置 → CMake）增加：

   | 键 | 值 |
   |----|-----|
   | `CMAKE_PREFIX_PATH` | `C:/qt5.15.2/5.15.2/msvc2019_64` |
   | `MAGIMAIM_QT_ROOT` | `D:/2025WorkSpace/magaim-qt` |

4. **重新配置 CMake** → **构建** → 运行 `efield-viewer`

> 勿将 `CMakeLists.txt.user` 中的本机绝对路径提交到 Git；团队统一用 CMake 缓存变量。

### 4.3 修改 magaim-qt 路径（换电脑）

**只需改一处 CMake 变量**，无需改 C++ 源码：

```powershell
cmake -S . -B build -DMAGIMAIM_QT_ROOT="E:/libs/magaim-qt"
```

或在 Qt Creator 的 CMake 配置里更新 `MAGIMAIM_QT_ROOT` 后 Re-configure。

---

## 5. 本项目中的 VTK 用法

### 5.1 模块初始化（`main.cpp`）

```cpp
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
```

必须在创建 `QApplication` 之前或程序早期调用，否则 OpenGL 渲染模块可能未注册。

### 5.2 渲染架构

```
ApplicationShell (QWidget)
├── QQuickWidget × N          # QML 面板（软件 Scene Graph）
├── VtkRenderWidget           # Qt 控件宿主
│   └── vtkWin32OpenGLRenderWindow  # Win32 子窗口，SetParentId(winId())
└── QQuickWidget (overlay)    # 全屏模态，默认隐藏
```

关键实现（`VtkRenderWidget.cpp`）：

- `Qt::WA_NativeWindow` + `Qt::WA_PaintOnScreen` — 提供原生 HWND
- `vtkWin32OpenGLRenderWindow::SetParentId(winId())` — 嵌入 Qt
- `paintEngine() == nullptr` — 由 VTK 自行绘制
- `vtkInteractorStyleTrackballCamera` — 旋转/平移/缩放

**未使用** `QVTKOpenGLWidget` / VTK 9 的 Qt Quick 集成。

### 5.3 Qt Quick 与 VTK 共存

`main.cpp` 中：

```cpp
QQuickWindow::setSceneGraphBackend("software");
```

QML 走软件渲染，避免与 VTK OpenGL 上下文冲突（否则易出现侧栏黑屏、UI 不可点）。

### 5.4 业务代码入口

| 文件 | 职责 |
|------|------|
| `VtkRenderWidget.*` | 场景加载、渲染、交互、LUT 着色 |
| `ModelLoader.*` | `.ctm` / `.obj` → `vtkPolyData` |
| `FieldSimulator.*` | 电场 bin → 顶点标量 |
| `FieldColorMap.*` | 标量 → RGB |
| `AppController.*` | QML 调用 `setViewer()`、主题背景 `setViewportColors()` |

默认资源路径（`AppController::assetsRoot()`）：

1. `exe 目录/assets/`（构建/打包后）
2. 回退 `exe 目录/../../assets/`（部分 IDE 构建树）

---

## 6. 打包与分发

使用项目脚本（会传入 `-DMAGIMAIM_QT_ROOT` 并执行 `windeployqt --qmldir qml`）：

```powershell
.\scripts\package.ps1 `
  -QtPath "C:\qt5.15.2\5.15.2\msvc2019_64" `
  -MagaimQtRoot "D:/2025WorkSpace/magaim-qt"
```

输出目录 `dist/efield-viewer-2.0.0-win64/` 应包含：

| 内容 | 说明 |
|------|------|
| `efield-viewer.exe` | 主程序 |
| `assets/` | 模型与 `b9076_efield.bin` |
| `Qt5*.dll` | windeployqt 部署 |
| `QtQuick/`、`QtQuick.2/` 等 | QML 模块（**必须**，否则 UI 无法加载） |
| `vtk*.dll` | 构建时已复制；打包脚本从 build 目录取 |
| `openctm.dll` | CTM 加载 |

在**无开发环境**的机器上运行前，确认上述文件齐全；**不需要**单独安装 VTK SDK。

---

## 7. 单元测试与 VTK

`tests/` 中链接 `VtkRenderWidget.cpp` 的用例（如 `test_apptcontroller`）同样依赖 VTK DLL。

```powershell
.\scripts\run_tests.ps1 -MagaimQtRoot "D:/2025WorkSpace/magaim-qt"
```

CTest 会通过 `PATH` prepend 注入 Qt、VTK、OpenCTM 的 bin 目录。

纯数据层测试（`test_fieldcolormap`、`test_fieldsimulator`、`test_modelloader`）不启动 GUI，但仍链接 VTK 库以保持一致 ABI。

---

## 8. 环境自检清单

配置完成后逐项确认：

- [ ] `magaim-qt/3dLib/VTK64/include/vtk-8.0/vtkPolyData.h` 存在
- [ ] `magaim-qt/3dLib/VTK64/lib/vtkRenderingOpenGL-8.0.lib` 存在
- [ ] `magaim-qt/3dLib/VTK64/bin/` 下有大量 `vtk*.dll`
- [ ] `magaim-qt/3dLib/openctm/openctm.lib` 与 `openctm.dll` 存在
- [ ] CMake 配置无 `VTK headers not found` 报错
- [ ] 构建成功生成 `efield-viewer.exe`
- [ ] exe 同目录有 `assets/`、`openctm.dll`、若干 `vtk*.dll`
- [ ] 启动后 3D 视口可见头/脑模型（非纯黑）
- [ ] 左键旋转、滚轮缩放正常

---

## 9. 常见问题

### 9.1 CMake：`VTK headers not found`

**原因**：`MAGIMAIM_QT_ROOT` 错误或 magaim-qt 不完整。

**处理**：

```powershell
cmake -S . -B build -DMAGIMAIM_QT_ROOT="正确路径"
```

确认存在 `3dLib/VTK64/include/vtk-8.0/vtkPolyData.h`。

### 9.2 链接错误：`无法解析的外部符号 vtkXXX`

**原因**：VTK 库版本与头文件不一致，或漏链模块。

**处理**：

- 确认 `VTK_VERSION` 为 `8.0`，库名带 `-8.0` 后缀
- 不要混用 VTK 9 或其他编译器生成的 lib
- 清理 build 目录后重新 `cmake` + 全量构建

### 9.3 运行时报错：找不到 `vtkXXX-8.0.dll` 或 `openctm.dll`

**原因**：DLL 未在 exe 同目录或 PATH 中。

**处理**：

- 重新构建（POST_BUILD 会复制）
- 或手动从 `VTK64/bin`、`openctm/` 复制到 exe 目录
- 用 Dependencies 等工具查看缺失 DLL

### 9.4 启动后 3D 全黑，但 UI 正常

**可能原因**：

- `assets/` 未复制 → 场景加载失败，查看控制台 `errorOccurred`
- VTK 初始化时控件尺寸为 0 → 调整窗口大小或确认 `ensureInitialized()` 被调用
- 显卡/OpenGL 驱动问题 → 更新驱动；本项目 `SetMultiSamples(0)` 已禁用 MSAA

### 9.5 UI 黑屏但 3D 正常

**原因**：QML 与 VTK OpenGL 冲突。

**处理**：确认 `main.cpp` 保留 `QQuickWindow::setSceneGraphBackend("software")`；不要用默认 OpenGL Scene Graph。

### 9.6 打包后 QML 报错 `QtQuick.Controls is not installed`

**原因**：`windeployqt` 未扫描 QML 依赖。

**处理**：使用项目 `scripts/package.ps1`（已带 `--qmldir qml`），或手动：

```powershell
windeployqt --release --qmldir qml path\to\efield-viewer.exe
```

### 9.7 能否改用 VTK 9 / 自行编译 VTK？

**当前不建议**。本项目与 magaim-qt 预编译 **VTK 8.0 + MSVC** 深度绑定：

- 使用 `vtkWin32OpenGLRenderWindow`（VTK 8 Windows 路径）
- 库名、模块划分与 9.x 不同
- 升级需同步修改 CMake 库列表、渲染初始化及全部 DLL

若必须升级，建议在独立分支验证 `VtkRenderWidget` 与 `ModelLoader` 全流程后再合并。

---

## 10. 与 Qt 路径的关系（易混淆）

| 依赖 | 配置方式 | 是否写死在源码 |
|------|----------|----------------|
| **VTK + OpenCTM** | `MAGIMAIM_QT_ROOT` | 仅 CMake 默认值，可覆盖 |
| **Qt** | `CMAKE_PREFIX_PATH` | 否 |
| **assets** | 构建复制 + `AppController::assetsRoot()` | 相对 exe 路径 |

脚本默认（可参数覆盖）见 `scripts/package.ps1`、`scripts/run_tests.ps1`。

---

## 11. 快速参考命令

```powershell
# 配置
cmake -S . -B build `
  -DCMAKE_PREFIX_PATH="C:\qt5.15.2\5.15.2\msvc2019_64" `
  -DMAGIMAIM_QT_ROOT="D:/2025WorkSpace/magaim-qt"

# 构建
cmake --build build --config Release

# 测试
.\scripts\run_tests.ps1 -MagaimQtRoot "D:/2025WorkSpace/magaim-qt"

# 打包
.\scripts\package.ps1 -MagaimQtRoot "D:/2025WorkSpace/magaim-qt"
```

---

*文档版本：1.0 · 对应 efield-viewer 2.0.0 · VTK 8.0 / magaim-qt VTK64*
