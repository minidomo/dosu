# dosu

## Building

1. Clone the repository and enter the directory

```
git clone https://gitlab.com/minidomo/dosu.git
cd dosu
```

2. Setup `config.json`

If you don't have a `config.json` file in the root directory, run the following to create the file:

```
make config
```

Follow the comments in `config.json` to properly assign values.


3. Build the Godot bindings for C++ 

Run the following command from the root directory:

```
make bindings
```

4. Build the plugin

Run the following command from the root directory:

```
make plugin
```

5. Running the project

Launch Godot and open both `dosu/project.godot`. Run the project!

**Note:** When opening the project in Godot, files will sometimes get modified unintentionally and may cause unintended behavior when running the project. Ensure that files are unmodified prior to running. If changes are made, restore the original files and reload changes in Godot:

```
git restore .
```

## Other

- [Technical Design Document](pdf/cs354r-final-project-tech-doc.pdf)
