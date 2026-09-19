#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 vColor;

void main()
{
     gl_Position = vec4(aPos.x / 320.0 - 1.0, aPos.y / 240.0 - 1.0, aPos.z, 1.0);
     vColor = aColor;
     gl_PointSize = 2.0;
}
