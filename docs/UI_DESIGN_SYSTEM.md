# Mag-aim 产品设计规范（通用版）

> **适用范围**：Mag-aim 全系桌面端产品（Qt Widgets / Qt Quick / QQuickWidget 混合架构）。  
> **目标**：建立可复用、可度量、可审查的视觉与交互标准，对齐 Apple HIG、Material Design 3、Microsoft Fluent 2 的核心原则，同时保持医疗/工业场景的 **低干扰、高可读、强一致性**。  
> **组件细则**：[`公共组件编写规范.md`](./公共组件编写规范.md)  
> **测试要求**：[`TESTING.md`](./TESTING.md)

---

## 1. 设计原则

| 原则 | 说明 | 反模式 |
|------|------|--------|
| **Clarity 清晰** | 一屏一事，层级分明；主操作始终可见 | 同屏 3 种以上按钮样式 |
| **Deference 克制** | 内容优先，Chrome 退后；色块面积 ≤ 15% | 大面积渐变背景 |
| **Depth 深度** | 用 elevation、边框、间距表达层级，而非重阴影 | 多层 DropShadow 叠加 |
| **Consistency 一致** | Token 驱动，禁止页面内硬编码色值 | `#3B82F6` 散落各处 |
| **Feedback 反馈** | 每次操作 100ms 内有视觉/状态响应 | 点击无 hover/active |
| **Forgiveness 容错** | 破坏性操作二次确认；表单可撤销 | 直接删除无确认 |
| **Accessibility 可达** | 对比度 WCAG AA；键盘可达；字号 ≥ 12px | 11px 正文、低对比灰字 |

---

## 2. Design Tokens（设计令牌）

所有 UI 必须通过 `themeColors` / `App.themePalette()` 注入，**禁止**在业务 QML 中写死语义色（调试色 `#FF0000` 除外）。

### 2.1 色彩 — 语义层

| Token | 默认值（Light） | 用途 |
|-------|-----------------|------|
| `clrBg` | `#F5F8FD` | 页面/工作区背景 |
| `clrCard` | `#FFFFFF` | 卡片、顶栏、底栏 |
| `clrBorder` | `#E2E8F0` | 分割线、卡片描边 |
| `clrTitle` | `#0F172A` | 一级标题 |
| `clrText` | `#334155` | 正文、次按钮文字 |
| `clrSubText` | `#64748B` | 标签、说明、单位 |
| `clrMuted` | `#94A3B8` | 占位符、禁用态 |
| `clrPrimary` | `#3B82F6` | 主操作、选中强调 |
| `clrPrimaryHover` | `#2563EB` | 主按钮 hover |
| `clrPrimaryActive` | `#1D4ED8` | 主按钮 pressed / 选中文字 |
| `primarySoft` | `#DBEAFE` | 选中行、主题卡片选中底 |
| `primarySoftest` | `#EFF6FF` | Tab 选中、信息提示区 |
| `clrFieldBg` | `#F8FAFC` | 输入框背景 |
| `clrFieldBorder` | `#CBD5E1` | 输入框默认边框 |
| `clrFocusRing` | `#93C5FD` | 聚焦描边 |
| `clrScrim` | `#590F172A` | 模态遮罩（35% 透明黑） |
| `clrDanger` | `#EF4444` | 危险文字/图标 |
| `clrSuccess` | `#16A34A` | 成功态 |
| `clrWarning` | `#D97706` | 警告态 |

### 2.2 色彩 — 语义背景（图标容器）

| 类型 | 背景 | 前景 |
|------|------|------|
| Success | `#DCFCE7` | `#16A34A` |
| Warning | `#FEF3C7` | `#D97706` |
| Error | `#FEE2E2` | `#DC2626` |
| Info | `#EFF6FF` | `#2563EB` |

### 2.3 字体阶梯（Type Scale）

基准：**14px 正文**，使用 `font.pixelSize`，不混用 `pointSize`。

| Level | 名称 | 字号 | 字重 | 行高 | 场景 |
|-------|------|------|------|------|------|
| L1 | Display | 22 | Bold | 1.25 | 抽屉主标题 |
| L2 | Headline | 18 | Bold | 1.3 | 弹窗标题 |
| L3 | Title | 16 | SemiBold | 1.35 | 区块标题 |
| L4 | Body | 14 | Regular | 1.5 | 正文、按钮 |
| L5 | Label | 12–13 | Regular/Medium | 1.4 | 表单标签 |
| L6 | Caption | 11 | Medium | 1.35 | 分区标签、单位 |
| L7 | Micro | 10 | Regular | 1.3 | 色标「高/低」辅助 |

**字距**：Caption 级分区标签 `letterSpacing: 0.4`。

### 2.4 间距系统（4px 网格）

| Token | 值 | 用途 |
|-------|-----|------|
| `space-1` | 4 | 图标与文字、紧凑行内 |
| `space-2` | 8 | 按钮组、表单行内 |
| `space-3` | 12 | 卡片内区块、侧栏节间距 |
| `space-4` | 16 | 抽屉/页面外边距 |
| `space-5` | 20 | 弹窗底栏、顶栏内边距 |
| `space-6` | 24 | 弹窗内容边距 |
| `space-8` | 32 | 大区块分隔 |

