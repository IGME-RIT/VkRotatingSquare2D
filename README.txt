To make this square rotate, we need to pass a "model matrix"
into the vertex buffer, via a "uniform buffer" and then multiply 
each vertex by the model matrix that is in our uniform buffer. 
If a student has never used matrices in shaders before,
PLEASE look at the Atlas OpenGL tutorials for scale, rotation,
and translation. Literally the exact same matrix math is used in
both tutorials, but it is easier to learn it in OpenGL than it is
to learn it in Vulkan. After having a decent understanding of
matrices in OpenGL, then please feel free to try it here in 
this Vulkan tutorial.

In OpenGL, it only takes a few lines to send a matrix to the
vertex shader. In Vulkan, we need a descriptor pool, a 
descriptor layout, and a descriptor set. Then we need to
create the uniform buffer, then we need to update it each
frame. The first time doing this will be difficult, but by practicing,
this will get easier as time goes on. Please read Demo::Prepare()
and all the comments inside it, to see where we add each of these
steps. Please look at Square.vert shader to see where we use
the uniform buffer, the GLSL code is exactly the same as OpenGL.

In our Demo::draw function, we update the uniform buffer,
right before drawing the next frame

