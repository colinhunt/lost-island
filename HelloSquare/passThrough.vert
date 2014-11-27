// passThrough.vs
//
// Pass-through vertex shader: 
// just multiplies vertex position vector by modelview and projection matrices.

void main()
{	
   gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

   // The following statement is equivalent to the preceding as the derived 
   // matrix gl_ModelViewProjectionMatrix is equal to the matrix product 
   // gl_ProjectionMatrix * gl_ModelViewMatrix. 

   // gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   
   // The following statement is equivalent as well. 
   // It replicates what the fixed functionality pipeline does exactly, 
   // so is better optimized than the preceding two statements.

   // gl_Position = ftransform();
}
