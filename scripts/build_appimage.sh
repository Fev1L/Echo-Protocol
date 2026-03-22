#!/bin/bash
set -e

apt-get update
apt-get install -y build-essential cmake wget fuse libfuse2 \
  libasound2-dev libpulse-dev libx11-dev libxext-dev \
  libxcursor-dev libxinerama-dev libxi-dev libxrandr-dev \
  libxss-dev libxtst-dev libwayland-dev libxkbcommon-dev \
  libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \
  libdbus-1-dev libibus-1.0-dev libudev-dev \
  libfreetype-dev libharfbuzz-dev libpng-dev libjpeg-dev \
  libwebp-dev libtiff-dev libavif-dev

mkdir -p build/AppDir/usr/bin
cp build/EchoProtocol build/AppDir/usr/bin/
cp -r assets build/AppDir/usr/

mkdir -p build/AppDir/usr/share/applications
cat > build/AppDir/usr/share/applications/echo-protocol.desktop <<'EOF'
[Desktop Entry]
Name=EchoProtocol
Exec=EchoProtocol
Icon=LOGO
Type=Application
Categories=Game;
EOF

chmod 644 build/AppDir/usr/share/applications/echo-protocol.desktop

mkdir -p build/AppDir/usr/share/icons
cp assets/LOGO.png build/AppDir/usr/share/icons/LOGO.png

wget -O appimagetool.AppImage https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
chmod +x appimagetool.AppImage

./appimagetool.AppImage build/AppDir build/EchoProtocol.AppImage --no-appstream
