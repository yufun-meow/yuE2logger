- [中文版](README_CN.md)
- English
# !Attention!
!!This is a simulation calculation tool, and its output results absolutely cannot replace professional medical testing and doctor's guidance!!
>
The main branch only provides core computation code and does not include UI, etc. You can create your own UI
> 
This project is primarily for practice purposes, hence the functionality is relatively basic. You can use this as a foundation to add or modify features on your own
>
Regarding the pharmacokinetic model, please refer to [Model Description](MODEL.md)
# Yu's E2 Pharmacokinetic Simulation Engine/yuE2logger
## Introduction
A pharmacokinetic simulation library for estradiol (E2) and its esters (EB, EV, EC, EN), implemented in C++, supports the calculation of blood drug concentration for multiple administration routes
- Supported administration routes: oral (koufu), sublingual (shexia), gel application (ningjiao), patch (tiepian), injection (zhushe)
- Supported esters: E2, EB, EV, EC, EN
- Output time-concentration curve
- Calculate AUC (Area Under the Curve)
- Support time series simulation of multiple drug administration events
## Compilation
Place `pksim.cpp` and `pksim.h` in your project and compile them together with your main program:
```bash
g++ -std=c++11 main.cpp pksim.cpp -o sim
```
## File Description
- `pksim.h` header file, containing data structure definitions and function declarations
- `pksim.cpp` core implementation code
## Data structure
| Structure | Description |
|--------|---------------------|
| `DoseEvent` | Single dose event |
| `PK` | Pharmacokinetic parameters |
| `GridPoint` | Time-Concentration Point |
| `SimulationResult` | Simulation result (curve + AUC) |
---
## How to Use
Copy `pksim.cpp` and `pksim.h` into your project, include the header file, and you can use it.
## Parameter Description
### Drug administration route parameters
| Parameter | Description |
|--------|---------------------|
| `koufu` | Oral administration |
| `shexia` | sublingual administration |
| `ningjiao` | gel application |
| `tiepian`	| patch |
| `zhushe` | injection |
---
### Ester parameters
| Parameter | Description |
|--------|---------------------|
| `E2` | Estradiol |
| `EB` | estradiol benzoate |
| `EV` | Estradiol valerate |
| `EC`	| Estradiol Cyclopentylpropionate |
| `EN` | Estradiol valerate |
---
### Gel part parameters (only for ningjiao)

| Parameter | Description |
|------|------|
| `dd` | scrotum |
| Other values | Other parts (inner thigh/inner arm, etc.) |
---
#### !Special note!
The absorption rate of the scrotum here is ten times higher than that of other parts, which is inferred from the absorption rate data of testosterone gel and may not be accurate!
## Function Description

### 1.  `realE2`

Calculate the molecular weight coefficient of esters converted to estradiol (E2)

```cpp
double realE2(string ester);
```

| Parameter | Type | Description |
|------|------|------|
| `ester` | string | Ester names: E2 / EB / EV / EC / EN |

**Return value:** Conversion factor (double)

---

### 2.  `utility`

Calculate bioavailability

```cpp
double utility(string way, string ester, double xishou = 0, string buwei = "");
```

| Parameter | Type | Description |
|------|------|------|
| `way` | string | Route of administration |
| `ester` | string | Ester name |
| `xishou` | double | Sublingual absorption ratio (only for shexia) |
| `buwei` | string | Gel site: dd (scrotum) or other |

**Return value:** Bioavailability (double)

---

### 3.  `calculatePK`

Calculate pharmacokinetic parameters (F, k1_f, k1_s, k2, k3, frac_f, r)

```cpp
PK calculatePK(string way, string ester = "", double xishou = 0, double r = 0, string buwei = "");
```

| Parameter | Type | Description |
|------|------|------|
| `way` | string | route of administration |
| `ester` | string | Ester name |
| `xishou` | double | Sublingual absorption ratio (only for shexia) |
| `r` | double | Patch release rate, in mg/day (only used for tiepian) |
| `buwei` | string | Gel site: dd (scrotum) or other |

**Return value:** `PK` struct
#### Special Note on the r Parameter of Patch Administration
The r parameter supports two usage modes, with the caller determining the meaning of the passed-in value based on the scenario:

> Mode 1 (Rate Mode): Set r as the release rate of the patch (unit: mg/day), and set the dose field in DoseEvent to the same value as r, while setting wearDuration to the duration of wear. In this mode, the simulation calculates based on constant release rate
>
> Mode 2 (Total Dose Mode): Set r to 0, set the "dose" field in DoseEvent to the total patch dose (unit: mg), and set "wearDuration" to the duration of wear. In this mode, the simulation treats the total dose as a one-time release and eliminates it using a one-compartment model

The difference between the two modes lies in the shape of the drug release curve: the rate mode simulates continuous and constant blood entry, while the total dose mode simulates the instantaneous release of the entire dose from the patch

### 4.  `oneCompAmount`

Calculation of blood drug concentration for a single dose in a one-compartment model

```cpp
double oneCompAmount(double tau, double doseMG, double F, double ka, double ke);
```

| Parameter | Type | Description |
|------|------|------|
| `tau` | double | Time elapsed after administration (hours) |
| `doseMG` | double | Dose (mg) |
| `F` | double | Bioavailability |
| `ka` | double | Absorption rate constant |
| `ke` | double | elimination rate constant |

**Return value:** Blood drug concentration (double)

---

### 5.  `runSimulation`

**Main entry function.** Run the complete simulation and return the time-concentration curve and AUC

```cpp
SimulationResult runSimulation(vector<DoseEvent>& events, double weight);
```

| Parameter | Type | Description |
|------|------|------|
| `events` | vector<DoseEvent>& | List of dosing events (automatically sorted by time) |
| `weight` | double | Weight (kg) |

**Return value:** `SimulationResult` struct (including curve + AUC)

---

### 6.  `interpolateConcentration`

Interpolate from the simulation results to obtain the blood drug concentration at any given time point

```cpp
double interpolateConcentration(const SimulationResult& sim, double hour);
```

| Parameter | Type | Description |
|------|------|------|
| `sim` | const SimulationResult& | The result returned by `runSimulation` |
| `hour` | double | Target time point (hour) |

**Return value:** The plasma drug concentration at this time point (double)

## License
GPL v3

Please refer to [LICENSE](LICENSE)
