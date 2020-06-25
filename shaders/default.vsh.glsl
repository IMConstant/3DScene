layout (location = 0) in vec3 position;

uniform mat4 u_modelViewProjectionMatrix;
uniform mat4 u_transform;

void main() {
    gl_Position = u_modelViewProjectionMatrix * u_transform * vec4(position.xyz, 1.0);
}