### 2.5 圆角（Radius）

| Token | 值 | 元素 |
|-------|-----|------|
| `radius-sm` | 6 | 色条、小标签 |
| `radius-md` | 8 | 按钮、输入框 |
| `radius-lg` | 10–12 | 卡片、弹窗 |
| `radius-xl` | 14 | 可折叠侧栏外框 |
| `radius-full` | 50% | 图标圆形容器 |

### 2.6 高度（Touch Target）

| Token | 值 | 元素 |
|-------|-----|------|
| `height-control` | **38** | 按钮、输入、Toggle 行 |
| `height-header` | **56** | 顶栏、弹窗标题栏 |
| `height-status` | **32** | 状态栏 |
| `height-tab` | **36** | Tab 按钮 |

### 2.7 海拔（Elevation）

QQuickWidget 环境**默认禁用** `QtGraphicalEffects/DropShadow`（与 VTK OpenGL 冲突）。用 **1px 边框 + 浅底对比** 代替阴影。

| Level | 实现 | 场景 |
|-------|------|------|
| 0 | 无边框，与 `clrBg` 同色 | 内嵌区域 |
| 1 | `border: 1px clrBorder` | 普通卡片 |
| 2 | `border: 2px clrPrimary`（选中） | 主题选择卡 |
| 3 | `clrScrim` 全屏遮罩 | 模态弹窗 |

---

## 3. 布局模式

### 3.1 应用壳（Application Shell）

```
┌──────────────────────────────────────────────────────────┐
│ HeaderBar                                    56px        │
├──────────┬───────────────────────────────────┬───────────┤
│ Left     │         3D Viewport               │ ColorBar  │
│ Panel    │         (VTK)                     │ Panel     │
│ 300px    │                                   │ 184px     │
├──────────┴───────────────────────────────────┴───────────┤
│ StatusBar                                    32px        │
└──────────────────────────────────────────────────────────┘
         ▲ AppOverlay（全屏透明层，弹窗居中） ▲
```

- 工作区内边距：`12px`，面板间距：`12px`。
- 侧栏可折叠：展开宽度见组件 `expandedWidth`，折叠 `44px`。

### 3.2 右侧色标面板（ColorBar Panel）

结构自上而下：

1. **分区标签** `FlatSectionLabel` —「电场强度」
2. **FlatCard** 包裹：
   - 单位 `V/m` 居中（Caption）
   - 色条（36px 宽，圆角 6，clip）+ 数值列（max / 高 / 低 / min）
3. **FlatDivider**
4. **分区标签** —「色标范围」
5. 两行 **RowLayout**：标签 28px + `FlatTextField` + 单位 24px

色条高度：`max(220, 视口高度 × 0.42)`，保证大屏不显得过短。

### 3.3 模态弹窗 — 全屏居中（QQuickWidget 方案）

**禁止**将 `SettingsDialog` / `MessageDialog` 放在侧栏子树内居中。

正确架构：

```
ApplicationShell
├── QQuickWidget (panels…)
└── QQuickWidget (AppOverlay.qml)  ← 几何 = 整窗 rect()
    ├── SettingsDialog  anchors.centerIn: parent
    └── MessageDialog   anchors.centerIn: parent
```

- 无弹窗时：`WA_TransparentForMouseEvents`，不阻挡 3D 视口交互。
- 有弹窗时：关闭鼠标穿透，Popup.modal 自带遮罩。
- 打开方式：`App.openSettings()` → `settingsRequested` → Overlay 内 `open()`。

#### 设置弹窗规格

| 属性 | 值 |
|------|-----|
| 宽度 | 520px |
| 标题栏高度 | 56px |
| 内容边距 | 24px |
| 主题卡片 | 2×2 网格，高 96px，圆角 10 |
| 选中态 | 2px 主色边框 + `primarySoft` 底 |
| 底栏 | 取消 92px + 完成 104px，右对齐 |
| 取消行为 | 恢复 `originalTheme` |

#### 消息弹窗规格

| 属性 | 值 |
|------|-----|
| 宽度 | 400px |
| 边距 | 24px |
| 图标容器 | 44×44 圆形 |
| 标题 | 18 bold |
| 正文 | 14，相对图标缩进 56px |
| 按钮 | 「知道了」104px 宽 |

---

## 4. 组件库

| 组件 | 文件 | 用途 |
|------|------|------|
| `FlatButton` | `common/FlatButton.qml` | 全部按钮 |
| `FlatTextField` | `common/FlatTextField.qml` | 单行输入 |
| `FlatSliderRow` | `common/FlatSliderRow.qml` | 标签+滑条+数值 |
| `FlatToggle` | `common/FlatToggle.qml` | 布尔开关 |
| `FlatCard` | `common/FlatCard.qml` | 卡片容器 |
| `FlatDivider` | `common/FlatDivider.qml` | 分割线 |
| `FlatIconButton` | `common/FlatIconButton.qml` | 图标/关闭钮 |
| `FlatSectionLabel` | `common/FlatSectionLabel.qml` | 分区小标题 |
| `FlatModalScrim` | `common/FlatModalScrim.qml` | 可动画遮罩 |
| `CollapsiblePanel` | `components/CollapsiblePanel.qml` | 可折叠侧栏 |
| `SettingsDialog` | `components/SettingsDialog.qml` | 主题设置 |
| `MessageDialog` | `components/MessageDialog.qml` | 错误/提示 |

