# encoding: utf-8

from distutils.core import setup, Extension

main_module = Extension(
      'polygons',
      sources=['main.cpp'],
      extra_compile_args=['-std=c++11']
)

setup(name='polygons',
      version='0.1.0',
      description='Module for performing geometries math.',
      ext_modules=[main_module],
)
