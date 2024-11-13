from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        'game_module',
        ['bindings.cpp', 'Game.cpp', 'AI.cpp'],
        include_dirs=[pybind11.get_include()],
        language='c++',
        extra_compile_args=['-Ofast', '-g', '-Wall', '-std=c++20'],
        extra_link_args=[],
    ),
]

setup(
    name='game_module',
    ext_modules=ext_modules,
)