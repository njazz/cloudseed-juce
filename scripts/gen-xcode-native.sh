mkdir builds
cd builds
mkdir xcode-native
cd xcode-native

cmake ../.. -G Xcode -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 \
    -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY="iPhone Developer" \
    -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=85478UQJJR -DBUILD_MACOS_FLAGS=1
