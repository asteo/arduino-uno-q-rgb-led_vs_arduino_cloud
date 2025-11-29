# SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
# SPDX-License-Identifier: MPL-2.0

from arduino.app_bricks.arduino_cloud import ArduinoCloud, ColoredLight
from arduino.app_utils import App, Bridge
from typing import Any
import time

arduino_cloud = ArduinoCloud()
cl = ColoredLight("CClight", swi=True)

def light_callback(client: object, value: Any):
    print(f"Cloud updated: {value}")

    # Send HSV values to MCU via RPC
    Bridge.call("set_hue", cl.hue)
    Bridge.call("set_sat", cl.sat)
    Bridge.call("set_bri", cl.bri)

arduino_cloud.register(cl)
cl.on_write = light_callback

App.start_brick(arduino_cloud)

while True:
    print(f"H:{cl.hue} S:{cl.sat} B:{cl.bri}")
    time.sleep(2)
