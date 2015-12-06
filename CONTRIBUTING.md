# Contributing

This project is very young, it's not a lot of code and given the fact that I'm not a fan obscure things and voodoo magic it should also be understandable.

Given that, any help is appreciated :D

With any help I mean really any, from typos to whatever you want.

## Targets

Webfs is actually three things

- *libwebfs* -> The library that handles storage, directories tree and which can be embedded into another project
- *webfs* -> An executable that allows to mount webfs via FUSE. This makes use of **libwebfs** internally.
- **libwebfstest** -> The libwebfs testsuite

## Build steps
Webfs depends on a few libraries that on Fedora can be obtained with a:

```
dnf install gcc cmake make fuse-devel curl-devel
```

I'm using cmake to generate build files, since you're hacking with webfs you're going to need
a debug enabled version:
```
cmake -DCMAKE_BUILD_TYPE=Debug .
```

This command will build the libwebfs and webfstest targets.
```
make
```

To build the webfs executable you need to specify it

```
make webfs
```


