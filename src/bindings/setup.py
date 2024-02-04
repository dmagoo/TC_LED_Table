from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext
import platform

# Define platform-specific file extensions
if platform.system() == "Windows":
    pyd_file = 'python_bindings/ledtableapi_py.cp38-win_amd64.pyd'
elif platform.system() == "Linux":
    pyd_file = 'python_bindings/ledtableapi_py.so'  # Use .so for Linux

setup(
    name="ledtableapi",
    version="0.1",
    author="Dave MacGugan",
    description="Python bindings for LedTableApi",
    ext_modules=[
        Extension('ledtableapi', [pyd_file]),
    ],
)

