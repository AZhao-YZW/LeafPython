# LeafPython

## Getting Started

### Git clone this repository

```sh
git clone git@github.com:LeafScript/LeafPython.git
cd LeafPython
git submodule update --init --recursive
```

## Build & Run Project

```sh
cd build
source completion.sh
./leafpy_test.sh -h
# example:
# ./leafpy_test.sh build -t demo -c file_mode
# ./leafpy_test.sh run -t demo -c file_mode
```

## Run Tests

### Use C++ TestMate extension to run tests

1. First, install the `C++ TestMate` extension.
2. append the following code to your `settings.json` file:
```json
{
    "testMate.cpp.test.advancedExecutables": [
        {
            "name": "lib_test",
            "pattern": "test/**/lib_test/**/*{test,Test,TEST}*",
        },
        {
            "name": "testcore_test",
            "pattern": "test/**/testcore_test/**/*{test,Test,TEST}*",
        },
        {
            "name": "leafpy_test",
            "pattern": "test/**/leafpy_test/**/*{test,Test,TEST}*",
        },
    ]
}
```
### Or use leafpy_test.sh to run tests

1. enter `build` directory
2. run command:
```sh
./leafpy_test.sh build -t <llt/hlt/benchmark> -c <case>
```
where `<test_dir_name>` is one of the following: `lib_test`, `core_test/testcore_test`, `core_test/leafpy_test`