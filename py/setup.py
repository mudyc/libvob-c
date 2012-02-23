from distutils.core import setup, Extension
import os
import popen2

line = popen2.popen2('pkg-config --cflags glib-2.0')[0].readline()
print line
inc_dirs = ['../src/']
for s in line.split(" "):
    if s.startswith('-I'):
        inc_dirs.append(s[2:])
    

module1 = Extension('libvob',
                    sources = ['libvob.c'],
                    include_dirs = inc_dirs,
                    library_dirs = ['../'],
                    libraries = ['vob-c', 'glib-2.0', 'mcheck'],
                    runtime_library_dirs = [os.path.abspath('..')],
                    )

setup (name = 'PyLibvob',
       version = '0.0.1',
       description = 'A python bindings for Libvob-c',
       #ext_package='libvob',
       ext_modules = [module1])

