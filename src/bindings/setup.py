from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "ledtableapi",
        ["src/bindings/api/led_table_api_bindings.cpp"],
        # Extra compile args can be specified if needed
    ),
]

setup(
    name="ledtableapi",
    version="0.1",
    author="Your Name",
    description="Python bindings for LedTableApi",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)