### 4.1 按钮 `tone`

| tone | 填充 | 边框 | 文字 |
|------|------|------|------|
| primary | `clrPrimary` → hover `clrPrimaryHover` | 无 | `#FFFFFF` |
| secondary | `#FFFFFF` → hover `#F8FAFC` | `clrBorder` | `clrText` |
| danger | `#FFFFFF` → hover `#FFF1F2` | `#FCA5A5` | `#EF4444` |

### 4.2 输入框状态

| 状态 | 背景 | 边框 |
|------|------|------|
| default | `clrFieldBg` | `clrFieldBorder` |
| focus | `clrFieldBg` | `clrFocusRing` |
| error | `#FEF2F2` | `#EF4444` |
| disabled | `#F1F5F9` | `clrBorder`，文字 `clrMuted` |

---

## 5. 动效（Motion）

| 场景 | 时长 | 曲线 |
|------|------|------|
| 颜色/边框过渡 | 120ms | 默认 |
| 侧栏折叠宽度 | 180ms | `Easing.OutCubic` |
| 遮罩显隐 | 180ms | `Easing.OutCubic` |
| 按钮按下 scale | 140ms | 0.96–1.0 |

原则：**不过度动画**；医疗场景避免闪烁与大面积 parallax。

---

## 6. 主题与暗色模式

四套主题：`晨曦白` / `深空夜` / `海洋蓝` / `薄荷绿`。

- 切换通过 `App.themeIndex`，实时刷新 `themePalette` 与 VTK 视口背景。
- 暗色主题：`clrBg` 深、`clrCard` 略浅一级，**禁止**纯 `#000000` 大面积。
- 对比度：正文与背景对比度 ≥ **4.5:1**（WCAG AA）。

---

## 7. 无障碍（A11y）

- 所有可点击控件最小命中区域 **38×38**（或等效 padding）。
- 表单标签必须可见，禁止仅用 placeholder 代替 label。
- 弹窗：`focus: true`，支持 Esc 关闭。
- 色觉辅助：热力图色标旁须带 **数值** 与 **高/低** 文字，不仅依赖颜色。

---

## 8. 代码组织

```
qml/
├── common/           # Flat* 设计系统组件
├── components/       # 业务面板与弹窗
└── overlays/         # AppOverlay 全屏模态层
src/
├── ApplicationShell  # QWidget 壳 + QQuickWidget 布局
└── AppController     # QML 桥接、主题、仿真状态
```

- 新组件：`FlatXxx.qml`，注册到 `resources/app.qrc`。
- 第三次重复样式 → 提取到 `common/`。
- Qt 5.15 禁止覆盖 `palette`、`checked`（Controls）、`cursorShape`。

---

## 9. 禁止事项

1. 业务 QML 内原生 `Button`/`TextField` 手写样式。
2. 侧栏内挂载需全屏居中的模态弹窗。
3. QML 直接调用 VTK API（走 `App`/`AppController`）。
4. VTK 视口上叠加 QML 交互层。
5. `QtGraphicalEffects` 用于 QQuickWidget 内日常 UI（OpenGL 冲突）。
6. 未走 design token 的硬编码色值。

---

## 10. 设计走查清单（Design QA）

**视觉**
- [ ] 色值来自 `themeColors` / 第 2 节 token？
- [ ] 控件高度 38 / 圆角 8|12 符合规范？
- [ ] 主/次/危险按钮 tone 正确？
- [ ] 间距为 4px 网格倍数？

**布局**
- [ ] 模态弹窗在 `AppOverlay` 全屏居中？
- [ ] 色标面板分区清晰（强度 / 范围）？
- [ ] 折叠侧栏宽度与 `ApplicationShell` 一致？

**交互**
- [ ] hover/focus/disabled 态完整？
- [ ] 设置弹窗取消可恢复主题？
- [ ] 无弹窗时 3D 视口可正常操作？

**工程**
- [ ] 新 QML 已加入 `app.qrc`？
- [ ] 单元测试 / 冒烟清单已更新？

---

## 11. 参考实现

| 场景 | 文件 |
|------|------|
| 应用壳 | `src/ApplicationShell.cpp` |
| 全屏模态层 | `qml/overlays/AppOverlay.qml` |
| 色标面板 | `qml/components/ColorBarPanel.qml` |
| 设置弹窗 | `qml/components/SettingsDialog.qml` |
| 左侧控制 | `qml/components/LeftControlPanel.qml` |
| 主题数据 | `src/AppController.cpp` → `themePalette()` |

---

*文档版本：3.0 · Mag-aim Design System · 与 EField Viewer 及后续 Qt 桌面产品同步维护。*
