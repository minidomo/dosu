# dosu

## Building

1. Clone the repository and enter the directory

```
git clone https://gitlab.com/minidomo/dosu.git
cd dosu
```

2. Set up `config.json`

If you don't have a `config.json` file in the root directory, run the following to create the file:

```
node scripts/config-init.js
```

Follow the comments in `config.json` to properly assign values. Since it's a JSON file, delete the comments once the file is set up.


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

Launch Godot and open `dosu/project.godot`. Run the project!

## Other

- [Technical Design Document](pdf/cs354r-final-project-tech-doc.pdf)
- [Alpha Document](pdf/cs354r-alpha.pdf)
- [Alpha Video](https://www.youtube.com/watch?v=HFe5l2v143Q)
