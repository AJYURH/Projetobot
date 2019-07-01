import matplotlib
matplotlib.use("TkAgg")
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg,NavigationToolbar2Tk
from matplotlib.figure import Figure
import tkinter as tk;
from tkinter import ttk
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import servidorUdp
import threading
from matplotlib import style


LARGE_FONT = ("Verdana",12)
timeB = np.arange(50)

tempS1 = [20.0,30.0,10.0,20.0,30.0,50.0,70.0,20.0,50.0,60.0]
tempS2 = [40.0,30.0,10.0,20.0,30.0,50.0,70.0,20.0,50.0,60.0]
tempS3 = [10.0,30.0,10.0,20.0,30.0,45.0,20.0,20.0,50.0,60.0]
tempS4 = [0.0,30.0,10.0,5.0,30.0,50.0,70.0,35.0,50.0,60.0]



f = Figure(figsize=(5,5),dpi=100)
a = f.add_subplot(221)
b = f.add_subplot(222)
c = f.add_subplot(223)
d = f.add_subplot(224)



def animate(i):
    adado = servidorUdp.flys[0].temps[:50]
    bdado = servidorUdp.flys[1].temps[:50]
    cdado = servidorUdp.flys[2].temps[:50]
    ddado = servidorUdp.flys[3].temps[:50]

    a.clear()
    a.plot(timeB,adado)

    b.clear()
    b.plot(timeB,bdado)

    c.clear()
    c.plot(timeB,cdado)

    d.clear()
    d.plot(timeB,ddado)







class ProjetoRobapp(tk.Tk):
    def __init__(self,*args,**kwargs):
        tk.Tk.__init__(self,*args,**kwargs)
        tk.Tk.wm_title(self,"ProjetoRob")
        container = tk.Frame()
        container.pack(side="top",fill="both",expand= True)

        container.grid_rowconfigure(0, weight = 1)
        container.grid_columnconfigure(0, weight = 1)

        self.frames = {}

        for F in (StartPage, PageOne,PageTwo):
            frame = F(container, self)

            self.frames[F] = frame

            frame.grid(row = 0, column = 0, sticky = "nsew")


        
        self.show_frame(StartPage)

    def show_frame(self, cont):
        frame = self.frames[cont]
        frame.tkraise()

class StartPage(tk.Frame):
    def __init__(self,parent,controller):
        tk.Frame.__init__(self,parent)
        label = ttk.Label(self,text="Start Page",font=LARGE_FONT)
        label.pack(pady=10,padx=10)
         
        botao1 = ttk.Button(self, text = "Visit Pagina 1:",command = lambda:controller.show_frame(PageOne))
        botao1.pack()
        botao2 = ttk.Button(self, text = "Graficos:",command = lambda:controller.show_frame(PageTwo))
        botao2.pack()
        botao2 = ttk.Button(self, text = "Reinicia:",command = lambda : restatingAnimatio()  )
        botao2.pack()

class PageOne(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        label = ttk.Label(self,text="Page ONe",font=LARGE_FONT)
        label.pack(pady=10,padx=10)
            
        botao1 = ttk.Button(self, text = "Voltar para página inicial:",command = lambda x: controller.show_frame(StartPage) )
        botao1.pack()
        

class PageTwo(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        label = ttk.Label(self,text="Pagina dos Gráficos",font=LARGE_FONT)
        label.pack(pady=10,padx=10)
            
        botao1 = ttk.Button(self, text = "Voltar para página inicial:",command = lambda :controller.show_frame(StartPage))
        botao1.pack()


        #plt.tight_layout()
        
        canvas = FigureCanvasTkAgg(f,self)
        canvas.draw()
        canvas.get_tk_widget().pack(side=tk.TOP,fill=tk.BOTH,expand=True)
        canvas._tkcanvas.pack(side=tk.TOP,fill=tk.BOTH,expand=True)
        
        print('end')



#if __name__ == "__main__":
threadTemp =threading.Thread(target=servidorUdp.iniciaLeitura,args=(),daemon=False)
threadTemp.start()
print("passei aqui")
app = ProjetoRobapp()
ani = animation.FuncAnimation(f,animate,interval=1000)
app.mainloop()