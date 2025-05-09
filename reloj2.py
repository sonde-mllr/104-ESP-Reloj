import tkinter as tk
import time
from datetime import datetime


# ========================================================================== #

#           Diccionarios creados por u/socal_nerdtastic en reddit
#           Además de corregir errores de operaciones condicionales
btn_style = dict(font=("Helvetica", 14),bg="gray20",fg="white",padx=10,pady=5)
lbl_style = dict(bg = "white",fg= "black",pady = 20,padx = 20)
big_font = dict(font=("Helvetica",48,"bold"))
normal_font = dict(font=("Helvetica",12,"bold"))

# ========================================================================== #


class Reloj(tk.Tk):
    def __init__(self):
        super().__init__()
        
        # Flags de control
        self.menuMin = 0
        self.menu = 2
        self.menuMax = 4
        self.hora_inicio = None
        self.cronometro = False

        self.title("Reloj con Tkinter")
        self.resizable(False, False)
        self.configure(bg="white",padx=20,pady=20)
        self.maxsize(width=300,height=240)
        self.geometry("300x240")
        self.etiqueta_hora = tk.Label(self, **big_font, **lbl_style)
        self.etiqueta_hora.pack()

        self.boton_mas = tk.Button(
            self,text="->",command=self.mas, **btn_style)
        self.boton_mas.pack(side="right",padx=10)

        self.boton_menos = tk.Button(
            self,text="<-",command=self.menos, **btn_style)
        self.boton_menos.pack(side="left",padx=10)

        self.indicador = tk.Label(self, **normal_font, **lbl_style,)
        self.indicador.pack()

        self.boton_tempo = tk.Button(
            self,text="Alternar crono",command=self.tempAlt, **btn_style)

        self.actualizar_reloj()


# FUNCIONES ===========================
    def actualizar_reloj(self):
        match(self.menu):
            case 0:
                self.etiqueta_hora.config(text="X")
            case 1:
                self.etiqueta_hora.config(text="papota")
            case 2:
                hora_actual = time.strftime('%H:%M:%S')
                self.etiqueta_hora.config(text=hora_actual)
            case 3:
                self.boton_tempo.pack()
                if self.cronometro:
                    segundos = int((datetime.now() - self.hora_inicio).total_seconds())
                    minutos, segundos = divmod(segundos, 60)
                    horas, minutos = divmod(minutos, 60)
                    crono = f"{horas:02d}:{minutos:02d}:{segundos:02d}"
                else:
                    crono = "00:00:00"  
                self.etiqueta_hora.config(text=crono)
            case 4:
                self.etiqueta_hora.config(text="X")

        self.after(1, self.actualizar_reloj)

        
        if self.menu != 3:
            if bool(self.boton_tempo.winfo_ismapped()) is True:
                self.boton_tempo.forget()

    def mas(self):
        print("¡Botón mas presionado!")
        if self.menu < self.menuMax:
            self.menu += 1
        self.indicador.config(text=self.menu)
        print(self.menu)

    def menos(self):
        print("¡Botón menos presionado!")
        if self.menu > self.menuMin:
            self.menu -= 1
        self.indicador.config(text=self.menu)
        print(self.menu)

    def tempAlt(self):
        print("cronometro alternado")
        self.cronometro = not self.cronometro
        if self.cronometro:
            self.hora_inicio = datetime.now()
        else:
            self.hora_inicio = None
        print(f"cronometro está : {self.cronometro}")

def main():
    app = Reloj()
    app.mainloop()

if __name__ == "__main__":
    main()
