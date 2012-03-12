# Shows a rectangle in the window.

import libvob
print dir(libvob)

import libvob.lob
print dir(libvob.lob)

import libvob.vob
print dir(libvob.vob)


from libvob.lob import RoundedRect, Margin, Stack
from libvob.vob import Shade, Color

def c(s):
    return Color(int(s[0:2],16)/255.,int(s[2:4],16)/255.,int(s[4:6],16)/255.)

def create_lob(reg):
    print 'create lob in py'

    color = Color(.9, .2, .5)
    print 'color', color
    shade = Shade(1, 0, 
                  #[0., Color(.9, .2, .5),
                  # 1., Color(.2, .5, .3)])
                  [0, c("58595b"),
                   0.22, c("a7a7a7"),
                   0.45, c("454547"),
                   0.60, c("fefefe"),
                   0.80, c("adadad"),
                   1, c("fafafa")])

    lob = RoundedRect(shade, 10.,20.,30.,40.)

    center = Margin(RoundedRect(Shade(1,0,
                                      [0, c("58595b"),
                                       0.60, c("fefefe"),
                                       1, c("53525b"),
                                       ]), 
                                40,40,40,40), 20,20,20,20)
    lob = Stack([center, lob])

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

