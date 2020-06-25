layout (location = 0) in vec3 position;

uniform mat4 u_light_space_matrix;
uniform mat4 u_transform;

void main() {
    gl_Position = u_light_space_matrix * u_transform * vec4(position, 1.0);
}
