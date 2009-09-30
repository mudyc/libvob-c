# Shows a rectangle in the window.

import libvob


def create_lob(reg):
    print 'create lob in python\n\n'
    return reg.comp_rect(reg.comp_color(1, .5, .2))

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

