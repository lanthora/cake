# Cake

Candy GUI desktop application implemented by Qt.

## Windows (Build on MINGW64 MSYS2)

安装依赖

```bash
pacman -Syu mingw-w64-x86_64-{cmake,ninja,gcc,libconfig,spdlog,uriparser,poco,qt6-base}
```

进入项目所在目录编译,编译产物放到 `.Release` 目录

```bash
cmake -B .Release -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build .Release
```

执行以下命令复制 `.Release/cake.exe` 和所有依赖到 `artifacts` 目录

```bash
./winqtcollect.sh .Release/cake.exe artifacts
cp .Release/_deps/candy-build/src/tun/wintun/bin/amd64/wintun.dll artifacts
```

此时在 Windows 资源管理器找到 `artifacts` 目录并执行 `cake.exe`,一切正常的话将能看界面.
