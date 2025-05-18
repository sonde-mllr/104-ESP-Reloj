import tkinter as tk
import time

class RelojApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Reloj con Tkinter")
        self.root.geometry("300x100")
        self.root.resizable(False, False)

        self.label = tk.Label(self.root, font=("Helvetica", 48), fg="black")
        self.label.pack(expand=True)

        self.boton1 = tk.Button(self.root,command=root.destroy,font=("Helvetica", 12), fg="black",)
        self.boton1.config(text="Quitar Reloj")
        self.boton1.pack()

        self.actualizar_reloj()

    def actualizar_reloj(self):
        ahora = time.strftime("%H:%M:%S")
        self.label.config(text=ahora)
        print(self.boton1.pack_info())
        self.root.after(1000, self.actualizar_reloj)

def main():
    root = tk.Tk()
    app = RelojApp(root)
    root.mainloop()

if __name__ == "__main__":
    main()
