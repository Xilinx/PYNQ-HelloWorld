# Copyright (C) 2022 Xilinx, Inc
#
# SPDX-License-Identifier: BSD-3-Clause

import numpy as np
from PIL import Image
import pynq
from pynq import allocate, DefaultIP, Overlay
import pytest

_type = 'xilinx.com:hls:resize_accel:1.0'


class ResizeIP(DefaultIP):
    bindto = [_type]


@pytest.fixture
def create_overlay():
    for i in range(5):
        try:
            ol = Overlay("../resizer.xclbin", device=pynq.Device.devices[0])
            cu = ol.ip_dict['resize_accel_1']
        except OSError as e:
            if "Overlay file" in str(e):
                pytest.exit(str(e))
            elif i != 4:
                continue
            raise OSError("Could not program the FPGA")

    yield ol, cu
    ol.free()


def test_overlay_type(create_overlay):
    ol, _ = create_overlay
    assert type(ol) == Overlay


def test_overlay_ip_dict(create_overlay):
    ol, _ = create_overlay
    assert ol.ip_dict


def test_type(create_overlay):
    _, cu = create_overlay
    assert cu['type'] == _type


def test_hw_control(create_overlay):
    _, cu = create_overlay
    assert cu['hw_control_protocol'] == 'ap_ctrl_chain'


def test_cu_name(create_overlay):
    _, cu = create_overlay
    assert cu['cu_name'] == 'resize_accel:resize_accel_1'


def test_cu_index(create_overlay):
    _, cu = create_overlay
    assert cu['cu_index'] == 0


def test_index(create_overlay):
    _, cu = create_overlay
    assert cu['index'] == 0


def test_memory_banks(create_overlay):
    """Test memory banks"""
    _, cu = create_overlay
    registers = cu['registers']
    assert registers['img_inp']['memory'] in ['bank0', 'DDR0', 'HBM0']
    assert registers['img_out']['memory'] in ['bank1', 'DDR1', 'HBM1']


def test_driver(create_overlay):
    """Test if driver have been assigned properly"""
    ol, _ = create_overlay
    assert type(ol.resize_accel_1) == ResizeIP


@pytest.mark.timeout(60)
def test_resizer(create_overlay):
    ol, _ = create_overlay
    resizer = ol.resize_accel_1
    if 'bank0' in ol.mem_dict:
        memp0 = ol.bank0
        memp1 = ol.bank1
    elif 'DDR0' in ol.mem_dict:
        memp0 = ol.DDR0
        memp1 = ol.DDR1
    elif 'HBM0' in ol.mem_dict:
        memp0 = ol.HBM0
        memp1 = ol.HBM1

    image = Image.open("../images/sahara.jpg")
    old_width, old_height = image.size
    resize_factor = 4
    new_width = old_width//resize_factor
    new_height = old_height//resize_factor

    in_buffer = allocate((old_height, old_width, 3), np.uint8, target=memp0)
    out_buffer = allocate((new_height, new_width, 3), np.uint8, target=memp1)

    in_buffer[:] = np.array(image)

    in_buffer.sync_to_device()
    resizer.start(in_buffer, out_buffer, old_height, old_width, new_height,
                  new_width)
    out_buffer.sync_from_device()
    assert np.any(out_buffer)

    del in_buffer
    del out_buffer
