import tkinter as tk
from tkinter import simpledialog, filedialog, messagebox
import os
import struct

class FontCanvas:
    def __init__(self, master, width, height, fontfile, chars, char_ids, char_index=0, pixel_size=20):
        self.width = width
        self.height = height
        self.pixel_size = pixel_size
        self.fontfile = fontfile
        self.chars = chars  # list of [ [row bits], ... ]
        self.char_ids = char_ids  # list of IDs
        self.char_index = char_index
        self.pixels = [[0 for _ in range(width)] for _ in range(height)]
        self.canvas = tk.Canvas(master, width=width*pixel_size, height=height*pixel_size, bg='white')
        self.canvas.pack()
        self.canvas.bind("<Button-1>", self.toggle_pixel)
        self.draw_grid()

        self.label = tk.Label(master, text=self._label_text())
        self.label.pack()
        self.id_entry = tk.Entry(master, width=6)
        self.id_entry.pack()
        self.id_entry.insert(0, str(self.char_ids[self.char_index]) if self.char_ids else "0")
        tk.Button(master, text="Go to ID", command=self.goto_id).pack()

        btn_frame = tk.Frame(master)
        btn_frame.pack()
        tk.Button(btn_frame, text="Prev Char", command=self.prev_char).pack(side=tk.LEFT)
        tk.Button(btn_frame, text="Next Char", command=self.next_char).pack(side=tk.LEFT)
        tk.Button(btn_frame, text="Save Char", command=self.save_char).pack(side=tk.LEFT)
        tk.Button(btn_frame, text="Add Char", command=self.add_char).pack(side=tk.LEFT)
        tk.Button(btn_frame, text="Clear", command=self.clear_canvas).pack(side=tk.LEFT)
        tk.Button(btn_frame, text="Export Binary", command=self.export_binary).pack(side=tk.LEFT)

        self.load_char(self.char_index)

    def _parse_id_entry(self):
        val = self.id_entry.get()
        # If it's a 2-digit hex, use as byte
        try:
            if len(val) == 2 and all(c in '0123456789abcdefABCDEF' for c in val):
                return int(val, 16)
        except Exception:
            pass
        # Otherwise, use first byte of UTF-8 encoding
        if val:
            return ord(val[0])
        return 0

    def _label_text(self):
        if self.char_ids:
            cid = self.char_ids[self.char_index]
            # Show as hex and as printable char if possible
            if 32 <= cid < 127:
                return f"Char {self.char_index+1}/{len(self.chars)} (ID: {cid:02X} '{chr(cid)}')"
            else:
                return f"Char {self.char_index+1}/{len(self.chars)} (ID: {cid:02X})"
        else:
            return f"Char {self.char_index+1}/{len(self.chars)}"

    def draw_grid(self):
        self.canvas.delete('all')
        for y in range(self.height):
            for x in range(self.width):
                color = 'black' if self.pixels[y][x] else 'white'
                self.canvas.create_rectangle(
                    x*self.pixel_size, y*self.pixel_size,
                    (x+1)*self.pixel_size, (y+1)*self.pixel_size,
                    fill=color, outline='gray'
                )

    def toggle_pixel(self, event):
        x = event.x // self.pixel_size
        y = event.y // self.pixel_size
        if 0 <= x < self.width and 0 <= y < self.height:
            self.pixels[y][x] ^= 1
            color = 'black' if self.pixels[y][x] else 'white'
            self.canvas.create_rectangle(
                x*self.pixel_size, y*self.pixel_size,
                (x+1)*self.pixel_size, (y+1)*self.pixel_size,
                fill=color, outline='gray'
            )

    def load_char(self, idx):
        if 0 <= idx < len(self.chars):
            flat = self.chars[idx]
            for y in range(self.height):
                for x in range(self.width):
                    bit = (flat[y*self.width + x]) if (y*self.width + x) < len(flat) else 0
                    self.pixels[y][x] = bit
            self.id_entry.delete(0, tk.END)
            cid = self.char_ids[idx]
            # Show as char if printable, else hex
            if 32 <= cid < 127:
                self.id_entry.insert(0, chr(cid))
            else:
                self.id_entry.insert(0, f"{cid:02X}")
        else:
            self.clear_canvas()
        self.label.config(text=self._label_text())
        self.draw_grid()

    def save_char(self):
        # Save current pixels to self.chars at current index
        flat = [self.pixels[y][x] for y in range(self.height) for x in range(self.width)]
        if 0 <= self.char_index < len(self.chars):
            self.chars[self.char_index] = flat
            # Update ID from entry
            new_id = self._parse_id_entry()
            self.char_ids[self.char_index] = new_id
        else:
            self.chars.append(flat)
            new_id = self._parse_id_entry()
            self.char_ids.append(new_id)
        self.save_fontfile()

    def add_char(self):
        # Add a new blank char and switch to it
        flat = [0 for _ in range(self.width * self.height)]
        self.chars.append(flat)
        # Use entry as ID
        new_id = self._parse_id_entry()
        self.char_ids.append(new_id)
        self.char_index = len(self.chars) - 1
        self.load_char(self.char_index)

    def prev_char(self):
        if self.char_index > 0:
            self.char_index -= 1
            self.load_char(self.char_index)

    def next_char(self):
        if self.char_index < len(self.chars) - 1:
            self.char_index += 1
            self.load_char(self.char_index)

    def goto_id(self):
        target_id = self._parse_id_entry()
        for i, cid in enumerate(self.char_ids):
            if cid == target_id:
                self.char_index = i
                self.load_char(i)
                return
        # If not found, add new
        self.chars.append([0 for _ in range(self.width * self.height)])
        self.char_ids.append(target_id)
        self.char_index = len(self.chars) - 1
        self.load_char(self.char_index)

    def clear_canvas(self):
        self.pixels = [[0 for _ in range(self.width)] for _ in range(self.height)]
        self.draw_grid()

    def export_binary(self):
        output_lines = []
        for row in self.pixels:
            padded = row + [0] * ((8 - len(row) % 8) % 8)
            bytestr = []
            for i in range(0, len(padded), 8):
                byte = 0
                for b in range(8):
                    byte = (byte << 1) | padded[i + b]
                bytestr.append(f'{byte:02X}')
            output_lines.append(' '.join(bytestr))
        for line in output_lines:
            print(line)
        print()
        self.clear_canvas()

    def save_fontfile(self):
        with open(self.fontfile, 'wb') as f:
            f.write(bytes([self.width, self.height]))
            for cid, flat in zip(self.char_ids, self.chars):
                f.write(bytes([cid]))
                bits = flat + [0] * ((8 - len(flat) % 8) % 8)
                for i in range(0, len(bits), 8):
                    byte = 0
                    for b in range(8):
                        byte = (byte << 1) | bits[i + b]
                    f.write(bytes([byte]))


