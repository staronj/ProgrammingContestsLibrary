# Programming Contests Library

## Usage

#### Build documentation
```bash
make doc
```

#### Run tests
```bash
make
make test
```

#### Run benchmarks

Build target `all`, go to subdirectory `benchmarks` and run executable.

## Dependencies

### Doxygen

Tool for generating documentation. 

#### Installation
```bash
sudo apt-get install doxygen
```

### Boost unit test framework

See Boost unit test framework [documentation](http://www.boost.org/doc/libs/1_58_0/libs/test/doc/html/utf.html).

#### Installation
```bash
sudo apt-get install libboost-test-dev
```

### Celero benchmark framework

See Celero project on [GitHub](https://github.com/DigitalInBlue/Celero).

#### Installation
```bash
cd ~/
mkdir celero && cd celero
git clone https://github.com/DigitalInBlue/Celero.git
mkdir build
cd build
cmake ../Celero
make -j
sudo make install
cd ..
sudo cp -R Celero/include/celero /usr/local/include/
```

### Regex python package

#### Installation
```bash
pip install regex
```
