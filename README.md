# Argon

A simple UI library that renders to a pixel buffer for C and C++ with retained
mode rendering.

> Currently in early development

## Building

You can build the sources in your own build system with the flags `-Iinclude -Ideps/olive.c`
or you can use the [Beaver Build System](https://github.com/jomy10/beaver).

```sh
# Clone the repository
git clone https://github.com/jomy10/argon --recurse-submdules
cd argon

# Build the project
./make.rb build Argon
```

## Running examples

```sh
./make.rb run test
```

## License

This library is licensed under the [GNU Lesser General Public License v3 (GNU LGPLv3)](COPYING.LESSER).

