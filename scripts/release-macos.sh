# build with cmake in release configuration
#run from repository root
appname=ddtctmcc
version=1.0.1

#####
mkdir builds
cd builds
#rm -rf release-macos
mkdir release-macos
cd release-macos

set -e
# export SDKROOT=$(xcrun --sdk macosx --show-sdk-path)
cmake ../.. -DWITH_CLAP=TRUE -DMSC_PLATFORM=macOS -DCMAKE_BUILD_TYPE=Release -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY="iPhone Developer" \
    -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=85478UQJJR -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 -DBUILD_MACOS_FLAGS=1 -DCMAKE_OSX_SYSROOT=$(xcrun --sdk macosx --show-sdk-path)

make ${appname}_Standalone -j 8
make ${appname}_AU -j 8
make ${appname}_VST3 -j 8
make ${appname}_CLAP -j 8

cd ../..

mkdir -p bin
cd bin
mkdir -p macos-${version}
cd ..

rm -rf bin/macos-${version}/${appname}.component
rm -rf bin/macos-${version}/${appname}.vst3
rm -rf bin/macos-${version}/${appname}.app
rm -rf bin/macos-${version}/${appname}.clap

cp -r builds/release-macos/${appname}_artefacts/Release/AU/${appname}.component bin/macos-${version}/${appname}.component
cp -r builds/release-macos/${appname}_artefacts/Release/VST3/${appname}.vst3 bin/macos-${version}/${appname}.vst3
cp -r builds/release-macos/${appname}_artefacts/Release/Standalone/${appname}.app bin/macos-${version}/${appname}.app
cp -r builds/release-macos/${appname}_artefacts/Release/CLAP/${appname}.clap bin/macos-${version}/${appname}.clap

cp builds/release-macos/${appname}_artefacts/JuceLibraryCode/${appname}_AU.entitlements bin/macos-${version}/${appname}_AU.entitlements
cp builds/release-macos/${appname}_artefacts/JuceLibraryCode/${appname}_VST3.entitlements bin/macos-${version}/${appname}_VST3.entitlements
cp builds/release-macos/${appname}_artefacts/JuceLibraryCode/${appname}_Standalone.entitlements bin/macos-${version}/${appname}_Standalone.entitlements

#./scripts/sign-and-deploy-bin.sh
