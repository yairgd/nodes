from distutils.core import setup, Extension


nodes_c = Extension('Algo.Nodes.Node', ['./src/py_node.c','./src/py_output.c','./src/py_node_manager.c','./src/py_init.c'],libraries=['pthread'],define_macros=[('aDEBUG', None),('PYTHON',None)], extra_compile_args=['-O0','-g'],include_dirs=['../src/'],extra_objects=['../src/libnode.a'])




setup (name = 'Algo',
       version = '1.0',
       author='Yair Gadelov',
       author_email='yair.gadelov@gmail.com',
       description = 'This is nodes packgae',
       ext_modules = [nodes_c],
       packages=['Algo','Algo.Nodes','Algo.Gui'])
