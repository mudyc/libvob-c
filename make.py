#
# The simple make
# ===============
# 
# What are makes made of? From targets, dependencies, definitions and
# commands.
#
# target: dependencies
#       commands
#
# This is what make util describes and I would be happy with it if it
# would work. Commands produces files, or make transformations or
# such.  Let's make it visible, mark it as "target outputs file,
# target: -> f".
#
# compile: -> f
#     gcc -c ...
#
# What else? If we want to...
#
# lib: object_files(c_files()) -> f
#     gcc -shared ...
#
# ..we havea function called lib which takes object files as input and
# produces one file. We need to reduct c_files function.
#

import ply.lex as lex
import subprocess
import sys, os

__version__ = "1.9"
version = "1.9"
tokens = [
    'SPACE',
    'NAME',
    'OPEN_PAREN',
    'CLOSE_PAREN',
    'ARROW',
    'OPEN_BRACKET',
    'CLOSE_BRACKET',
    'COLON',
    'COMMA',
    'COMMENT',
    'ASTERISK'
]

t_SPACE = r'[ \t\r]+' #" \t\r"
t_NAME = r'[A-Za-z_~][A-Za-z0-9_]*'
t_ARROW = r'->'
t_OPEN_PAREN = r'\('
t_CLOSE_PAREN = r'\)'
t_OPEN_BRACKET = r'\['
t_CLOSE_BRACKET = r'\]'
t_COLON = r':'
t_COMMA = r','
t_COMMENT = r'\#.*'
t_ASTERISK = r'\*'

def t_error(v):
    #print "Lex error: ", v
    v.lexer.skip(1)

lex.lex()

dbg = False
targets = {}



class Type:
    def __init__(self, tokens):
        if len(tokens) == 1:
            self.type = tokens[0].value
            tokens.pop(0)
        else: raise RuntimeError("not a type")
    def is_mess(self):
        return self.type == '*'
    def is_file(self):
        return self.type == 'f'

class Arr(Type):
    def __init__(self, tokens):
        self.type = create_type(tokens)
    def is_file(self):
        return self.type.is_file()

def create_type(tokens):
    if tokens[0].type == 'OPEN_BRACKET':
        tokens.pop(0)
        tokens.pop()
        return Arr(tokens)
    else:
        return Type(tokens)


class Dep:
    def __init__(self, tokens):
        self.name = tokens.pop(0).value
        self.params = []
        if tokens[0].type == 'OPEN_PAREN':
            tokens.pop(0)
            self.params.append(Dep(tokens))
            if tokens[0].type != 'CLOSE_PAREN':
                raise RuntimeError('params')
            tokens.pop(0)
    def flat(self, params):
        if len(params) == 0: yield []
        for i in range(len(params)):
            if not isinstance(params[i], list):
                params[i] = [params[i]]
        idx = map(lambda x: 0, params)
        if dbg: print 'flat', idx, params
        run = True
        while run:
            #print params, idx
            yield map(lambda x,y: x[y], params, idx)
            for i in range(len(idx)):
                #print 'a',i,len(idx), idx[i], len(params[i])
                if idx[i] + 1 >= len(params[i]):
                    if i == len(idx) -1:
                        run = False
                    continue
                else: 
                    idx[i]+=1
                    for j in range(i): idx[j] = 0
                    break

    def is_variable(self):
        return len(self.params) == 0
    def reduce(self, input_params):
        if dbg: print 'reduce dep', self.name, input_params
        params = []
        for d in self.params:
            params.append(d.reduce(input_params))
        if self.is_variable():
            if dbg: print params, input_params
            # let's see if we have input parameter definition for that!
            for k,v in input_params:
                if k == self.name: 
                    return v
            return targets[self.name].reduce([])
        else: # target, function like dependant
            #   just call it right away with reduced params.
            ret = []
            for pars in self.flat(params):
                ret += targets[self.name].reduce(pars)
            if dbg: print 'depreduce ret',ret
            return ret

