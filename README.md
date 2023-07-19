# Learning OpenGL

Learning computer graphics programming with OpenGL via [Learn OpenGL](https://learnopengl.com/) by [Joey de Vries](https://joeydevries.com/). My progress following the Learn OpenGL tutorials can be found here.

## Visual Studio Configurations

### Output location
*Project > Properties > Configuration Properties > General*
```
Output Directory: $(SolutionDir)bin\$(Platform)\$(Configuration)\
Intermediate Directory: $(SolutionDir)bin\intermediates\$(Platform)\$(Configuration)\
```

### Locate DLLs
*Project > Properties > Configuration Properties > Debugging*
```
Environment: PATH=$(SolutionDir)dll\
```

### Locate include files
*Project > Properties > Configuration Properties > C/C++ > General*
```
Additional Include Directories: $(SolutionDir)include\
```

### Locate library files
*Project > Properties > Configuration Properties > Linker > General*
```
Additional Library Directories: $(SolutionDir)lib\
```

### Specify dependencies
*Project > Properties > Configuration Properties > Linker > Input*
```
Additional Dependencies: assimp.lib;glfw3.lib;opengl32.lib;%(AdditionalDependencies)
```
