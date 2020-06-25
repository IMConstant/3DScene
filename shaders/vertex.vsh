layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 u_modelViewProjectionMatrix;
uniform mat4 u_transform;
uniform mat4 u_light_space_matrix;

uniform vec3 u_light_position;
uniform vec3 u_camera_position;

out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_frag_position;
out vec4 v_frag_pos_light_space;


void main(void) {
    gl_Position = u_modelViewProjectionMatrix * u_transform * vec4(position.xyz, 1.0);
    v_texCoord = texCoord;
    v_normal = transpose(inverse(mat3(u_transform))) * normal;//vec3(u_transform * vec4(normal, 0.0));
    v_frag_position = vec3(u_transform * vec4(position, 1.0));
    v_frag_pos_light_space = u_light_space_matrix * vec4(v_frag_position, 1.0);
}