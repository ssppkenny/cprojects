from distutils.core import setup, Extension
setup(name="avltree", version="1.0",
      ext_modules=[Extension("avltree", ["avltree.c"])])
