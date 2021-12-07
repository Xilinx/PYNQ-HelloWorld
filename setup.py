#   Copyright (c) 2019, Xilinx, Inc.
#   All rights reserved.
# 
#   Redistribution and use in source and binary forms, with or without 
#   modification, are permitted provided that the following conditions are met:
#
#   1.  Redistributions of source code must retain the above copyright notice, 
#       this list of conditions and the following disclaimer.
#
#   2.  Redistributions in binary form must reproduce the above copyright 
#       notice, this list of conditions and the following disclaimer in the 
#       documentation and/or other materials provided with the distribution.
#
#   3.  Neither the name of the copyright holder nor the names of its 
#       contributors may be used to endorse or promote products derived from 
#       this software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
#   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
#   PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
#   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
#   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
#   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#   OR BUSINESS INTERRUPTION). HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
#   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
#   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
#   ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

__author__ = "Yun Rock Qu"
__copyright__ = "Copyright 2020, Xilinx"
__email__ = "pynq_support@xilinx.com"


from setuptools import setup, find_packages
import os
import platform
import re
from pynq.utils import build_py


# global variables
module_name = "pynq_helloworld"
data_files = []
current_platform = ""


# parse version number
def find_version(file_path):
    with open(file_path, 'r') as fp:
        version_file = fp.read()
        version_match = re.search(r"^__version__ = ['\"]([^'\"]*)['\"]",
                                  version_file, re.M)
    if version_match:
        return version_match.group(1)
    raise NameError("Version string must be defined in {}.".format(file_path))


# extend package
def extend_package(path):
    if os.path.isdir(path):
        data_files.extend(
            [os.path.join("..", root, f)
             for root, _, files in os.walk(path) for f in files]
        )
    elif os.path.isfile(path):
        data_files.append(os.path.join("..", path))


# get current platform: either edge or pcie
def get_platform():
    cpu = platform.processor()
    if cpu in ['armv7l', 'aarch64']:
        return "edge"
    elif cpu in ['x86_64']:
        return "pcie"
    else:
        raise OSError("Platform is not supported.")


pkg_version = find_version('{}/__init__.py'.format(module_name))
with open("README.md", encoding='utf-8') as fh:
    readme_lines = fh.readlines()[2:47]
long_description = (''.join(readme_lines))
extend_package(os.path.join(module_name, "notebooks"))


setup(
    name=module_name,
    version=pkg_version,
    description="PYNQ example design supporting edge and PCIE boards",
    long_description=long_description,
    long_description_content_type='text/markdown',
    author='Xilinx PYNQ Development Team',
    author_email="pynq_support@xilinx.com",
    url='https://github.com/Xilinx/PYNQ-HelloWorld.git',
    license='BSD 3-Clause License',
    packages=find_packages(),
    package_data={
        "": data_files,
    },
    python_requires=">=3.6.0",
    install_requires=[
        "pynq",
        "matplotlib",
        "ipython"
    ],
    entry_points={
        "pynq.notebooks": [
            "pynq-helloworld = {}.notebooks.{}".format(
                module_name, get_platform())
        ]
    },
    cmdclass={"build_py": build_py}
)
