# SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
# SPDX-License-Identifier: MPL-2.0

from arduino.app_bricks.arduino_cloud import ArduinoCloud, ColoredLight
from arduino.app_utils import App, Bridge
from typing import Any
import time

arduino_cloud = ArduinoCloud()

def light_callback(client: object, value: Any):
    print(f"Cloud updated: H:{cl.hue} S:{cl.sat} B:{cl.bri} SW:{cl.swi}")
    Bridge.call("set_hue", int(cl.hue))
    Bridge.call("set_brightness", int(cl.bri))
    Bridge.call("set_switch", cl.swi)

cl = ColoredLight("clight", swi=True, on_write=light_callback)
arduino_cloud.register(cl)

App.start_brick(arduino_cloud)

print("\n" + "="*50)
print("Arduino Cloud RGB LED Control")
print("LED3: Cloud-controlled (HUE/BRI/SWITCH)")
print("LED4: Heartbeat (R->G->B->OFF)")
print("="*50 + "\n")

while True:
    time.sleep(10)
