# SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
# SPDX-License-Identifier: MPL-2.0

from arduino.app_bricks.arduino_cloud import ArduinoCloud, ColoredLight
from arduino.app_utils import App, Bridge
from typing import Any
import time

arduino_cloud = ArduinoCloud()

def light_callback(client: object, value: Any):
    print(f"Cloud was updated: {value}")
    print(f"H:{cl.hue} S:{cl.sat} B:{cl.bri}")

cl = ColoredLight("clight", swi=True, on_write=light_callback)
arduino_cloud.register(cl)

App.start_brick(arduino_cloud)

# Bridge Protocol Test: int (0=OFF, 1=R, 2=G, 3=B)
print("\n" + "="*50)
print("BRIDGE TEST: set_color_int (int)")
print("="*50)

colors = [
    (1, "RED"),
    (2, "GREEN"),
    (3, "BLUE"),
    (0, "OFF")
]

for val, name in colors:
    print(f"\n[Test] Sending {name} ({val})")
    Bridge.call("set_color_int", val)
    print(f"  Expected: LED3 fade {name}, LED4 blink {name}")
    time.sleep(5)

print("\n" + "="*50)
print("Test complete. Entering normal loop...")
print("="*50 + "\n")

while True:
    print(f"Python Loop: H:{cl.hue} S:{cl.sat} B:{cl.bri}")
    time.sleep(10)
