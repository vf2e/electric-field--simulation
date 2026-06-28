# EField Viewer 技术说明

本文档描述 **QML 重构版** EField Viewer 的架构、模块划分与扩展方式。算法与 VTK 渲染细节与 [ctm-viewer TECHNICAL.md](D:/opensource/ctm-viewer/docs/TECHNICAL.md) 保持一致，此处重点说明 **Qt Quick 分层**。

---

## 1. 技术栈

| 层级 | 技术 |
|------|------|
| UI | Qt 5.15 Quick / QML |
| UI ↔ 仿真 | `AppController` (QObject) |
| 3D 嵌入 | `VtkHost` + `QWidget::createWindowContainer` |
| 渲染 | VTK 8.0 Win32 OpenGL |
| 仿真 | `FieldSimulator` / `FieldColorMap` |
| 构建 | CMake 3.16+ / C++17 |

---

## 2. 总体架构

```
┌─────────────────────────────────────────────────────────────┐
│  Presentation (QML)                                          │
│  main.qml · LeftControlPanel · ColorBarPanel · SettingsDialog│
├─────────────────────────────────────────────────────────────┤
│  Bridge (C++)                                                │
│  AppController · VtkHost                                     │
├─────────────────────────────────────────────────────────────┤
│  Rendering + Simulation (C++)                                │
│  VtkRenderWidget · FieldSimulator · ModelLoader              │
└─────────────────────────────────────────────────────────────┘
```

### 2.1 QML 入口

`main.qml` 创建 `ApplicationWindow`，通过全局上下文属性 `App` 访问 `AppController`：

```qml
VtkHost {
    onViewerReady: {
        App.setViewer(viewer)
        App.loadDefaultSimulation()
    }
}
```

### 2.2 AppController

暴露给 QML 的属性与 invokable 方法：

| 类别 | 成员 |
|------|------|
| 状态 | `statusMessage`, `themeIndex`, `simulationEnabled`, `intensity`, `headOpacity`, `lutMin/Max` |
| 显示 | `fpsOverlay`, `coilModelVisible`, `roamEnabled` |
| 线圈 | `coilX/Y/Z`, `coilRx/Ry/Rz`, `coilControlsEnabled` |
| 操作 | `openFileDialog()`, `resetCamera()`, `loadDefaultSimulation()` |
| 主题 | `themePalette(index)`, `themeNames()` |

信号：`errorOccurred`, `simulationReady`, `themeChanged` 等。

### 2.3 VtkHost

`QQuickItem` 子类，在 `windowChanged` 时：

1. 创建 `VtkRenderWidget`
2. `QWidget::createWindowContainer` 生成原生容器
3. 在 `beforeRendering` 同步几何到 QML Item 位置
4. 发出 `viewerReady` 供 QML 绑定 `AppController`

> **限制**：与 ctm-viewer 相同，VTK 使用 Win32 子窗口，视口内不宜再叠 QML 控件（提示文字放在视口外缘）。

---

## 3. QML 目录

```
qml/
├── main.qml                 # ApplicationWindow、顶栏、布局
├── common/                  # Flat* 基础组件
│   ├── FlatButton.qml
│   ├── FlatCard.qml
│   ├── FlatSliderRow.qml
│   ├── FlatTextField.qml
│   ├── FlatToggle.qml
│   └── ...
└── components/
    ├── CollapsiblePanel.qml # 可折叠侧栏
    ├── LeftControlPanel.qml
    ├── ColorBarPanel.qml
    ├── ColorBar.qml         # Canvas 热力渐变
    ├── SettingsDialog.qml
    └── MessageDialog.qml
```

所有 QML 文件注册在 `resources/app.qrc`，引擎加载 `qrc:/qml/main.qml`。

---

## 4. 主题系统

- **QML 侧**：`App.themePalette(themeIndex)` 返回 `clrBg`、`clrCard`、`clrPrimary` 等键值，组件通过 `palette` 属性注入。
- **VTK 侧**：`AppController::applyTheme` 调用 `AppTheme::viewportColors` 设置视口渐变背景。
- 四套主题：晨曦白、深空夜、海洋蓝、薄荷绿（与 ctm-viewer 一致）。

---

## 5. 数据流（与 Widgets 版等价）

```
FlatSliderRow.valueChanged
  → App.coilX = value
  → AppController::setCoilX
  → VtkRenderWidget::setCoilPose
  → FieldSimulator::applyToBrain
  → render
```

---

## 6. 扩展指南

### 6.1 新增 QML 面板控件

1. 优先使用 `qml/common/Flat*` 组件；
2. 通过 `App` 读写仿真状态，勿在 QML 中直接访问 VTK；
3. 新组件加入 `resources/app.qrc`。

### 6.2 新增主题

1. 扩展 `AppTheme.h` 枚举与 `viewportColors`；
2. 在 `AppController::themePalette` 补充 QML 色板；
3. `SettingsDialog` 的 `Repeater` 自动读取 `App.themeNames()`。

### 6.3 新增仿真参数

1. 在 `VtkRenderWidget` 增加 API；
2. 在 `AppController` 暴露 `Q_PROPERTY` + setter；
3. 在 `LeftControlPanel.qml` 增加 `FlatSliderRow` 或 `FlatToggle`。

---

## 7. 已知限制

1. Windows + VTK Win32 专用（与 ctm-viewer 相同）；
2. QML 控件不可覆盖 VTK 原生 HWND 区域；
3. 需同时链接 `Qt5::Quick` 与 `Qt5::Widgets`（`createWindowContainer` 依赖 Widgets）。

---

## 8. 相关文档

- [ONBOARDING.md](./ONBOARDING.md) — 构建与运行
- [TESTING.md](./TESTING.md) — 单元测试
- [软件设计风格规范.md](../软件设计风格规范.md) — UI 规范
