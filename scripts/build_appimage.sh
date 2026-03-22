#!/bin/bash
set -e

mkdir -p build/AppDir/usr/bin
mkdir -p build/AppDir/usr/share/applications
mkdir -p build/AppDir/usr/share/icons/hicolor/256x256/apps

cp build/EchoProtocol build/AppDir/usr/bin/
cp -r assets build/AppDir/usr/

cat > build/AppDir/usr/share/applications/echo-protocol.desktop <<'EOF'
[Desktop Entry]
Name=EchoProtocol
Exec=EchoProtocol
Icon=echo-protocol
Type=Application
Categories=Game;
Terminal=false
EOF

chmod 644 build/AppDir/usr/share/applications/echo-protocol.desktop

cp assets/LOGO.png build/AppDir/usr/share/icons/hicolor/256x256/apps/echo-protocol.png

cp build/AppDir/usr/share/applications/echo-protocol.desktop build/AppDir/
cp build/AppDir/usr/share/icons/hicolor/256x256/apps/echo-protocol.png build/AppDir/

cat > build/AppDir/AppRun <<'EOF'

#!/bin/bash

HERE="$(dirname "$(readlink -f "$0")")"
exec "$HERE/usr/bin/EchoProtocol" "$@"
EOF

chmod +x build/AppDir/AppRun

wget -O appimagetool.AppImage https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
chmod +x appimagetool.AppImage

./appimagetool.AppImage --no-appstream build/AppDir build/EchoProtocol.AppImage