# Clone emsdk
git clone https://github.com/emscripten-core/emsdk.git lib/emsdk

# Navigate to emsdk
pushd
cd lib/emsdk

# Ensure emsdk is up to date
git pull

# Install and activate emsdk
./emsdk install latest
./emsdk activate latest

# Navigate out of emsdk
popd
