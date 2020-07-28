echo "Cleanup"
del *.obj, *.exe

echo "Compile"
cl.exe /c /EHsc MyWindow.cpp

echo "Link"
link.exe MyWindow.obj user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

echo "Run"
MyWindow.exe