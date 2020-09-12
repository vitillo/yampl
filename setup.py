import os
import re
import sys
import sysconfig
import platform
import subprocess

from distutils.version import LooseVersion
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext


class ArgParserHelper:
    """Helper class to parse arguments"""
    def __init__(self, argv):
        self.args = list(argv)
        self.rx = re.compile(r'^-D([a-zA-Z0-9_\-]+)=([^*&%\s]+)$')

    def parse(self):
        kwargs = {}

        try:
            for arg in self.args:
                m = self.rx.match(arg)

                if m is not None:
                    if not m.group(1) in kwargs:
                        kwargs[m.group(1)] = m.group(2)
                    else:
                        raise ValueError('Duplicate argument')
        except ValueError as e:
            print('Argument format is invalid: %s' % e.message)

        return kwargs

    def purge(self):
        for arg in self.args:
            args = list(self.args)
            m = self.rx.match(arg)

            if m is not None:
                args.remove(m.group())
            
        return args


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError(
                "CMake must be installed to build the following extensions: " +
                ", ".join(e.name for e in self.extensions))

        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)',
                                         out.decode()).group(1))
            if cmake_version < '3.1.0':
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(
            os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DPYTHON_EXECUTABLE=' + sys.executable]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        # WITH_PYBIND=ON && PYBIND_ONLY=ON
        cmake_args += ['-DPYPI_INVOKE=ON']

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(
                cfg.upper(),
                extdir)]
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            build_args += ['--', '-j2']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(
            env.get('CXXFLAGS', ''),
            self.distribution.get_version())

        # self.build_temp += "yampl%s.d" % subprocess.check_output([sys.executable + '-config', '--extension-suffix'])

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        if not os.path.exists(extdir):
            os.makedirs(extdir)

        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args,
                              cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args,
                              cwd=self.build_temp)
        print()


# Filter custom arguments
arg_parser = ArgParserHelper(sys.argv)
sys.argv = arg_parser.purge()

# Setup
setup(
    name='yampl',
    version='1.0',
    author='Ayoub Chouak',
    author_email='a.chouak@protonmail.com',
    description='Python bindings for YAMPL',
    long_description='',
    ext_modules=[CMakeExtension('yampl')],
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
)


