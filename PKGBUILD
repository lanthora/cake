pkgname=cake-git
pkgdesc="A GUI client for the candy peer-to-peer mesh network"
pkgver=0
pkgrel=1
arch=('x86_64' 'aarch64')
url="https://github.com/lanthora/cake"
license=('MIT')
depends=('qt6-base' 'poco' 'openssl')
makedepends=('cmake' 'ninja' 'git')
source=('git+https://github.com/lanthora/cake.git')
sha256sums=('SKIP')

pkgver() {
    cd "$srcdir/cake"
    git describe --tags --long 2>/dev/null | sed 's/^v//; s/-/./g' || echo 0.0.0
}

prepare() {
    cmake -S "$srcdir/cake" -B build -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr
}

build() {
    cmake --build build
}

package() {
    DESTDIR="$pkgdir" cmake --install build

    install -Dm644 "$srcdir/cake/cake.desktop" \
        "$pkgdir/usr/share/applications/cake.desktop"
    install -Dm644 "$srcdir/cake/cake.png" \
        "$pkgdir/usr/share/icons/hicolor/256x256/apps/cake.png"
    install -Dm644 "$srcdir/cake/cake.png" \
        "$pkgdir/usr/share/pixmaps/cake.png"
}