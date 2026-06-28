# EField Viewer 测试规范

本文档定义本项目的测试策略、用例矩阵、运行方式与发版门禁。设计走查见 [`UI_DESIGN_SYSTEM.md`](./UI_DESIGN_SYSTEM.md) 第 10 节。

---

## 1. 测试金字塔

```
                    ┌─────────────┐
                    │  手工冒烟   │  发版前必做（§6）
                    └──────┬──────┘
               ┌───────────┴───────────┐
               │   UI 视觉走查清单      │  Design QA（§6.2）
               └───────────┬───────────┘
          ┌────────────────┴────────────────┐
          │     集成 / 控制器单测            │  test_apptcontroller
          └────────────────┬────────────────┘
     ┌─────────────────────┴─────────────────────┐
     │           数据层单元测试（核心）            │  4 个 legacy + theme
     └───────────────────────────────────────────┘
```

| 层级 | 工具 | 目标覆盖率 |
|------|------|------------|
| 数据层单测 | Qt Test | 核心算法 100% 行覆盖（FieldColorMap / FieldSimulator / ModelLoader） |
| 控制器单测 | Qt Test | 主题/LUT/信号契约 |
| UI 走查 | 人工 | 100% 关键路径 |
| 3D 渲染 | 人工 | 默认场景 + 交互 |

### 1.1 为什么不测 GUI 自动化

- `VtkRenderWidget` 使用 Win32 原生 OpenGL 子窗口，CI 无头环境不稳定；
- QQuickWidget + VTK 混合架构暂无可靠跨平台 GUI 测试方案；
- **策略**：数据层充分单测 + Design QA 清单 + 发版冒烟。

---

## 2. 框架与目录

| 项目 | 选择 |
|------|------|
| 框架 | Qt Test (`Qt5::Test`) |
| 构建 | CMake `enable_testing()` + CTest |
| 入口 | `scripts/run_tests.ps1` / `run_tests.bat` |

```
tests/
├── CMakeLists.txt
├── fixtures/
│   └── triangle.obj
├── helpers/
│   ├── TestPaths.h
│   ├── VtkTestHelpers.h
│   └── EfieldBinaryWriter.h
├── test_fieldcolormap.cpp
├── test_fieldsimulator.cpp
├── test_modelloader.cpp
├── test_apptheme.cpp
└── test_apptcontroller.cpp      ← 新增
```

---

## 3. 构建与运行

### 3.1 一键运行（推荐）

```powershell
.\scripts\run_tests.ps1
```

### 3.2 CTest

```powershell
ctest --test-dir build -C Release --output-on-failure
```

### 3.3 编译全部测试目标

```powershell
cmake --build build --config Release `
  --target test_fieldcolormap test_fieldsimulator test_modelloader `
           test_apptheme test_apptcontroller
```

---

## 4. 测试用例矩阵

### 4.1 `test_fieldcolormap` — 色标映射

| 用例 | 验证点 | 优先级 |
|------|--------|--------|
| `brainGrayRgb_matchesConstant` | 脑灰 RGB = 148 | P0 |
| `setRange_enforcesMinimumSpan` | max ≥ min + 1 | P0 |
| `colorForValue_belowMin_returnsBrainGray` | 低于下限 → 灰 | P0 |
| `colorForValue_atMin_returnsBrainGray` | 等于下限 → 灰 | P0 |
| `colorForValue_aboveMax_clampsToHeatMax` | 超高钳制 | P0 |
| `colorForValue_midRange_differsFromGray` | 中间非灰 | P1 |
| `colorForNormalized_endpoints_areStable` | t=0/1 稳定 | P1 |
| `colorForNormalized_isMonotonicInRedNearHighEnd` | 高段红色单调 | P1 |
| `defaultRange_matchesProductDefaults` | 默认 40–150 V/m | P0 |

### 4.2 `test_fieldsimulator` — 电场仿真

| 用例 | 验证点 | 优先级 |
|------|--------|--------|
| `loadBinary_missingFile_fails` | 文件不存在 | P0 |
| `loadBinary_truncatedHeader_fails` | 头过短 | P0 |
| `loadBinary_truncatedPayload_fails` | 数据不符 | P0 |
| `loadBinary_validFile_succeeds` | 合法 bin | P0 |
| `hasData_reflectsLoadState` | 状态一致 | P0 |
| `isCoilWithinRange_handlesNullAndDistance` | 距离阈值 | P1 |
| `resetBrainColors_setsUniformGray` | 重置灰色 | P0 |
| `applyToBrain_colorsVerticesWithFieldData` | 着色非灰 | P0 |
| `applyToBrain_respectsIntensityScale` | 强度影响 | P1 |
| `applyToBrain_emptySamples_isNoOp` | 空样本无操作 | P1 |

### 4.3 `test_modelloader` — 模型加载

| 用例 | 验证点 | 优先级 |
|------|--------|--------|
| `loadModel_emptyPath_fails` | 空路径 | P0 |
| `loadModel_unsupportedExtension_fails` | 非 ctm/obj | P0 |
| `loadModel_missingFile_fails` | 不存在 | P0 |
| `loadModel_triangleObj_succeeds` | 加载 obj | P0 |
| `loadModel_triangleObj_hasExpectedCounts` | 3 顶点 1 面 | P0 |

