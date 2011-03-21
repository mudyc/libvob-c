# Creates the python bindings for libvov-c

import os
import sys
sys.path = ["./py/"] + sys.path
import CppHeaderParser
#CppHeaderParser.debug = True

structs_and_types = []
init_type_ready = []
init_add_objects = []


def write(f, list_of_data):
    f = file(f, 'w+')
    f.write('''// GENERATED FILE - DO NOT EDIT! \n''')
    for i in list_of_data:
        f.write(i)
    f.close()

def gen(name, clzz):
    struct = 'Py'+name
    typee = 'Py'+name+'Type'
    py_name = 'libvob.'+name
    structs_and_types.append("""
    /* *********************
    * %s stucture 
    ************************ */

    typedef struct {
        PyObject_HEAD
        %s *obj;
    } %s;

    static PyTypeObject %s = {
        PyObject_HEAD_INIT(NULL)
        0,\t\t\t\t\t/*ob_size*/
        "%s",\t\t\t/*tp_name*/
        sizeof(%s),\t\t\t/*tp_basicsize*/
    };
""" % ( struct, name, struct, typee, py_name, struct))

    init_type_ready.append("""
        if (PyType_Ready(&%s) < 0)
        return;
""" % (typee))

    init_add_objects.append("""
    Py_INCREF(&%s);
    PyModule_AddObject(m, "%s", (PyObject *)&%s);
""" % (typee, name, typee))
                           
if __name__ == '__main__':

    headers = []
    for path in ['src/lob', ]: #'src/vob']:
        for root, dirs, files in os.walk(path):
            if root.find('src/gfx') >= 0: continue

            for f in files:
                if f.endswith('.h'):
                    headers.append(root+'/'+f)

    files = []
    for hdr in headers:
        files.append(CppHeaderParser.CppHeader(hdr))

    all_classes = {}
    for f in files:
        print f
        for name, c in f.classes.items():
            if c['typedef'] != '':
                name = c['typedef']
            all_classes[name] = c

    for name, clzz in all_classes.items():
        #print name, clzz['typedef']
        if len(clzz['properties']['public']) > 0:
            var = clzz['properties']['public'][0]
            var_type = clzz['properties']['public'][0]['type']
            if var_type in all_classes.keys():
                clzz['inherits'].append(all_classes[var_type])


    def children(name):
        for nam, clzz in all_classes.items():
            if len(clzz['inherits']) > 0:
                if name == clzz['inherits'][0]['typedef']:
                    yield clzz

    # Sort the order by parenting.

    def tree(name, ind):
        gen(name, all_classes[name])
        #print ind, name
        for nam, clzz in all_classes.items():
            if len(clzz['inherits']) > 0:
                if name == clzz['inherits'][0]['typedef']:
                    tree(nam, ind+'   ')

    for name, clzz in all_classes.items():
        if len(list(children(name))) == 0 and len(clzz['inherits']) == 0:
            #print name
            gen(name, clzz)
    for name, clzz in all_classes.items():
        if len(list(children(name))) > 0:
            tree(name, '')
    #print all_classes

    write('py/gen_structs.c', structs_and_types)
    write('py/gen_init_types.c', init_type_ready)
    write('py/gen_add_obs.c', init_add_objects)
