import gtk
#from gi.repository import Gtk as gtk
class ConnectionPoint(gtk.DrawingArea):
    def __init__(self,layout,x,y,text):
        gtk.DrawingArea.__init__(self)
        self.drag = False
        self.drag_x = 0
        self.drag_y = 0
        self.layout = layout
        self.x = x
        self.y = y
        self.pangolayout = self.create_pango_layout(text)
        self.w,self.h = self.pangolayout.get_pixel_size();
        self.w+=10
        self.h+=10
        self.set_size_request(self.w, self.h)
       
        print self.w,self.h
        #self.style = self.get_style()
#        self.gc = self.style.fg_gc[gtk.STATE_NORMAL]
       # self.window.draw_rectangle(self.gc, True, x+10, y+10, 20, 20)
        #self.event_box = gtk.EventBox();
        self.set_events(gtk.gdk.POINTER_MOTION_MASK | gtk.gdk.BUTTON_PRESS_MASK | gtk.gdk.BUTTON_RELEASE_MASK)
        self.connect("button-press-event", self.click)
        self.connect("button-release-event", self.release)
        self.connect("motion-notify-event", self.mousemove)
        #self.event_box.add(self);
        #self.set_events(gtk.gdk.POINTER_MOTION_MASK |
        #                     gtk.gdk.POINTER_MOTION_HINT_MASK )
        

        self.layout.put( self, x, y )
        self.show();
        self.connect("expose-event", self.area_expose_cb)        
    def click(self, widget, event):
        self.drag =  True
        self.drag_x =  event.x
        self.drag_y =  event.y
        print(self.drag_x, self.drag_y)
        
    def area_expose_cb(self, area, event):
        style = self.get_style()
        gc = style.fg_gc[gtk.STATE_NORMAL]
      #  area.window.draw_point(gc,  self.w,self.h)
     #   self.pangolayout.set_text("Point")
        area.window.draw_rectangle(gc, False, 0, 0, self.w-1, self.h-1)
        #area.window.draw_rectangle(gc, True, 0, 0, 20, 20)
        area.window.draw_layout(gc, 2, 2, self.pangolayout)
        
       # print gc,area,self


    def release(self, widget, event):
        self.drag =  False

    def mousemove(self,widget,event):
       # if self.drag:
       #     self.layout.move(self,self.x+int(event.x-self.drag_x),self.y+int(event.y-self.drag_y))
       #     self.x, self.y = self.layout.child_get(self,'x','y')





class Compoent(gtk.DrawingArea):
    def __init__(self,layout,x,y,text):
        gtk.DrawingArea.__init__(self)
        self.drag = False
        self.drag_x = 0
        self.drag_y = 0
        self.layout = layout
        self.x = x
        self.y = y
        self.pangolayout = self.create_pango_layout(text)
        self.w,self.h = self.pangolayout.get_pixel_size();
        self.w+=10
        self.h+=10
        self.set_size_request(self.w, self.h)
       
        print self.w,self.h
        #self.style = self.get_style()
#        self.gc = self.style.fg_gc[gtk.STATE_NORMAL]
       # self.window.draw_rectangle(self.gc, True, x+10, y+10, 20, 20)
        #self.event_box = gtk.EventBox();
        self.set_events(gtk.gdk.POINTER_MOTION_MASK | gtk.gdk.BUTTON_PRESS_MASK | gtk.gdk.BUTTON_RELEASE_MASK)
        self.connect("button-press-event", self.click)
        self.connect("button-release-event", self.release)
        self.connect("motion-notify-event", self.mousemove)
        #self.event_box.add(self);
        #self.set_events(gtk.gdk.POINTER_MOTION_MASK |
        #                     gtk.gdk.POINTER_MOTION_HINT_MASK )
        

        self.layout.put( self, x, y )
        self.show();
        self.connect("expose-event", self.area_expose_cb)        
    def click(self, widget, event):
        self.drag =  True
        self.drag_x =  event.x
        self.drag_y =  event.y
        print(self.drag_x, self.drag_y)
        
    def area_expose_cb(self, area, event):
        style = self.get_style()
        gc = style.fg_gc[gtk.STATE_NORMAL]
      #  area.window.draw_point(gc,  self.w,self.h)
     #   self.pangolayout.set_text("Point")
        area.window.draw_rectangle(gc, False, 0, 0, self.w-1, self.h-1)
        #area.window.draw_rectangle(gc, True, 0, 0, 20, 20)
        area.window.draw_layout(gc, 2, 2, self.pangolayout)
        
       # print gc,area,self


    def release(self, widget, event):
        self.drag =  False

    def mousemove(self,widget,event):
        if self.drag:
            self.layout.move(self,self.x+int(event.x-self.drag_x),self.y+int(event.y-self.drag_y))
            self.x, self.y = self.layout.child_get(self,'x','y')


class move_test(object):
    def __init__(self):
        window =  gtk.Window()
        window.connect("destroy", lambda w: gtk.main_quit())
        
        layout =  gtk.Layout()
        
        Compoent(layout,70,70,"one");
        Compoent(layout,90,70,"two");

        Compoent(layout,40,70,"three");

        Compoent(layout,20,70,"four");



        window.add(layout)
        window.show_all()
       # print da.window
move_test()
gtk.main()
