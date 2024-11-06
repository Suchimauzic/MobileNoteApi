# Mobile Note API

This is my C++ Note API.

## About program

Developed API requires to develop a mobile program. It allows users to interact with the database.

Using this API users can:

* Create and delete accounts
* Create and delete sessions
* Create, update and delete notes
* Update password
* Get all information about note
* Get notes list for a specific user

## Dependencies

The project uses these third party dependencies:

- [Pistache] (https://github.com/pistacheio/pistache)
- [Libpqxx] (https://github.com/jtv/libpqxx)
- [Nlohmann json] (https://github.com/nlohmann/json)
- [GoogleTest] (https://github.com/google/googletest)

## Build and compile

Run install_libs.sh to clone dependencies:

```sh
$ ./install_libs.sh
```

### Using CMake

In this case you need `gcc` and `cmake`.

Create build directory:

```sh
$ mkdir build
$ cd build
```
Next if you want to create a unit-tests program you need to write:

```sh
$ cmake -DMOBILE_UNIT_TEST=TRUE ..
```

or

```sh
$ cmake ..
```

If you don't want you need to write:

```sh
$ cmake -DMOBILE_UNIT_TEST=FALSE ..
```

Build and run the API:

```sh
$ make
```

### Using Docker

In this case you need to write this commands:

```sh
$ docker buildx build -t mobile-note-api .
```

## Usage

### Cmake

Run the program:

```sh
$ ./build/MobileNoteApi
```

Program can be accessed on http://127.0.0.1:9080

Run the unit-tests:

```sh
$ ./build/tests/tests
```

### Docker

```sh
$ docker run -d -p PORT:9080 mobile-note-api
```

Program can be accessed on http://127.0.0.1:PORT

Example:

```sh
$ docker run -d -p 9080:9080 mobile-note-api
```