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


from setuptools import setup, find_packages, Distribution
from setuptools.command.build_ext import build_ext
from distutils.dir_util import copy_tree
from distutils.file_util import copy_file
import os
import subprocess
import re
import shutil


# Parse version number
def find_version(file_path):
    with open(file_path, 'r') as fp:
        version_file = fp.read()
        version_match = re.search(r"^__version__ = ['\"]([^'\"]*)['\"]",
                                  version_file, re.M)
    if version_match:
        return version_match.group(1)
    raise NameError("Version string must be defined in {}.".format(file_path))


# global variables
data_files = []
download_links = {
    'Pynq-Z1': [
        'https://dl.dropboxusercontent.com/s/m5ay6j4to7ryr4p/resizer.bit',
        'https://dl.dropboxusercontent.com/s/1qiy3124wjmq803/resizer.hwh'],
    'Pynq-Z2': [
        'https://dl.dropboxusercontent.com/s/bwqk0uzdc4o9afd/resizer.bit',
        'https://dl.dropboxusercontent.com/s/vdcwzhsvstko4yj/resizer.hwh'],
    'Ultra96': [
        'https://dl.dropboxusercontent.com/s/l1n2903aj5sac8e/resizer.bit',
        'https://dl.dropboxusercontent.com/s/2greuk281iu0txb/resizer.hwh'],
    'ZCU104': [
        'https://dl.dropboxusercontent.com/s/i9vcgcv2pqlszrr/resizer.bit',
        'https://dl.dropboxusercontent.com/s/mlx1shzyyn8oqgs/resizer.hwh'],
}


# check whether board is supported
def check_env():
    global board, notebooks_dir

    if 'BOARD' not in os.environ:
        raise ValueError(
            'Set BOARD variable in the environment.')
    board = os.environ['BOARD']

    if 'PYNQ_JUPYTER_NOTEBOOKS' not in os.environ:
        raise ValueError(
            'Set PYNQ_JUPYTER_NOTEBOOKS variable in the environment.')
    notebooks_dir = os.environ['PYNQ_JUPYTER_NOTEBOOKS']


# copy notebooks to jupyter home
def copy_notebooks():
    src_nb_dir = os.path.join('boards', board, 'resizer', 'notebooks')
    dst_nb_dir = os.path.join(notebooks_dir, 'pynq-helloworld')
    if os.path.exists(dst_nb_dir):
        shutil.rmtree(dst_nb_dir)
    copy_tree(src_nb_dir, dst_nb_dir)


# Enforce platform-dependent distribution
class BinaryDistribution(Distribution):
    def has_ext_modules(self):
        return True


# Build extension
class BuildExtension(build_ext):
    def download_files(self):
        if board not in download_links:
            raise ValueError(
                "Board {} is probably not supported.".format(board))

        board_links = download_links[board]
        dst_ol_dir = os.path.join(self.build_lib, 'pynqhelloworld', 'overlays')
        if not os.path.isdir(dst_ol_dir):
            os.makedirs(dst_ol_dir)
        for i in board_links:
            _ = subprocess.check_output(['wget', '-q', '-P', dst_ol_dir,
                                         i, '--no-check-certificate'])
        data_files.extend(
            [os.path.join("..", dst_ol_dir, f) for f in os.listdir(dst_ol_dir)])

    def install_overlays(self):
        src_ol_dir = os.path.join('boards', board, 'resizer')
        dst_ol_dir = os.path.join(self.build_lib, 'pynqhelloworld', 'overlays')
        [copy_file(os.path.join(src_ol_dir, f), dst_ol_dir)
            for f in os.listdir(src_ol_dir) if f.endswith('.py')]

    def run(self):
        check_env()
        copy_notebooks()
        self.download_files()
        self.install_overlays()
        build_ext.run(self)


pkg_version = find_version('pynqhelloworld/__init__.py')
with open("README.md", encoding='utf-8') as fh:
    readme_lines = fh.readlines()[2:6]
long_description = (''.join(readme_lines))

setup(
    name="pynqhelloworld",
    version=pkg_version,
    description="PYNQ example design supporting PYNQ-enabled boards",
    long_description=long_description,
    long_description_content_type='text/markdown',
    author='Xilinx PYNQ Development Team',
    author_email="pynq_support@xilinx.com",
    install_requires=['pynq>=2.5'],
    url='https://github.com/Xilinx/PYNQ-HelloWorld.git',
    license='BSD 3-Clause License',
    packages=find_packages(),
    cmdclass={
        "build_ext": BuildExtension,
    },
    distclass=BinaryDistribution,
    python_requires='>=3.6.0',
    package_data={
        'pynqhelloworld': data_files,
    },

)
