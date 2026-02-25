v:
cd V:\wokwi-projects\wokwi-motor-chip
wokwi-cli chip compile src\chip-motor.chip.c
copy V:\wokwi-projects\wokwi-motor-chip\src\chip-motor.chip.wasm V:\wokwi-projects\wokwi-motor-chip\dist\chip.wasm 
copy V:\wokwi-projects\wokwi-motor-chip\src\chip-motor.chip.json V:\wokwi-projects\wokwi-motor-chip\dist\chip.json
 tar.exe -a -c -f V:\wokwi-projects\wokwi-LN298module-chip\dist\chip.zip V:\wokwi-projects\wokwi-motor-chip\src\chip-mootor.chip.json V:\wokwi-projects\wokwi-motor-chip\src\chip-motor.chip.wasm V:\wokwi-projects\wokwi-motor-chip\dist\chip.wasm 