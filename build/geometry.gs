#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
  vec3 color;
} gs_in[];

out vec3 fColor;

void build_house(vec4 position)
{
  fColor = gs_in[0].color; // [0] since theres only on input vertex
  gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // bot left
  EmitVertex();
  gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0); // bot right
  EmitVertex();
  gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0); // top left
  EmitVertex();
  gl_Position = position + vec4(0.2,  0.2, 0.0, 0.0); // top right
  EmitVertex();
  gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0); // top
  fColor = vec3(1.0, 1.0, 1.0);
  EmitVertex();
  EndPrimitive();
}

void main()
{
  build_house(gl_in[0].gl_Position);
}