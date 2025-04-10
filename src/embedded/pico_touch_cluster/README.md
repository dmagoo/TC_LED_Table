# Pico Touch Cluster Firmware

This is the firmware for the Raspberry Pi Pico that monitors up to 10 TTP223 touch sensors,
persists config in flash, and sends JSON-formatted messages over USB serial.

---

## ✅ Project Structure

```
pico_touch_cluster/
├── src/
│   └── main.cpp       ← Your firmware code
└── README.md          ← This file
```

---

## 🔧 Build & Upload (PlatformIO)
** NOTE that this is out of date, potentially **
1. Open VS Code
2. Navigate to `embedded/pico_touch_cluster`
3. Build:
   - Open PlatformIO sidebar
   - Click **Build** (checkmark icon)
4. Upload:
   - Hold the **BOOTSEL** button on the Pico
   - Plug it into USB
   - Drag the generated `.uf2` file from `.pio/build/pico/` onto the mounted USB drive

---

## 💬 Serial Communication

After upload, the Pico will communicate over USB serial (CDC):

- Incoming: JSON config from host
- Outgoing:
  - `touch_event` on sensor changes
  - `periodic_touch_status` every ~60 seconds

### Example config to send:
```json
{
  "clusterId": 2,
  "nodes": [
    { "nodeId": 1, "sensorGPIO": 4, "ledCount": 8 },
    { "nodeId": 2, "sensorGPIO": 5, "ledCount": 8 }
  ]
}
```

Send it over serial followed by a newline.

---

## 🛠 Requirements

- Pico board with TTP223 sensors on GPIOs
- PlatformIO installed in VS Code
- ArduinoJson library (declared in `platformio.ini`)