### 4.4 `test_apptheme` — 主题配置

| 用例 | 验证点 | 优先级 |
|------|--------|--------|
| `availableThemes_hasFourEntries` | 4 套主题 | P0 |
| `viewportColors_returnsValidRgb` | RGB ∈ [0,1] | P0 |
| `stylesheet_isNonEmptyForAllThemes` | QSS 非空 | P1 |
| `settingsDialogStylesheet_containsKeySelectors` | 设置框选择器 | P1 |
| `themeAccent_isValidHexColor` | `#RRGGBB` | P0 |

### 4.5 `test_apptcontroller` — QML 桥接契约

| 用例 | 验证点 | 优先级 |
|------|--------|--------|
| `themeNames_hasFourEntries` | 主题名列表 | P0 |
| `themePalette_containsRequiredKeys` | 含 clrBg/clrScrim 等 | P0 |
| `setLutMin_clampsBelowMax` | min < max | P0 |
| `setLutMax_clampsAboveMin` | max > min | P0 |
| `setLutMin_enforcesGlobalBounds` | 1–999 边界 | P0 |
| `openSettings_emitsSignal` | settingsRequested | P0 |
| `notifyModalVisible_togglesState` | modalActive 信号 | P1 |

---

## 5. 命名与编写规范

| 类型 | 约定 | 示例 |
|------|------|------|
| 测试类 | `ModuleNameTest` | `AppControllerTest` |
| 用例 | `method_condition_expectation` | `setLutMin_clampsBelowMax` |
| 夹具 | 小写+下划线 | `triangle.obj` |

**编写原则**

- 一个用例只验证一个行为；
- 优先边界：空输入、截断、极值；
- 禁止依赖网络、随机数（除非固定种子）；
- 临时文件用 `QTemporaryDir`；
- 新增 QML 桥接 API → 同步追加 `test_apptcontroller` 用例。

### 5.1 新增测试步骤

1. 创建 `tests/test_xxx.cpp`；
2. 在 `tests/CMakeLists.txt` 末尾 `ctm_viewer_add_test(test_xxx)`；
3. 若链接 `AppController`，确保 `CTM_TEST_CORE_SOURCES` 含依赖；
4. 更新本文档 §4 用例矩阵。

---

## 6. 发版门禁

### 6.1 自动化门禁（必须通过）

```powershell
ctest --test-dir build -C Release --output-on-failure
# 期望：5/5 passed
```

### 6.2 UI / Design QA 清单

**布局与视觉**
- [ ] 顶栏、侧栏、色标、状态栏背景正确（非黑屏）
- [ ] 色标面板：分区标签 + 圆角色条 + 高/低标注 + 范围输入对齐
- [ ] 设置弹窗：**整窗居中**，520px 宽，主题卡 2×2 网格
- [ ] 消息弹窗：整窗居中，400px 宽，图标+标题+知道了
- [ ] 取消设置弹窗可恢复进入前的主题

**交互**
- [ ] 无弹窗时 3D 视口可旋转/缩放
- [ ] 有弹窗时遮罩覆盖全窗，Esc/点击外部可关闭
- [ ] LUT 上下限修改后色条与脑模着色同步

**功能冒烟**
- [ ] 启动后默认场景正常
- [ ] 线圈滑条实时更新
- [ ] 漫游模式开关
- [ ] 打开 OBJ/CTM
- [ ] 四套主题切换

### 6.3 CI 模板（GitHub Actions / Azure Pipelines）

```yaml
- name: Configure
  run: >
    cmake -S . -B build
    -DCMAKE_PREFIX_PATH=${{ env.QT_PATH }}
    -DMAGIMAIM_QT_ROOT=${{ env.MAGIMAIM_QT_ROOT }}
    -DBUILD_TESTING=ON

- name: Build & Test
  run: |
    cmake --build build --config Release
    ctest --test-dir build -C Release --output-on-failure
```

---

## 7. 故障排查

| 现象 | 原因 | 处理 |
|------|------|------|
| `Fixture missing: triangle.obj` | 工作目录错误 | 用 CTest / `run_tests.ps1` |
| 链接缺少 VTK | 未配置主工程 | 先 `cmake -S . -B build` |
| test_apptcontroller 链接失败 | 缺 VtkRenderWidget | 确认 `CMakeLists.txt` 含完整 CORE_SOURCES |
| 单测全过但 UI 异常 | 渲染层未覆盖 | 执行 §6.2 Design QA |

---

## 8. 覆盖率目标（Roadmap）

| 模块 | 当前 | 目标 |
|------|------|------|
| FieldColorMap | 高 | 维持 |
| FieldSimulator | 高 | 维持 |
| ModelLoader | 中（无 CTM） | 增加 CTM fixture |
| AppController | 中 | LUT + 主题 + 信号 |
| ApplicationShell | 无 | 暂不自动化 |

---

*文档版本：2.0 · 与 5 项 CTest 用例同步。*
