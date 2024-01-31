# Development

# Building:

To build package and install in "development" mode, execute the `setup.py` using `pip3`:

```
pip3 install -e .
```

# Testing:

To run unit tests, execute the helper script `runtests`:

```
python3 -m runtests
```

# Tips

## Generate `compile_commands.json` (for Text Editors that support LSP)

If you have a text editor with a C/C++ language server installed (e.g., [clangd](https://clangd.llvm.org/design/compile-commands))
you can wrap the `pip3 install` command using
[bear](https://github.com/rizsotto/Bear) to automatically generate a
`compile_commands.json` file:

```
bear -- pip3 install -e ./
```

Your C/C++ language server should automatically be able to read this
to enable intelligent code navigation like go-to-definition, etc.
