v:
cd V:\wokwi-projects\wokwi-motor-chip
wokwi-cli chip compile src\chip-motor.chip.c
cd dist
copy ..\src\chip-motor.chip.wasm chip.wasm 
copy ..\src\chip-motor.chip.json chip.json

tar.exe -acf chip.zip chip.json chip.wasm
cd ..