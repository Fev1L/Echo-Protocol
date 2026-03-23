#!/bin/bash
set -e

APP_NAME="EchoProtocol"
DESKTOP_FILE="echo-protocol.desktop"
ICON_NAME="echo-protocol"
APPDIR="build/AppDir"

ARCH="$(uname -m)"

case "$ARCH" in
  x86_64)
    APPIMAGE_ARCH="x86_64"
    OUTPUT_APPIMAGE="build/EchoProtocol-x86_64.AppImage"
    ;;
  aarch64|arm64)
    APPIMAGE_ARCH="aarch64"
    OUTPUT_APPIMAGE="build/EchoProtocol-aarch64.AppImage"
    ;;
  *)
    echo "Unsupported architecture: $ARCH"
    exit 1
    ;;
esac

rm -rf "$APPDIR" build/squashfs-root appimagetool.AppImage
mkdir -p "$APPDIR/usr/bin"
mkdir -p "$APPDIR/usr/share/applications"
mkdir -p "$APPDIR/usr/share/icons/hicolor/256x256/apps"

cp "build/$APP_NAME" "$APPDIR/usr/bin/"
cp -r assets "$APPDIR/usr/"

cat > "$APPDIR/usr/share/applications/$DESKTOP_FILE" <<EOF
[Desktop Entry]
Name=$APP_NAME
Exec=$APP_NAME
Icon=$ICON_NAME
Type=Application
Categories=Game;
Terminal=false
EOF

chmod 644 "$APPDIR/usr/share/applications/$DESKTOP_FILE"

cp assets/LOGO.png "$APPDIR/usr/share/icons/hicolor/256x256/apps/$ICON_NAME.png"

cp "$APPDIR/usr/share/applications/$DESKTOP_FILE" "$APPDIR/$DESKTOP_FILE"
cp "$APPDIR/usr/share/icons/hicolor/256x256/apps/$ICON_NAME.png" "$APPDIR/$ICON_NAME.png"

cat > "$APPDIR/AppRun" <<EOF
#!/bin/bash
HERE="\$(dirname "\$(readlink -f "\$0")")"
exec "\$HERE/usr/bin/$APP_NAME" "\$@"
EOF

chmod +x "$APPDIR/AppRun"

wget -O appimagetool.AppImage "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-${APPIMAGE_ARCH}.AppImage"
chmod +x appimagetool.AppImage

./appimagetool.AppImage --appimage-extract
ARCH="$APPIMAGE_ARCH" ./squashfs-root/AppRun --no-appstream "$APPDIR" "$OUTPUT_APPIMAGE"

cp "$OUTPUT_APPIMAGE" build/EchoProtocol.AppImage

echo "Created AppImage: $OUTPUT_APPIMAGE"