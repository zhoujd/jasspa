# Maintainer: Detlef Groth <your@email.com>

pkgname=microemacs
pkgver=24.09.02
pkgrel=1
pkgdesc="Jasspa MicroEmacs Text Editor"
arch=('x86_64')
url="https://github.com/bjasspa/jasspa"
license=('GPL')
source=("mec.exe" "mew.exe" "readme.txt" "COPYING.txt" "macros/")
sha256sums=(
    'digest-mec.exe'
    'digest-mew.exe'
    'digest-readme.txt'
    'digest-COPYING.txt'
    'SKIP')

options=("!strip")
package() {
   install -Dm755 "${srcdir}/bin/windows100-intel32/mec.exe"     "${pkgdir}/usr/bin/mec.exe"
   install -Dm755 "${srcdir}/bin/windows100-intel32/mec.exe"     "${pkgdir}/usr/bin/mew.exe"   
   install -Dm644 "${srcdir}/readme.txt"  "${pkgdir}/usr/share/jasspa/readme.txt"
   install -Dm644 "${srcdir}/COPYING.txt" "${pkgdir}/usr/share/jasspa/COPYING.txt"
   install -Dm644 "${srcdir}/macros/*"    "${pkgdir}/usr/share/jasspa/macros/*"
}
