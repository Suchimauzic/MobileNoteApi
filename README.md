# Mobile Note API

This is my C++ Note API.

## About program

Developed API requires to develop a mobile program. It allows users to interact with the database.

Using this API users can:

* Create and delete accounts
* Create and delete sessions
* Create, update and delete notes
* Update password
* Get note and notes

## Build and compile

For building the project you need to clone libraries. Use this command:

```sh
$ ./install_libs.sh
```

### Using CMake

In this case you need `gcc` and `cmake`.

First step you have to create a build directory:

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

And when you finished building the project you have to compile it:

```sh
$ make
```

After successful compiling you can start the API:

```sh
$ ./MobileNoteApi
```

### Using Docker

In this case you need to write this commands:

```sh
$ sudo docker buildx build -t mobile-note-api .
$ sudo docker run mobile-note-api
```