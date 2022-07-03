#!/usr/bin/python3

# -*- coding: utf-8 -*-

import sys
import os

sys.path.insert(0, os.path.join('tools', 'gyp', 'pylib'))

from gyp.common import GetFlavor
flavor_params = {}
flavor = GetFlavor(flavor_params) # 获取到操作系统类型

# --generator-output 和 -G 指定了输出目录的位置。Node.js 中的 makefile 文件是自己写的，而不是 gyp 生成的。
import gyp
d = os.path.dirname(os.path.realpath(__file__))
gyp.main(['-f', 'make-' + flavor, os.path.join(d, 'build.gyp'), 
    '-I', os.path.join(d, 'default.gypi'),
    '-I', os.path.join(d,'var.gypi'),
    # '-Dcomponent=static_library', 
    # '-Dlibrary=static_library', 
    # '-Dlinux_use_bundled_binutils=0', '-Dlinux_use_bundled_gold=0', '-Dlinux_use_gold_flags=0',
    '--depth=.', '--no-parallel', '--generator-output', os.path.join(d, 'out'),'-Goutput_dir=' + os.path.join(d, 'out')])
