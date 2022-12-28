from tkinter import Tk, Canvas, Entry
from tkinter import ttk
import ctypes


class LabyrinthView:
    def __init__(self, master: Tk):
        self.master = master
        master.title("Labyrinth")

        master.geometry("1040x780")
        master.resizable(False, False)

        self.entry1 = Entry(fg="yellow", bg="gray", width=50)
        self.entry1.pack()

        self.entry2 = Entry(fg="yellow", bg="gray", width=50)
        self.entry2.pack()

        self.button = ttk.Button(text="Generate labyrinth", command=self.generate_labyrinth)
        self.button.pack()

    def generate_labyrinth(self):
        # put here your shared lib
        gen_maze_cpp = ctypes.CDLL("labyrinth_generation/liblgpp.so")
        gen_maze_cpp.wrappedGenerateLabyrinth.restype = ctypes.c_char_p
        gen_maze_cpp.wrappedGenerateLabyrinth.argtypes = [ctypes.c_int, ctypes.c_int]

        try:
            width = int(self.entry1.get())
        except ValueError:
            width = 79

        try:
            height = int(self.entry2.get())
        except ValueError:
            height = 59

        self.entry1.pack_forget()
        self.entry2.pack_forget()
        self.button.pack_forget()
        maze = gen_maze_cpp.wrappedGenerateLabyrinth(width, height).decode("utf-8")
        tr_maze = self.retranslate_maze(maze)
        self.set_walls(tr_maze)

        self.master.bind("<Escape>", self.escape_to_menu)

    def escape_to_menu(self, event):
        self.canvas.pack_forget()
        self.entry1.pack()
        self.entry2.pack()
        self.button.pack()

    def set_walls(self, maze: list[list[str]]):
        SIZE = 13

        self.canvas = Canvas(self.master)
        self.canvas.configure(background='#343231', width=1040, height=780)
        self.canvas.pack()

        for i in range(0, len(maze)):
            for j in range(0, len(maze[i])):
                self.canvas.create_rectangle(j * SIZE, i * SIZE, j * SIZE + SIZE, i * SIZE + SIZE, fill=maze[i][j])

    def retranslate_maze(self, maze: str) -> list[list[str]]:
        tr_maze = []
        rows = maze.split("\n")
        rows.pop()

        for row in rows:
            tr_maze.append([])
            for cell in row:
                if cell == "1":
                    tr_maze[-1].append("#31B7A3")
                else:
                    tr_maze[-1].append("#343231")

        return tr_maze


def read_maze() -> list[list[str]]:
    maze = []
    with open("mazes/maze-rofl.maze", "r") as maze_file:
        for line in maze_file:
            maze.append([])
            buff = line.rstrip("\n")
            for item in buff:
                if item == "1":
                    maze[-1].append("#31B7A3")
                else:
                    maze[-1].append("#343231")
    return maze


def main():
    root = Tk()
    window = LabyrinthView(root)

    # maze = read_maze()
    # window.set_walls(maze)

    root.mainloop()


if __name__ == "__main__":
    main()