class Target:
    def __init__(self, tokens):
        tokens = filter(lambda t: t.type != 'SPACE', tokens)
        self.name = tokens.pop(0).value
        self.params = []
        self.deps = []
        self.cmds = []

        # input params
        if tokens[0].type == 'OPEN_PAREN':
            tokens.pop(0)
            self.params.append(tokens.pop(0).value)
            if tokens[0].type != 'CLOSE_PAREN':
                raise RuntimeError('params')
            tokens.pop(0)
        # deps
        if tokens[0].type == 'COLON':
            tokens.pop(0)
            while tokens[0].type != 'ARROW':
                self.deps.append(Dep(tokens))
        # out type
        if tokens[0].type != 'ARROW':
            raise RuntimeError('arrow missing')
        tokens.pop(0)
        self.out_type = create_type(tokens)
        if len(tokens) != 0:
            raise RuntimeError("all nothandled?! "+str(tokens))
        
    def add_cmd(self, cmd):
        self.cmds.append(cmd)

    def time_dep(self, files):
        for i in files: 
            if not os.path.exists(i): return 2**40
        return max(map(lambda x: os.path.getmtime(x), files))

    def time_target(self,inputs):
        outs = self.make_it(inputs, False)
        for i in outs: 
            if not os.path.exists(i): 
                return -1
        return min(map(lambda x: os.path.getmtime(x), outs))
    def output_is_old(self, inputs, file_deps):
        if dbg: print 'is old? ', inputs, file_deps
        if len(self.deps) == 0:
            return True
        elif self.out_type.is_mess():
            return True
        elif self.out_type.is_file():
            return self.time_target(inputs) < self.time_dep(file_deps)

    def make_it(self, inputs, fullrun):
        if self.out_type.type == 'str':
            return " ".join(self.cmds)
        else:
            if dbg: print 'make it', self.name

        _last = None
        for cmd in self.cmds:
            if dbg: print cmd, inputs
            if cmd.startswith('!py:') or cmd.startswith('py:'):
                for i in inputs: exec i[0]+'="'+i[1]+'"'
                if cmd[0] == '!':
                    exec('_last='+cmd[4:].strip())
                else:
                    exec(cmd[3:].strip())
                if not fullrun and cmd[0] == '!':
                    if dbg: print 'small bits',_last.split('\n')
                    return _last.split('\n')
            else:
                env = ""
                for k,v in locals().items(): 
                    if isinstance(k, str) and isinstance(v, str) and k in cmd:
                        env += " "+k+'="'+v+'";'
                #print inputs + self.env()
                for (k,v) in inputs + self.env():
                    if k in cmd:
                        env += " "+k+'="'+v+'";'
                #all = self.env("", ";") + " "+env+" "+cmd
                all = [env+cmd]
                if dbg: print all

                if fullrun:
                    print all
                p = subprocess.Popen(all, stdout=subprocess.PIPE,
                                     stderr=subprocess.PIPE, shell=True)
                stdout, stderr = p.communicate()
                #print stdout, stderr
                #print stdout
                if p.returncode != 0:
                    print 'out:\n',stdout
                    print 'err:\n',stderr
                    raise RuntimeError(p.returncode)
                
                if _last == None:
                    _last = stdout

        if dbg: print _last
        _last = filter(lambda x: x != "", _last.split('\n'))
        for i in range(len(_last)):
            if ' ' in _last[i]:
                _last += _last[i].split(' ')
                del _last[i]
                
        #import traceback
        #traceback.print_stack()
        if dbg: print 'make_it ret',_last
        return _last
        

        
    def reduce(self, inputs):
        if dbg: print 'reduce', self.name, inputs
        inputs = map(lambda k,v: (k,v), self.params, inputs)
        file_deps = []
        for d in self.deps:
            red = d.reduce(inputs)
            inputs.append((d.name, " ".join(red)))
            file_deps += red
        if dbg: print inputs, file_deps

        #d.reduce(self.params)
        return self.make_it(inputs, self.output_is_old(inputs, file_deps))

    def make(self):
        self.reduce([])

    def env(self):
        def testenv(it):
            (k,v) = it
            if k in os.environ:
                return (k, os.environ[k])
            else: return (k, v)
        return map(testenv, 
                   map(lambda (k,v): (k, v.make_it([],None)),
                       filter(lambda (k,v): v.out_type.type == 'str', 
                              targets.items())))


if __name__ == '__main__':
    f = open('Makefile.2')
    

    active_target = None
    for line in f.readlines():
        line = line[:-1]
        if dbg: print line 
        lex.input(line)
        tokens = []
        while True:
            tok = lex.token()
            if not tok: break
            tokens.append(tok)
        if len(tokens) == 0: continue
        if tokens[0].type == 'SPACE':
            if active_target == None:
                raise RuntimeError('No target defined.')
            active_target.add_cmd(line.strip())
        elif tokens[0].type == 'NAME':
            target = Target(tokens)
            active_target = target
            targets[tokens[0].value] = target
        else:
            raise RuntimeError("Unknown line: "+line)

    import sys
    targets[sys.argv[1]].make()

