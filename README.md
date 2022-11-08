# Dosu

## Building on Lab Machines

1. Clone the repository and enter the directory

```
git clone https://gitlab.com/minidomo/dosu.git
cd dosu
```

2. Build the Godot bindings for C++ 

Run the following command from the root directory:

```
make bindings-lab
```

2. Build the plugin

Run the following command from the root directory:

```
make plugin-lab
```

3. Running the project

Launch Godot and open both `Assignment/Project/project.godot`. Run the project!

**Note:** When opening the project in Godot, files will sometimes get modified unintentionally and may cause unintended behavior when running the project. Ensure that files are unmodified prior to running. If changes are made, restore the original files and reload changes in Godot:

```
git restore .
```

## Other

- [Technical Design Document](pdf/cs354r-final-project-tech-doc.pdf)
