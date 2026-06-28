# EField Viewer

基于 **Qt Quick (QML)** 重构的八字拍（YRD-B9076 / B090C）电场仿真查看器，自 [ctm-viewer](D:/opensource/ctm-viewer) 抽离核心算法，UI 采用 Mag-aim Flat 设计体系，面向工业级三维仿真交互场景。

## 功能

- **默认灰白脑模** + 半透明头皮 + 八字拍线圈（默认隐藏，坐标轴示意）
- **电场仿真**：预计算场数据 IDW 插值着色（与 aimQ 算法一致）
- 线圈 **6 自由度**滑条、刺激强度、LUT 上下限
- 右侧连续色标条（灰 → 蓝 → 青 → 黄 → 红）
- 可折叠左右面板、四套主题（QML 设置弹窗）
- 漫游模式、FPS 叠加、打开附加 `.ctm` / `.obj`
- 现代化 QML 界面：顶栏品牌区、卡片浮层、Flat 组件库

## 与 ctm-viewer 的差异

| 方面 | ctm-viewer | EField Viewer (本项目) |
|------|------------|------------------------|
| UI 框架 | Qt Widgets + QSS | **Qt Quick / QML** |
| 主窗口 | `MainWindow` | `ApplicationWindow` + `AppController` |
| 侧栏 | `FloatPanel` (Widgets) | `CollapsiblePanel` (QML) |
| 色标 | `ColorBarWidget` (QPainter) | `ColorBar` (QML Canvas) |
| 3D 视口 | 中央 `VtkRenderWidget` | `VtkHost` 嵌入 QML |
| 主题 | QSS 切换 | QML 调色板 + VTK 背景联动 |

## 文档

| 文档 | 说明 |
|------|------|
| [docs/ONBOARDING.md](docs/ONBOARDING.md) | 新人上手指南 |
| [docs/TECHNICAL.md](docs/TECHNICAL.md) | 技术说明（QML 架构、VTK 嵌入、电场算法） |
| [docs/TESTING.md](docs/TESTING.md) | 单元测试 |
| [软件设计风格规范.md](软件设计风格规范.md) | UI 色彩与控件规范（QML 版） |
| [公共组件编写规范.md](公共组件编写规范.md) | Flat 组件使用规范 |

## 构建

| 依赖 | 来源 |
|------|------|
| Qt 5.12+ (Quick, Widgets) | 自行安装 |
| VTK 8.0 | `magaim-qt/3dLib/VTK64` |
| OpenCTM | `magaim-qt/3dLib/openctm` |

```powershell
cmake -S . -B build `
  -DCMAKE_PREFIX_PATH="C:\qt5.15.2\5.15.2\msvc2019_64" `
  -DMAGIMAIM_QT_ROOT="D:/2025WorkSpace/magaim-qt" `
  -DBUILD_TESTING=ON

cmake --build build --config Release
```

输出：`build/Release/efield-viewer.exe`

## 项目结构

```
electric-field-simulation/
├── CMakeLists.txt
├── README.md
├── qml/                     # QML 界面
│   ├── main.qml
│   ├── common/              # Flat* 公共组件
│   └── components/          # 面板、弹窗、色标
├── src/
│   ├── main.cpp
│   ├── AppController.*      # QML ↔ C++ 桥接
│   ├── VtkHost.*            # VTK 嵌入 QML
│   ├── VtkRenderWidget.*    # VTK 渲染核心
│   ├── FieldSimulator.*     # 电场 IDW
│   ├── FieldColorMap.*
│   ├── ModelLoader.*
│   └── AppTheme.h
├── assets/                  # 模型、场数据、图标
├── resources/app.qrc
├── docs/
└── tests/
```

## 操作说明

| 操作 | 方式 |
|------|------|
| 旋转 / 平移 / 缩放 | 左键 / 中键 / 滚轮 |
| 打开模型 | 左栏「打开」或 `Ctrl+O` |
| 重置视角 | 左栏「重置」或 `Home` |
| 切换主题 | 左栏「设置」 |
| LUT 上下限 | 右栏输入框（V/m） |

## 参考来源

- [ctm-viewer](D:/opensource/ctm-viewer) — 原始 Widgets 实现
- `magaim-qt` — CTM/OBJ 加载、VTK 渲染
- `aimQ-Frontend` — 电场数据与着色
