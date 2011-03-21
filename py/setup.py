from distutils.core import setup, Extension
import os

module1 = Extension('libvob',
                    sources = ['libvob.c'],
                    include_dirs = ['../src/',
                                    '/usr/include/glib-2.0',
                                    '/usr/lib/glib-2.0/include' ],
                    library_dirs = ['../'],
                    libraries = ['vob-c', 'glib-2.0'],
                    runtime_library_dirs = [os.path.abspath('..')],
                    )

setup (name = 'PyLibvob',
       version = '0.0.1',
       description = 'A python bindings for Libvob-c',
       #ext_package='libvob',
       ext_modules = [module1])

