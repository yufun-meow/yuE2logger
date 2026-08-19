# E2 Pharmacokinetic Simulation Engine

> **Note:** The `main` branch provides only the core calculation code, with no UI included. You are free to build your own UI.
>
> This project is for practice purposes, so the functionality is basic. Feel free to extend or modify it as needed.

---

## Introduction

A C++ library for simulating the pharmacokinetics of estradiol (E2) and its esters (EB, EV, EC, EN), supporting multiple routes of administration.

### Features

- Supported routes: oral (`koufu`), sublingual (`shexia`), gel (`ningjiao`), patch (`tiepian`), injection (`zhushe`)
- Supported esters: E2, EB, EV, EC, EN
- Outputs concentration-time curve
- Calculates AUC (Area Under Curve)
- Supports multi-dose time series simulation

---

## Compile

Place `pksim.cpp` and `pksim.h` in your project and compile them together with your main program:
```bash
g++ -std=c++11 main.cpp pksim.cpp -o sim
```
## Files

| File | Description |
|------|-------------|
| `pksim.h` | Header file containing data structure definitions and function declarations |
| `pksim.cpp` | Core implementation code |

---

## Data Structures

| Struct | Description |
|--------|-------------|
| `DoseEvent` | Single dose event |
| `PK` | Pharmacokinetic parameters |
| `GridPoint` | Time-concentration point |
| `SimulationResult` | Simulation result (curve + AUC) |

---

## Usage

Copy `pksim.cpp` and `pksim.h` into your project and include the header file.
## Parameter Reference

### Route Parameters

| Parameter | Description |
|-----------|-------------|
| `koufu` | Oral |
| `shexia` | Sublingual |
| `ningjiao` | Gel |
| `tiepian` | Patch |
| `zhushe` | Injection |

---

### Ester Parameters

| Parameter | Description |
|-----------|-------------|
| `E2` | Estradiol |
| `EB` | Estradiol Benzoate |
| `EV` | Estradiol Valerate |
| `EC` | Estradiol Cypionate |
| `EN` | Estradiol Enanthate |

---

## License

GPL v3

See [LICENSE](LICENSE) for details.
