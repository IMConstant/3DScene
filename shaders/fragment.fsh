uniform vec3 u_light_position;
uniform vec3 u_camera_position;
uniform mat4 u_transform;
uniform vec4 u_light_color;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_frag_position;
in vec4 v_frag_pos_light_space;


layout (binding = 0) uniform sampler2D samp;
layout (binding = 1) uniform sampler2D shadowMap;


float far_plane = 10000.0;
float near_plane = 0.2;


float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}


float calcShadow() {
    vec3 projCoords = v_frag_pos_light_space.xyz / v_frag_pos_light_space.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = LinearizeDepth(texture(shadowMap, projCoords.xy).r) / far_plane;

    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(v_normal, normalize(u_light_position - v_frag_position))), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    if (currentDepth > 1.0)
        shadow = 0.0;

    return shadow;
}

void main() {
    float k_diffuse = 0.8;
    float k_specular = 0.9;
    float ambient = 0.1;

    vec3 normal = normalize(v_normal);
    vec3 lightvector = normalize(u_light_position - v_frag_position);
    vec3 lookvector = normalize(u_camera_position - v_frag_position);

    float p = length(u_light_position - v_frag_position) / 3.0;

    if (p > 1.0) {
        ambient = 0.03;
        p = 1.0;
    }
    else {
        ambient = 0.03 + 0.1 * (1.0 - p);
    }

    vec4 light_color = vec4(1.0, 0.3, 0.0, 1.0);

    float diffuse = (1.0 - p) * k_diffuse * max(dot(normal, lightvector), 0.0);
    vec3 reflectvector = reflect(-lightvector, normal);
    float specular = (1.0 - p) * k_specular * pow(max(dot(lookvector, reflectvector), 0.0), 40.0);

    vec4 texColor = texture(samp, v_texCoord);
    float shadow = calcShadow();
    gl_FragColor = (ambient + (1.0 - shadow) * (diffuse + specular)) * texColor * u_light_color;
    gl_FragColor.a = 1.0;

    float dist = distance(v_frag_position, u_light_position);
    float max_dist = 0.5;

    if (dist <= max_dist) {
        gl_FragColor += (diffuse + specular) * pow(1.0 - dist / max_dist, 2) * (u_light_color - gl_FragColor);
        //gl_FragColor.a = 1.0;
    }
}