def main():
    root = tk.Tk()
    root.title("Font Generator")
    fontfile = filedialog.askopenfilename(defaultextension=".fnt", filetypes=[("Font files", "*.fnt")])
    if not fontfile:
        fontfile = filedialog.asksaveasfilename(defaultextension=".fnt", filetypes=[("Font files", "*.fnt")])
    if not fontfile:
        return
    if not os.path.exists(fontfile):
        width = simpledialog.askinteger("Canvas Width", "Enter canvas width (X):", minvalue=1, maxvalue=64)
        height = simpledialog.askinteger("Canvas Height", "Enter canvas height (Y):", minvalue=1, maxvalue=64)
        chars = []
        char_ids = []
        with open(fontfile, 'wb') as f:
            f.write(bytes([width, height]))
        FontCanvas(root, width, height, fontfile, chars, char_ids)
    else:
        with open(fontfile, 'rb') as f:
            wh = f.read(2)
            width, height = wh[0], wh[1]
            data = f.read()
            chars = []
            char_ids = []
            charsize = width * height
            i = 0
            while i < len(data):
                cid = data[i]
                i += 1
                bits = []
                for _ in range((charsize + 7) // 8):
                    if i < len(data):
                        byte = data[i]
                        i += 1
                    else:
                        byte = 0
                    for b in range(7, -1, -1):
                        bits.append((byte >> b) & 1)
                chars.append(bits[:charsize])
                char_ids.append(cid)
        FontCanvas(root, width, height, fontfile, chars, char_ids)
    root.mainloop()

if __name__ == "__main__":
    main()