..\Bin\glslangValidator.exe -V Square.vert -o Square.vert.spv
..\Bin\glslangValidator.exe -V Square.frag -o Square.frag.spv
..\Bin\spirv-opt --strip-debug Square.vert.spv -o Square2.vert.spv
..\Bin\spirv-opt --strip-debug Square.frag.spv -o Square2.frag.spv
bin2hex --i Square2.vert.spv --o Square.vert.inc
bin2hex --i Square2.frag.spv --o Square.frag.inc
del Square.vert.spv
del Square.frag.spv
del Square2.vert.spv
del Square2.frag.spv
pause