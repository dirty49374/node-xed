setlocal

cd build

python ../xed/mfile.py --install-dir="../dist/win" install

mkdir ..\..\cpp\lib\win

copy ..\dist\win\lib\* ..\..\cpp\lib\win
