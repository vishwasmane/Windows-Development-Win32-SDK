echo "Cleanup"
del *.obj, *.exe

echo "Compile"
cl.exe /c /EHsc MyWindow.cpp

echo "Resource Compile"
rc.exe MyWindow.rc

echo "Link"
link.exe MyWindow.obj MyWindow.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

echo "Run"
MyWindow.exe