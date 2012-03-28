# Shows a rectangle in the window.

import libvob
import libvob.lob
import libvob.vob

from libvob.lob import Button, ClickModel
from libvob.vob import Shade, Color

class MyButton:
    def __init__(self):
        self.model = ClickModel()

    def create_lob(self, reg):
        print 'create lob in py'

        lob = Button("Press On", self.model)

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

