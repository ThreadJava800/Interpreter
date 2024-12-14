# Interpreter

It is a simple interpreter of PASCAL, which supports:
1. Arithmetics
2. Logic functions (and, or, not)
3. Terminal output
4. IF-THEN-ELSE
5. Cycles (for, while)
6. Strings

## Build
Implementation uses [flex](https://github.com/westes/flex) and [GNU Bison](https://www.gnu.org/software/bison/). so in order to build the project, you must have them installed.

```bash
mkdir build
cd build
cmake ..
cd ..
make
```

## Run