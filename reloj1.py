import tkinter as tk
import time
from datetime import datetime
class Reloj(tk.Tk):
    def __init__(self):
        super().__init__()
        self.menuMin = 0
        self.menu = 2
        self.menuMax = 4
        self.hora_inicio = None
        self.boton_temp = False

        self.title("Reloj con Tkinter")
        self.resizable(False, False)
        self.configure(bg="white")

        self.etiqueta_hora = tk.Label(
            self,
            font=("Helvetica", 48, "bold"),
            bg="white",
            fg="black",
            pady=20,
            padx=20
        )
        self.etiqueta_hora.pack()

        self.boton_mas = tk.Button(
            self,text="+1",command=self.mas,font=("Helvetica", 14),bg="gray20",fg="white",padx=10,pady=5
        )
        self.boton_mas.pack(side="right",padx=10)

        self.boton_menos = tk.Button(
            self,text="-1",command=self.menos,font=("Helvetica", 14),bg="gray20",fg="white",padx=10,pady=5
        )
        self.boton_menos.pack(side="left",padx=10)

        self.indicador = tk.Label(
            self,font=("Helvetica",12,"bold"),bg = "white",fg= "black",pady = 20,padx = 20
        )
        self.indicador.pack()

        self.boton_tempo = tk.Button(
            self,text="Alternar Temp",command=self.tempAlt,font=("Helvetica", 14),bg="gray20",fg="white",padx=10,pady=5
        )

        self.actualizar_reloj()


# FUNCIONES ===========================
    def actualizar_reloj(self):
        match(self.menu):
            case 0:
                self.etiqueta_hora.config(text="Nada por ahora")
            case 1:
                self.etiqueta_hora.config(text="papota")
            case 2:
                hora_actual = time.strftime('%H:%M:%S')
                self.etiqueta_hora.config(text=hora_actual)
            case 3:
                if self.hora_inicio == None and self.boton_temp == True:
                    self.hora_inicio = datetime.now()
                if self.boton_temp == True:
                    segundos = int((datetime.now() - self.hora_inicio).total_seconds())
                    horas = int(segundos / 60 / 60)
                    segundos -= horas*60*60
                    minutos = int(segundos/60)
                    segundos -= minutos*60
                    temporizador = f"{horas:02d}:{minutos:02d}:{segundos:02d}"
                    self.etiqueta_hora.config(text=temporizador)
                    self.boton_tempo.pack()
                else:
                    self.boton_tempo.pack()
                    self.etiqueta_hora.config(text="00:00:00")
            case 4:
                self.etiqueta_hora.config(text="Nada por ahora")

        self.after(1000, self.actualizar_reloj)
        if self.menu != 3:
            hora_inicio = None

    def mas(self):
        print("¡Botón mas presionado!")
        if self.menu < self.menuMax:
            self.menu = self.menu + 1
        self.indicador.config(text=self.menu)
        print(self.menu)

    def menos(self):
        print("¡Botón menos presionado!")
        if self.menu > self.menuMin:
            self.menu = self.menu - 1
        self.indicador.config(text=self.menu)
        print(self.menu)

    def tempAlt(self):
        print("Temporizador iniciado")
        if self.boton_temp != True:
            self.boton_temp = True
        else:
            self.boton_temp = False
        print(self.boton_temp)

def main():
    app = Reloj()
    app.mainloop()

if __name__ == "__main__":
    main()
