set -x

brew install openssl libuv cmake
git clone https://github.com/uWebSockets/uWebSockets 
cd uWebSockets
git checkout e94b6e1
patch CMakeLists.txt < ../cmakepatch.txt
mkdir build
export PKG_CONFIG_PATH=/usr/local/opt/openssl/lib/pkgconfig 
cd build
OPENSSL_PATH=$(brew info openssl | awk 'NR==4' | cut -d ' ' -f 1)
cmake -DOPENSSL_ROOT_DIR=$OPENSSL_PATH -DOPENSSL_LIBRARIES=$OPENSSL_PATH/lib ..
make 
sudo make install
cd ..
cd ..
sudo rm -r uWebSockets
