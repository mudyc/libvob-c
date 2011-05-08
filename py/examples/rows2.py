# Shows a rectangles in a column.
import libvob

import libvob.lob
print dir(libvob.lob)


from libvob.lob import Rect, Color, VBox, HBox

def create_lob(reg):
    print 'create lob in py'
    row = HBox([
        Rect(Color(1,0,0)), Rect(Color(0,1,0)), Rect(Color(0,0,1))
        ])
    
    lob = VBox([row,
                Rect(Color(.7, .8, .9)),
                Rect(Color(.1, .2, .3)),
                Rect(Color(.4, .5, .6))])
    return lob

if __name__ == '__main__':
    win = libvob.Window(0,0,320,240)
    win.create_lob = create_lob
    #win.key_pressed = keypress
    #win.key_released = keyrelease

    libvob.main_loop(win)
