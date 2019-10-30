setlocal

cd build

python ../xed/mfile.py --install-dir="../dist/windows" install

mkdir ..\..\cpp\lib\windows

copy ..\dist\windows\lib\* ..\..\cpp\lib\windows
