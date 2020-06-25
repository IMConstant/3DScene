uniform sampler2D depthMap;

in vec2 v_texCoords;

void main() {
    float depthValue = texture(depthMap, v_texCoords).r;
    gl_FragColor = vec4(vec3(depthValue), 1.0);
}
