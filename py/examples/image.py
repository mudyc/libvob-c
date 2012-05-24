# Shows a rectangle in the window.

import libvob
import libvob.lob
import libvob.vob

from libvob.lob import Image, Margin


class MyButton:
    def create_lob(self, reg):
        print 'create lob in py'

        lob = Margin(Image("py/examples/test.png"), 2, 3, 4, 5)

        return lob

    def keypress(str):
        print 'key pressed', str

    def keyrelease(str):
        print 'key released', str


if __name__ == '__main__':
    win = libvob.Window(0,0,320,240)

    button = MyButton()
    def create_lob(reg):
        return button.create_lob(reg)

    print 'asdf'
    win.create_lob = create_lob
    #win.key_pressed = keypress
    #win.key_released = keyrelease

    libvob.main_loop(win)

