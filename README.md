# Riny Tenderer

Riny Tenderer is a tiny C++ project that takes an OBJ file, creates a 3D model, applies textures, adds lighting, performs perspective projection, and renders the output as a TGA image. This project is inspired by and credits ssloy on GitHub for foundational concepts and techniques.

## Features

- Load and parse OBJ files to create 3D models.
- Apply textures to the 3D models.
- Add basic lighting effects.
- Implement perspective projection for the camera.
- Render the final image and save it as a TGA file.

## Requirements

- A C++ compiler (GCC, Clang, etc.).
- Make utility.
- Standard C++ library.

## Getting Started

### Cloning the Repository

```sh
git clone https://github.com/Asher-Ul-Haque/RinyTenderer.git
cd riny-tenderer
```

### Building the Project

To build the project, simply run:

```sh
make
```

This will compile the source files and generate the executable.

### Running the Project

Once the project is built, you can run the executable with an example OBJ file:

```sh
./RinyTender path/to/your/model.obj
```

The output will be a TGA image file saved in the project output

## Project Structure

## Credits

This project is heavily inspired by the work of [ssloy](https://github.com/ssloy). Please check out their amazing work on rendering and graphics programming.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

---

Enjoy using Riny Tenderer! If you have any questions or suggestions, feel free to open an issue on the GitHub repository.
