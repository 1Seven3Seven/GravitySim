import os
from setuptools import Extension, setup


def get_source_files() -> list[str]:
    source_files: list[str] = []

    for root, dirs, files in os.walk("GravitySim"):
        for file in files:
            if not file.endswith(".c"):
                continue

            source_files.append(os.path.join(root, file))

    return source_files


setup(ext_modules=[Extension("GravitySim", get_source_files())])
