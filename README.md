# Arduino Uno Q - RGB LED Cloud Control

Full-featured RGB LED control via Arduino Cloud using the Arduino Uno Q's dual-processor architecture.

## Features

- ✅ **Cloud Control:** HUE (0-360), Brightness (0-100), ON/OFF switch
- ✅ **HSV to RGB Conversion:** Smooth color transitions
- ✅ **Dual LED Control:**
  - LED3: Cloud-controlled RGB (PWM)
  - LED4: Heartbeat indicator (R→G→B→OFF)
- ✅ **Bridge Protocol:** Python (Linux) ↔ C++ (STM32U5)
- ✅ **Zephyr RTOS:** Multi-threaded, hardware PWM

## Hardware

**Arduino Uno Q** - Dual processor board:
- **Qualcomm QRB2210** (Linux/Debian) - Runs Python app, Arduino Cloud connection
- **STM32U585ZI** (Zephyr RTOS) - Controls LEDs via PWM

**LEDs:**
- LED3: RGB LED with hardware PWM (TIM5, 500Hz)
- LED4: RGB LED, digital control

## Architecture

```
Arduino Cloud (Web/Mobile)
         ↓
    Python App (Linux)
         ↓
   Bridge Protocol (MessagePack)
         ↓
  STM32U5 Sketch (Zephyr RTOS)
         ↓
    LED3 (PWM) + LED4 (Digital)
```

## Quick Start

### Prerequisites

- Arduino Uno Q device
- Arduino Cloud account
- ADB access to device

### Deploy

```bash
# SSH into device
adb shell
su - arduino
cd ~/ArduinoApps/arduino-cloud

# Pull latest code
git pull

# Compile and upload sketch
cd sketch
arduino-cli compile -b arduino:zephyr:unoq . --clean --upload

# Restart Python app
docker restart arduino-cloud-main-1

# Watch logs
docker logs arduino-cloud-main-1 -f
```

### Monitor Output

```bash
# View STM32U5 debug output
cat /dev/ttyACM0
# or
minicom /dev/ttyACM0
```

## Project Structure

```
.
├── sketch/
│   ├── sketch.ino          # STM32U5 code (Zephyr RTOS)
│   └── sketch.yaml         # Sketch metadata
├── python/
│   └── main.py             # Linux side (Arduino Cloud → Bridge)
├── app.yaml                # App configuration
└── README.md               # This file
```

## Code Overview

### STM32U5 (sketch.ino)

**Bridge Handlers:**
```cpp
void set_hue(int hue);           // 0-360
void set_brightness(int bri);    // 0-100
void set_switch(bool swi);       // true/false
```

**LED3 Thread:**
- Converts HSV to RGB
- Updates PWM duty cycles (50ms refresh)
- Handles ON/OFF state

**LED4 Timer:**
- 500ms heartbeat cycle
- R→G→B→OFF pattern

### Python (main.py)

**Cloud Callback:**
```python
def light_callback(client, value):
    Bridge.call("set_hue", int(cl.hue))
    Bridge.call("set_brightness", int(cl.bri))
    Bridge.call("set_switch", cl.swi)
```

## Technical Details

### PWM Configuration

LED3 uses hardware PWM via Zephyr native API:
```cpp
static const struct pwm_dt_spec pwm_led3_r = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 5);
static const struct pwm_dt_spec pwm_led3_g = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 6);
static const struct pwm_dt_spec pwm_led3_b = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 7);
```

**Why indices 5,6,7?**
- Device tree has 8 PWM entries
- First 5 are digital pins (D3, D5, D6, D9, D10)
- LED3 RGB comes after at indices 5, 6, 7

### HSV to RGB Conversion

Full HSV color space support:
- **H (Hue):** 0-360° (color wheel)
- **S (Saturation):** Fixed at 100% (full color)
- **V (Value/Brightness):** 0-100% (controlled via Cloud)

### Bridge Protocol

Uses MessagePack for efficient serialization:
- **int:** HUE, Brightness values
- **bool:** Switch state
- Bidirectional communication supported

## Development

### Workflow

1. **Edit on laptop:**
   ```bash
   cd ~/Documents/ArduinoLab/arduino-uno-q-rgb-led_vs_arduino_cloud
   # Edit files
   git add .
   git commit -m "Description"
   git push
   ```

2. **Deploy on device:**
   ```bash
   adb shell
   su - arduino
   cd ~/ArduinoApps/arduino-cloud
   git pull
   cd sketch && arduino-cli compile -b arduino:zephyr:unoq . --clean --upload
   docker restart arduino-cloud-main-1
   ```

### Debugging

**Monitor.print output:**
```cpp
Monitor.print("HUE: ");
Monitor.println(led3_hue);
```
Visible in `/dev/ttyACM0`

**Important:** Always use `--clean` flag when changing PWM configuration!

## Commit History

16 commits documenting the learning journey:
- PWM investigation and fixes
- Thread implementation
- Bridge protocol integration
- HSV color conversion
- Full Cloud integration

## Future Enhancements

- [ ] Saturation control (currently fixed at 100%)
- [ ] Creative effects (pulse, rainbow, breathe)
- [ ] Effect selection via Cloud variable
- [ ] Performance optimization

## License

MPL-2.0

## Author

**asteo** - Arduino Uno Q RGB LED exploration

---

**Arduino Cloud Dashboard:** Control your LED from anywhere!
