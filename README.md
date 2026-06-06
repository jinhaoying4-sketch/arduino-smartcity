# arduino-smartcity
# 🌆 Smart City AI Weather Forecast System

> Arduino 기반 스마트 시티 환경 모니터링 및 AI형 날씨 예측 시스템
> Kakao AI Rookie Camp Portfolio Project

## 📌 Project Overview

본 프로젝트는 **Arduino 기반 스마트 시티(Smart City) 시스템**으로, 다양한 센서를 활용하여 도시 환경 정보를 수집하고 이를 분석하여 사용자에게 직관적인 정보를 제공하는 것을 목표로 합니다.

특히 단순 센서 측정을 넘어, 온도와 습도 데이터를 기반으로 한 **경량 LLM(Simple LLM Formula) 방식의 날씨 예측 로직**을 구현하여 인공지능 형태의 날씨 예보 기능을 제공합니다.

### 주요 목표

* 실시간 환경 데이터 수집
* 스마트 교통 신호 시뮬레이션
* 차량 접근 감지 및 경고 시스템
* AI 기반 날씨 상태 분석
* LCD 기반 사용자 인터페이스 제공
* 임베디드 시스템 안정성 확보

---

## 🎯 Problem Statement

도시 환경에서는 다양한 센서 데이터가 생성되지만, 사용자는 단순 수치보다 의미 있는 정보와 예측 결과를 원합니다.

본 프로젝트는 다음 문제를 해결하고자 하였습니다.

* 센서 데이터를 실시간으로 수집한다.
* 데이터를 사람이 이해하기 쉬운 형태로 가공한다.
* 단순 측정을 넘어 미래 상태를 예측한다.
* 제한된 Arduino 환경에서도 AI 개념을 적용한다.

---

## 🏗️ System Architecture

```text
[DHT11]
   │
   ├─ Temperature
   └─ Humidity
        │
        ▼
 AI Weather Logic
(Simple LLM Formula)
        │
        ▼
 LCD Display

[HC-SR04]
        │
        ▼
 Distance Detection
        │
        ▼
 Buzzer Warning

Traffic Light Controller
        │
        ▼
 Smart City Simulation
```

---

## 🔧 Hardware Components

| Component                 | Purpose                       |
| ------------------------- | ----------------------------- |
| Arduino UNO               | Main Controller               |
| DHT11                     | Temperature & Humidity Sensor |
| HC-SR04                   | Distance Detection            |
| LCD1602 I2C               | Information Display           |
| Passive Buzzer            | Warning Alarm                 |
| LEDs                      | Traffic Light Simulation      |
| Breadboard & Jumper Wires | Circuit Construction          |

---

## 💡 Key Features

### 1. Real-Time Environmental Monitoring

DHT11 센서를 활용하여 온도와 습도를 실시간 측정합니다.

표시 정보:

* Temperature (°C)
* Humidity (%)

---

### 2. AI-Style Weather Forecast

본 프로젝트의 핵심 기능입니다.

단순히 현재 데이터를 출력하는 것이 아니라 온도와 습도 조건을 분석하여 날씨 상태를 예측합니다.

### Forecast Logic

#### Snow Prediction

```text
Humidity ≥ 80%
AND
Temperature ≤ -2°C
```

#### Rain Prediction

```text
Humidity ≥ 80%
AND
Temperature ≤ 30°C
```

#### Hot Weather Warning

```text
Temperature > 26°C
```

#### Cold Weather Warning

```text
Temperature < 20°C
```

---

### Why "AI"?

실제 대규모 언어 모델(LLM)을 실행하지는 않지만,

본 프로젝트는 다음과 같은 AI 사고 과정을 모방합니다.

```text
Input Data
   ↓
Rule-based Reasoning
   ↓
Probability Estimation
   ↓
Natural Language Feedback
```

예를 들어,

```text
Humidity = 92%
Temperature = 23°C
```

인 경우

```text
Chance of rain. See status bar.
```

와 같은 자연어 형태의 예측 결과를 생성합니다.

이를 통해 제한된 임베디드 환경에서도 AI 개념을 적용할 수 있음을 보여줍니다.

---

### 3. Smart Parking Assistant

HC-SR04 초음파 센서를 이용하여 차량 후방 거리를 측정합니다.

기능:

* 실시간 거리 측정
* 위험 거리 감지
* 경고음 발생

```text
Distance ≤ 5cm
→ Buzzer ON
```

---

### 4. Smart Traffic Light Simulation

스마트 시티의 핵심 요소인 교통 신호 체계를 구현했습니다.

지원 기능:

* 차량 신호
* 보행자 신호
* 자동 주기 제어

---

### 5. Intelligent LCD Dashboard

LCD1602를 활용하여 다음 정보를 표시합니다.

* 온도
* 습도
* 날씨 예측
* 시스템 상태
* 오류 정보
* 스크롤 공지 메시지

---

### 6. Fault Detection & Recovery

시스템 안정성을 위해 오류 감지 기능을 구현했습니다.

#### DHT Sensor Error Detection

* 데이터 읽기 실패 감지
* 자동 경고 출력

#### HC-SR04 Error Detection

* 거리 측정 실패 감지
* 오류 상태 표시

#### Watchdog Recovery

* 시스템 정지 감지
* 자동 재부팅 수행

---

## 🧠 AI Design Concept

본 프로젝트는 "Tiny AI for Embedded Systems"라는 아이디어를 기반으로 설계되었습니다.

### AI Pipeline

```text
Sensor Data
     ↓
Feature Extraction
     ↓
Rule Evaluation
     ↓
Probability Calculation
     ↓
Weather Forecast
     ↓
Natural Language Response
```

제한된 메모리와 연산 성능을 가진 Arduino에서도 AI적 사고 과정을 구현할 수 있음을 보여줍니다.

---

## 📊 Example Outputs

### Rain Prediction

```text
Temperature : 25°C
Humidity    : 91%

Forecast:
Chance of rain.
Probability: 55%
```

### Heat Warning

```text
Temperature : 31°C

Forecast:
High temperature.
Beware of heatstroke.
```

### Comfortable Weather

```text
Temperature : 23°C
Humidity    : 55%

Forecast:
The weather is pleasant.
Welcome to the Smart City.
```

---

## 🚀 Future Improvements

### Short-Term

* ESP32 Wi-Fi Integration
* Weather API Connection
* Mobile Dashboard

### Mid-Term

* TinyML Integration
* Historical Data Storage
* Adaptive Forecast Model

### Long-Term

* LLM API Integration
* Smart City Digital Twin
* Multi-Sensor AI Analytics Platform

---

## 🛠️ Development Environment

```text
Language  : C++
Platform  : Arduino
IDE       : Arduino IDE
Libraries :
 - DHT
 - LiquidCrystal_I2C
 - Wire
 - AVR Watchdog
```

---

## 📈 Learning Outcomes

본 프로젝트를 통해 다음 역량을 향상시켰습니다.

* Embedded Programming
* Sensor Integration
* Hardware-Software Interaction
* Real-Time System Design
* Fault-Tolerant Architecture
* AI Thinking Process Design
* Smart City System Modeling

---

## 👨‍💻 Author

**Kim**

Kakao AI Rookie Camp Portfolio Project

2026
