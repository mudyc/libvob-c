# Shows a rectangle in the window.

import libvob
print dir(libvob)

import libvob.lob
print dir(libvob.lob)

import libvob.vob
print dir(libvob.vob)


from libvob.lob import Rect
from libvob.vob import Color


#def create_lob(reg):
#    print 'create lob in python\n\n'
#    return reg.comp_rect(reg.comp_color(1, .5, .2))

# from libvob.lob import Rect, Color
# def create_lob(reg):
#     push_reg(reg)
#     lob = Rect(Color(1, .5, .2))
#     pop_reg()
#     return lob


def create_lob(reg):
    print 'create lob in py'

    color = Color(.9, .2, .5)
    print 'color', color
    lob = Rect(color)
    return lob


def keypress(str):
    print 'key pressed', str

def keyrelease(str):
    print 'key released', str


if __name__ == '__main__':
    win = libvob.Window(0,0,320,240)
    print 'asdf'
    win.create_lob = create_lob
    #win.key_pressed = keypress
    #win.key_released = keyrelease

    libvob.main_loop(win)

