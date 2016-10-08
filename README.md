# Programming Contests Library

## Usage

#### Run tests
```bash
make
make test
```

#### Run benchmarks
```bash
make
make test
```

## Dependencies

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
