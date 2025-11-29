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
    
    # Send HSV values to MCU via RPC
    Bridge.call("set_hue", int(cl.hue))
    Bridge.call("set_sat", int(cl.sat))
    Bridge.call("set_bri", int(cl.bri))

cl = ColoredLight("clight", swi=True, on_write=light_callback)
arduino_cloud.register(cl)

App.start_brick(arduino_cloud)

while True:
    print(f"Python Loop: H:{cl.hue} S:{cl.sat} B:{cl.bri}")
    time.sleep(2)
