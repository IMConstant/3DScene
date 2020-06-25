layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

uniform mat4 u_modelViewProjectionMatrix;
uniform mat4 u_transform;

out vec2 v_texCoords;

void main() {
    gl_Position = u_modelViewProjectionMatrix * u_transform * vec4(position, 1.0);
    v_texCoords = texCoord;
}
