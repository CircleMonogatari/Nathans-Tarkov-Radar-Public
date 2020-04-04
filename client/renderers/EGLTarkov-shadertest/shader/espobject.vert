#version 300 es

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

uniform vec2 screen;
uniform vec2 size;
uniform vec4 color;

uniform vec2 screensize;

uniform mat4 cameramatrix;
uniform vec3 camerapos;
uniform vec3 targetpos;

out highp vec2 uv;
out highp vec2 sz;
out highp vec4 c;

vec2 w2s(mat4 Matrix, vec3 WorldPosition)
{
  Matrix = transpose(Matrix);

  vec3 translation = vec3(Matrix[3][0], Matrix[3][1], Matrix[3][2]);
  vec3 up = vec3(Matrix[1][0], Matrix[1][1], Matrix[1][2]);
  vec3 right = vec3(Matrix[0][0], Matrix[0][1], Matrix[0][2]);

  float w = dot(translation, WorldPosition) + Matrix[3][3];

  //if (w < 0.1f)
  //  return vec2(-1000, -1000);

  vec2 screenpos;
  screenpos.x = dot(right, WorldPosition) + Matrix[0][3];
  screenpos.y = dot(up, WorldPosition) + Matrix[1][3];

  screenpos.x = (screensize.x / 2.0) * (1.0 + screenpos.x / w) - (screensize.x / 2.0);
  screenpos.y = -((screensize.y / 2.0) * (1.0 - screenpos.y / w) - (screensize.y / 2.0));

  return screenpos;
}

void main()
{
  sz = vec2(size) + 0.5;

  vec2 finalscreenpos;
  finalscreenpos.xy = w2s(cameramatrix, camerapos) + w2s(cameramatrix, targetpos);

  gl_Position.xyz = vertexPosition_modelspace;
  gl_Position.w   = 1.0;
  gl_Position.xy *= (screen.xy * sz);
  gl_Position.xy += screen.xy * finalscreenpos.xy;

  uv = vertexUV;
  c  = color;
}
