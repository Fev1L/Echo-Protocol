#!/bin/bash
set -e

ARCH="$(uname -m)"

if [ "$ARCH" = "x86_64" ]; then
  APPIMAGE_TOOL_URL="https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage"
  APPIMAGE_ARCH="x86_64"
  OUTPUT_NAME="EchoProtocol-x86_64.AppImage"
elif [ "$ARCH" = "aarch64" ]; then
  APPIMAGE_TOOL_URL="https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-aarch64.AppImage"
  APPIMAGE_ARCH="aarch64"
  OUTPUT_NAME="EchoProtocol-aarch64.AppImage"
else
  echo "Unsupported architecture: $ARCH"
  exit 1
fi

rm -rf build/AppDir build/squashfs-root appimagetool.AppImage
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
cp build/AppDir/usr/share/applications/echo-protocol.desktop build/AppDir/echo-protocol.desktop
cp build/AppDir/usr/share/icons/hicolor/256x256/apps/echo-protocol.png build/AppDir/echo-protocol.png

cat > build/AppDir/AppRun <<'EOF'
#!/bin/bash
HERE="$(dirname "$(readlink -f "$0")")"
exec "$HERE/usr/bin/EchoProtocol" "$@"
EOF

chmod +x build/AppDir/AppRun

wget -O appimagetool.AppImage "$APPIMAGE_TOOL_URL"
chmod +x appimagetool.AppImage

./appimagetool.AppImage --appimage-extract
ARCH="$APPIMAGE_ARCH" ./squashfs-root/AppRun --no-appstream build/AppDir "build/$OUTPUT_NAME"
