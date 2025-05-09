import os
import multiprocessing
import sysconfig
import pybind11
from setuptools import setup, Extension

USE_SANITIZER = os.environ.get('BUILD_SANITIZER', '0') == '1'

ext_flags = [
    '-O3', '-g', '-Wall', '-Werror', '-Wextra', '-std=c++11', '-pedantic',
    '-march=native',
    f'-flto={multiprocessing.cpu_count()}',
    '-funroll-loops',
]
if USE_SANITIZER:
    ext_flags += ['-fsanitize=address,leak,undefined']

link_flags = [
    '-g',
    f'-flto={multiprocessing.cpu_count()}',
]
if USE_SANITIZER:
    link_flags += ['-fsanitize=address,leak,undefined']

cflags = sysconfig.get_config_var('CFLAGS')
if cflags:
    sysconfig.get_config_vars()['CFLAGS'] = cflags.replace('-g', '').replace('-O2', '-O3')

ext_modules = [
    Extension(
        'game_module',
        ['bindings.cpp', 'Game.cpp', 'AI.cpp', 'Constants.cpp'],
        include_dirs=[pybind11.get_include()],
        language='c++',
        extra_compile_args=ext_flags,
        extra_link_args=link_flags,
    ),
]

setup(
    name='game_module',
    ext_modules=ext_modules,
    options={'build_ext': {'parallel': multiprocessing.cpu_count()}},
)
