import os
from setuptools import Extension, setup

setup(ext_modules=[Extension("GravitySim",
                             [f"GravitySim/{file}" for file in os.listdir("GravitySim") if file.endswith(".c")])
                   ])
