# retromfa
## MFA file image viewer

This project aims to parse MFA binary files and display the images inside using a graphic library.

### About MFA files

MFA (Multimedia Fusion Application) is a format used to store and edit **video game visual assets**. It contains bitmap images along with their metadata.

Two types of color encoding are to be found in the files : 15 bit and 24 bit. Although there are no big differences in viewing, the rendering method is different.

### The retromfa project

The goal of the project is to study MFA files and recognize image signatures inside, then read their metadata and display the pictures. Our implementation leverages the simplicity of the 42 School's `minilibx` to render the pictures.

The retromfa project is designed for educational purposes and the resulting software has no practical use. For advanced viewing and editing of MFA assets, a good idea would be to use [The free Clickteam Fusion software](https://www.clickteam.com/fr/clickteam-fusion-2-5-free-edition).

### Requirements

This project was made on and for **x86** systems running **Linux**. 
Additionally, `make` and a C compiler (`cc`) are needed to build the project from source.

### Installation

1. Clone the repository :
```bash
   git clone https://github.com/ki-ba/retromfa.git retromfa
   cd retromfa
   ```

2. Build the project with `make` then run it:
```bash
make all
./retromfa path/to/file.mfa #file/white1.mfa
```

### License

[MIT License](https://mit-license.org/)
