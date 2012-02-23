# Stacked layers.
import libvob

import libvob.lob
print dir(libvob.lob)


from libvob.lob import Rect, Color, Vbox, Hbox, Font, FontText, Hglue, Vglue, Stack

def create_lob(reg):
    print 'create lob in py'
    print dir(Font)
    print Font("Sans")
    
    #for i in Font.list(): print i.family

    row = Hbox([
        Rect(Color(1,0,0)), Rect(Color(0,1,0)), Rect(Color(0,0,1))
        ])
    
    lob = Vbox([row,
                Rect(Color(.7, .8, .9)),
                Rect(Color(.4, .5, .6))])

    fonts = Font.list()
    texts = [Vglue()]
    for f in fonts[3:15]:
        texts.append(Hbox([FontText(f, 12, "Hello!"), Hglue()]))
        texts.append(Vglue())

    return Stack([Vbox(texts), lob])

if __name__ == '__main__':
    win = libvob.Window(0,0,2*320,2*240)
    win.create_lob = create_lob
    #win.key_pressed = keypress
    #win.key_released = keyrelease

    libvob.main_loop(win)

