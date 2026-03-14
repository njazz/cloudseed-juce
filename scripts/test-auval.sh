# build and run auval tool

mkdir builds
cd builds
# rm -rf for-auval-macOS
mkdir for-auval-macOS
cd for-auval-macOS

set -e
# export SDKROOT=$(xcrun --sdk macosx --show-sdk-path)
cmake ../.. -DAPP_PERSONAL_BUILD="[auval]" -DMSC_PLATFORM=macOS -DCMAKE_BUILD_TYPE=Release -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY="iPhone Developer" \
    -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=85478UQJJR -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 -DBUILD_MACOS_FLAGS=1 -DCMAKE_OSX_SYSROOT=$(xcrun --sdk macosx --show-sdk-path)

make ddtctmcc_AU -j 8
make ddtctmcc-ff_AU -j 8

#make install

rm -rf ~/Library/Audio/Plug-Ins/Components/ddtctmcc.component
rm -rf ~/Library/Audio/Plug-Ins/Components/ddtctmcc-ff.component

cp -r ddtctmcc_artefacts/Release/AU/ddtctmcc.component ~/Library/Audio/Plug-Ins/Components/ddtctmcc.component
cp -r ddtctmcc-ff_artefacts/Release/AU/ddtctmcc-ff.component ~/Library/Audio/Plug-Ins/Components/ddtctmcc-ff.component

cd ../..


auval -v aumi DtmC Andj #aumf aufx
auval -v aumi DtmE Andj #aumf aufx
