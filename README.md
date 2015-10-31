# webFS

Web backed filesystem

## Development

Clone project including submodules
```
git clone --recursive git://github.com/fntlnz/webfs.git
cd webfs
```

Compile a debug enabled binary
```
cmake -DCMAKE_BUILD_TYPE=Debug .
make -j
```

Use it
```
./bin/webfs -d -s -f
```

