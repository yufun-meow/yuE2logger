- 中文版
- [English](README.md)
# !注意!
> !!这是一个模拟计算工具，其输出结果绝对不能替代专业医疗检测和医生指导!!
>
> main 分支仅提供核心计算代码，不提供ui等，你可以自己制作ui
> 
> 本项目为练手性质，因此功能较为基础，您可以以此为基础自行添加功能/修改
>
> 关于药代动力学模型，请查看 [模型说明](MODEL_CN.md)
# 雨亦E2药代动力学模拟引擎/yuE2logger
## 简介
基于 C++ 实现的雌二醇（E2）及其酯类（EB、EV、EC、EN）药代动力学模拟库，支持多种给药途径的血药浓度计算
- 支持给药途径：口服（koufu）、舌下含服（shexia）、凝胶涂抹（ningjiao）、贴片（tiepian）、注射（zhushe）
- 支持酯类：E2、EB、EV、EC、EN
- 输出时间-浓度曲线
- 计算 AUC（药时曲线下面积）
- 支持多个给药事件的时间序列模拟
## 编译
将 `pksim.cpp` 和 `pksim.h` 放到你的项目中，与你的主程序一起编译：
```bash
g++ -std=c++11 main.cpp pksim.cpp -o sim
```
## 文件说明
- `pksim.h` 头文件，包含数据结构定义和函数声明
- `pksim.cpp` 核心实现代码
## 数据结构
| 结构体 | 说明 / Description |
|--------|---------------------|
| `DoseEvent` | 单次给药事件 |
| `PK` | 药代动力学参数 |
| `GridPoint` | 时间-浓度点 |
| `SimulationResult` | 模拟结果（曲线 + AUC）|
---
## 如何使用
将 `pksim.cpp` 和 `pksim.h` 复制到你的项目中，包含头文件即可调用。
## 参数说明
### 给药途径参数
| 参数 | 说明 |
|--------|---------------------|
| `koufu` |	口服 |
| `shexia` |	舌下含服 |
| `ningjiao` |	凝胶涂抹 |
| `tiepian`	| 贴片 |
| `zhushe` | 注射 |
---
### 酯类参数
| 参数 | 说明 |
|--------|---------------------|
| `E2` | 雌二醇 |
| `EB` |	苯甲酸雌二醇 |
| `EV` |	戊酸雌二醇 |
| `EC`	| 环戊丙酸雌二醇 |
| `EN` | 庚酸雌二醇 |
---
### 凝胶部位参数（仅 ningjiao 用）

| 参数 | 说明 |
|------|------|
| `dd` | 阴囊 |
| 其他值 | 其他部位（大腿内侧/大臂内侧等） |
---
#### !特别说明!
此处阴囊吸收率为其它部位的十倍，该数值由睾酮凝胶的吸收率数据推断而来，不一定准确！
## 函数说明

### 1. `realE2`

计算酯类折算为雌二醇（E2）的分子量系数

```cpp
double realE2(string ester);
```

| 参数 | 类型 | 说明 |
|------|------|------|
| `ester` | string | 酯类名称：E2 / EB / EV / EC / EN |

**返回值：** 折算系数（double）

---

### 2. `utility`

计算生物利用度

```cpp
double utility(string way, string ester, double xishou = 0, string buwei = "");
```

| 参数 | 类型 | 说明 |
|------|------|------|
| `way` | string | 给药途径 |
| `ester` | string | 酯类名称 |
| `xishou` | double | 舌下吸收比例（仅 shexia 用） |
| `buwei` | string | 凝胶部位：dd（阴囊）或其他 |

**返回值：** 生物利用度（double）

---

### 3. `calculatePK`

计算药代动力学参数（F、k1_f、k1_s、k2、k3、frac_f、r）

```cpp
PK calculatePK(string way, string ester = "", double xishou = 0, double r = 0, string buwei = "");
```

| 参数 | 类型 | 说明 |
|------|------|------|
| `way` | string | 给药途径 |
| `ester` | string | 酯类名称 |
| `xishou` | double | 舌下吸收比例（仅 shexia 用） |
| `r` | double | 贴片释放速率，单位 mg/天（仅 tiepian 用） |
| `buwei` | string | 凝胶部位：dd（阴囊）或其他 |

**返回值：** `PK` 结构体

---
#### 关于贴片 r 参数的特别说明
r 参数支持两种使用模式，由调用者根据场景决定传入值的含义：

> 模式一（速率模式）：将 r 设为贴片的释放速率（单位：mg/天），同时将 DoseEvent 中的 dose 字段设为释放速率（数值与 r 相同），wearDuration 设为佩戴时长。此模式下，模拟会按恒速释放计算
>
> 模式二（总量模式）：将 r 设为 0，将 DoseEvent 中的 dose 字段设为贴片总剂量（单位：mg），wearDuration 设为佩戴时长。此模式下，模拟会将总剂量视为一次性释放，再按一房室模型消除
>
两种模式的区别在于药物释放曲线的形态：速率模式模拟持续恒速入血，总量模式模拟贴片瞬间释放全部剂量
### 4. `oneCompAmount`

一室模型单次给药的血药浓度计算

```cpp
double oneCompAmount(double tau, double doseMG, double F, double ka, double ke);
```

| 参数 | 类型 | 说明 |
|------|------|------|
| `tau` | double | 给药后经过的时间（小时） |
| `doseMG` | double | 剂量（mg） |
| `F` | double | 生物利用度 |
| `ka` | double | 吸收速率常数 |
| `ke` | double | 消除速率常数 |

**返回值：** 血药浓度（double）

---

### 5. `runSimulation`

**主入口函数。** 运行完整模拟，返回时间-浓度曲线和 AUC

```cpp
SimulationResult runSimulation(vector<DoseEvent>& events, double weight);
```

| 参数 | 类型 | 说明 |
|------|------|------|
| `events` | vector<DoseEvent>& | 给药事件列表（自动按时间排序） |
| `weight` | double | 体重（kg） |

**返回值：** `SimulationResult` 结构体（包含曲线 + AUC）

---

### 6. `interpolateConcentration`

从模拟结果中插值获取任意时间点的血药浓度

```cpp
double interpolateConcentration(const SimulationResult& sim, double hour);
```

| 参数 | 类型 | 说明 |
|------|------|------|
| `sim` | const SimulationResult& | `runSimulation` 返回的结果 |
| `hour` | double | 目标时间点（小时） |

**返回值：** 该时间点的血药浓度（double）

## 许可证
GPL v3

请查看 [LICENSE](LICENSE)
