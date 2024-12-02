@echo off
echo *** Build all clean: %DATE% %TIME% *** > buildall.log
call build -la buildall.log %* -C
call build -la buildall.log %* -C -t c
call build -la buildall.log %* -C -t cw
call build -la buildall.log %* -C -d
call build -la buildall.log %* -C -d -t c
call build -la buildall.log %* -C -d -t cw
call build -la buildall.log %* -C -ne
call build -la buildall.log %* -C -ne -t c
call build -la buildall.log %* -C -ne -t cw
call build -la buildall.log %* -C -ne -d
call build -la buildall.log %* -C -ne -d -t c
call build -la buildall.log %* -C -ne -d -t cw
echo *** Build all start: %DATE% %TIME% *** >> buildall.log
call build -la buildall.log %*
call build -la buildall.log %* -t c
call build -la buildall.log %* -t cw
call build -la buildall.log %* -d
call build -la buildall.log %* -d -t c
call build -la buildall.log %* -d -t cw
call build -la buildall.log %* -ne
call build -la buildall.log %* -ne -t c
call build -la buildall.log %* -ne -t cw
call build -la buildall.log %* -ne -d
call build -la buildall.log %* -ne -d -t c
call build -la buildall.log %* -ne -d -t cw
echo *** Build all complete: %DATE% %TIME% *** >> buildall.log
