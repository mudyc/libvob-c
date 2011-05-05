# (c) Matti Katila, 2011
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

def gen(name, clzz, funcs):
    struct = 'Py'+name
    typee = 'Py'+name+'Type'
    py_name = 'libvob.'+name
    nice_name = name
    pre_str = 'libvob'
    if name.startswith('Lob'):
        pre_str = 'lob'
        if len(name) > 3:
            nice_name = name[3:]
        py_name = 'libvob.lob.'+nice_name
    if name.startswith('Vob'):
        pre_str = 'vob' 
        if len(name) > 3:
            nice_name = name[3:]
        py_name = 'libvob.vob.'+nice_name
    

    
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

    for f in funcs:
        if f['name'] == clzz['name']:

            types = ''
            params = ''
            s = ''
            if len(f['parameters']) > 1 \
                    and f['parameters'][0]['type'] == 'Region *':
                structs_and_types.append("""
PyObject *%s_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    //printf("%s_new\\n");

    %s *self;

    self = (%s *)type->tp_alloc(type, 0);
    if (self != NULL) {
""" % (typee, struct, struct, struct))

                for idx in range(len(f['parameters'])):
                    param = f['parameters'][idx]
                    if idx == 0 and param['type'] == 'Region *':
                        s += '        Region *p0 = region;\n'
                    else:
                        if param['type'] == 'float':
                            s += '        float p'+str(idx)+';\n' 
                            types += 'f'
                            params += '&p'+str(idx)+', '
                        elif param['type'].startswith('Lob'):
                            s += '        PyLob *p'+str(idx)+';\n' 
                            types += 'O'
                            params += '&p'+str(idx)+', '
                        elif param['type'] == 'char *':
                            s += '        const char *p'+str(idx)+';\n' 
                            types += 's'
                            params += '&p'+str(idx)+', '
                        else:
                            print param
                            raise 'asdf'

                s += '        if (! PyArg_ParseTuple(args, "%s", %s)) {\n' \
                    %(types, params[:-2])
                s += '            Py_DECREF(self);\n'
                s += '            return NULL;\n'
                s += '        }\n'
                s += '        self->obj = '+f['name']+'('
                for i in range(len(f['parameters'])): 
                    if i > 0:
                        s += ', '
                    s+= 'p'+str(i)
                    if f['parameters'][i]['type'].startswith('Lob'):
                        s+= '->obj'
                s += ');\n    }\n    return self;\n}\n\n'
                structs_and_types.append(s)

                init_type_ready.append(
                    "        %s.tp_new = %s_new;" % (typee, typee))
            elif f['parameters'][0]['type'] == 'Region *' and \
                 len(filter(lambda x: x['name'] == clzz['name']+'_add', funcs)) == 1:
                structs_and_types.append("""
PyObject *%s_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    //printf("%s_new\\n");

    %s *self;

    self = (%s *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->obj = %s(region);
    
        PyObject *list = NULL;
        if (! PyArg_ParseTuple(args, "O", &list)) {
            Py_DECREF(self);
            return NULL;
        }
        if (list != NULL && PySequence_Check(list)) {
            Py_ssize_t len = PySequence_Size(list);
            Py_ssize_t idx = 0;
            for (; idx<len; idx++) {
                //printf("got arg...%%i\\n", idx);
                PyObject *lob = PySequence_GetItem(list, idx);
                //if (PyObject_TypeCheck(lob, &PyLobType))
                    %s_add(region, ((%s*)self->obj)->items, ((PyLob*)lob)->obj);
                //else printf("asdf\\n");

            }
        }
    }
    return self;
}
        
""" % (typee, typee, struct, struct, clzz['name'], clzz['name'], name))

                init_type_ready.append(
                    "        %s.tp_new = %s_new;" % (typee, typee))
                
            else:
                init_type_ready.append(
                    "        %s.tp_new = PyType_GenericNew;"\
                        % (typee))
                

    init_type_ready.append("""
        %s.tp_flags = Py_TPFLAGS_DEFAULT;
        if (PyType_Ready(&%s) < 0)
            return;\n
""" % (typee,typee))

    init_add_objects.append("""
    Py_INCREF(&%s);
    PyModule_AddObject(%s, "%s", (PyObject *)&%s);
""" % (typee, pre_str, nice_name, typee))
                           
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

    all_funcs = []
    all_classes = {}
    for f in files:
        print f
        for func in f.func_protos:
            all_funcs.append(func)
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
        gen(name, all_classes[name], all_funcs)
        #print ind, name
        for nam, clzz in all_classes.items():
            if len(clzz['inherits']) > 0:
                if name == clzz['inherits'][0]['typedef']:
                    tree(nam, ind+'   ')

    for name, clzz in all_classes.items():
        if len(list(children(name))) == 0 and len(clzz['inherits']) == 0:
            #print name
            gen(name, clzz, all_funcs)
    for name, clzz in all_classes.items():
        if len(list(children(name))) > 0:
            tree(name, '')
    #print all_classes

    write('py/gen_structs.c', structs_and_types)
    write('py/gen_init_types.c', init_type_ready)
    write('py/gen_add_obs.c', init_add_objects)
