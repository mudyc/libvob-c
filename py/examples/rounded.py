# Shows a rectangle in the window.

import libvob
print dir(libvob)

import libvob.lob
print dir(libvob.lob)

import libvob.vob
print dir(libvob.vob)


from libvob.lob import RoundedRect
from libvob.vob import Shade, Color


def create_lob(reg):
    print 'create lob in py'

    color = Color(.9, .2, .5)
    print 'color', color
    shade = Shade(1, 0, 
                  [0., Color(.9, .2, .5),
                   1., Color(.2, .5, .3)])
    lob = RoundedRect(shade, 1.,2.,3.,4.)
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

