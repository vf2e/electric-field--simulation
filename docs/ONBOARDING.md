# EField Viewer 新人上手指南

30–60 分钟内完成环境搭建并成功运行 **QML 版**电场仿真查看器。

> 架构细节见 [TECHNICAL.md](./TECHNICAL.md)  
> **VTK 环境配置见 [VTK_ENVIRONMENT.md](./VTK_ENVIRONMENT.md)**  
> 测试说明见 [TESTING.md](./TESTING.md)

---

## 1. 项目是什么

**EField Viewer** 是 [ctm-viewer](D:/opensource/ctm-viewer) 的 QML 重构版，用于：

1. 展示八字拍线圈经颅磁刺激电场仿真；
2. 调节线圈 6-DOF、强度、色标、头皮透明度；
3. 加载附加 CTM/OBJ 模型。

UI 采用扁平化工业软件风格（Mag-aim Flat 设计体系）。

---

## 2. 环境要求

| 组件 | 说明 |
|------|------|
| Windows 10/11 x64 | 必须 |
| Visual Studio 2019+ | MSVC 工具链 |
| CMake ≥ 3.16 | |
| Qt 5.15.2 msvc2019_64 | 需 **Quick** 与 **Widgets** 模块 |
| VTK 8.0 + OpenCTM | 来自 magaim-qt，详见 [VTK_ENVIRONMENT.md](./VTK_ENVIRONMENT.md) |

### magaim-qt 路径

默认：`D:/2025WorkSpace/magaim-qt`

```
magaim-qt/3dLib/VTK64/
magaim-qt/3dLib/openctm/
```

---

## 3. 第一次构建

```powershell
cd "c:\Users\wangchang\Desktop\electric-field -simulation"

cmake -S . -B build `
  -DCMAKE_PREFIX_PATH="C:\qt5.15.2\5.15.2\msvc2019_64" `
  -DMAGIMAIM_QT_ROOT="D:/2025WorkSpace/magaim-qt"

cmake --build build --config Release
```

运行：

```powershell
.\build\Release\efield-viewer.exe
```

`assets/` 会在构建后自动复制到 exe 旁。

---

## 4. 目录速览

| 路径 | 职责 |
|------|------|
| `qml/main.qml` | 主窗口布局 |
| `qml/common/` | Flat 基础组件 |
| `qml/components/` | 业务面板与弹窗 |
| `src/AppController.*` | QML 与仿真桥接 |
| `src/VtkHost.*` | VTK 嵌入 QML |
| `src/VtkRenderWidget.*` | 三维场景 |
| `src/FieldSimulator.*` | 电场 IDW |

---

## 5. 修改 UI 的快速路径

1. **改颜色/间距** → 查阅 [软件设计风格规范.md](../软件设计风格规范.md)
2. **改左栏控件** → `qml/components/LeftControlPanel.qml`
3. **改主题** → `src/AppTheme.h` + `AppController::themePalette`
4. **改仿真逻辑** → `src/FieldSimulator.cpp`（一般无需动 QML）

---

## 6. 常见问题

| 现象 | 处理 |
|------|------|
| 启动白屏 | 检查 `qrc:/qml/main.qml` 是否编译进资源 |
| 视口全黑 | 确认 `onViewerReady` 已调用 `loadDefaultSimulation` |
| 模型/场数据缺失 | 确认 exe 旁 `assets/` 完整 |
| VTK 找不到 | 见 [VTK_ENVIRONMENT.md](./VTK_ENVIRONMENT.md) §9 |

---

## 7. 单元测试

```powershell
.\scripts\run_tests.ps1
```

覆盖 `FieldColorMap`、`FieldSimulator`、`ModelLoader`、`AppTheme`。
