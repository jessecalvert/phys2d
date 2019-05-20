#ifdef VERTEX_SHADER

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec4 Color;

out vec2 TexCoordsToFragment;
out vec4 ColorToFragment;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform mat3 TexTransform;

void main()
{
    gl_Position = Projection * View * Model * vec4(Position, 1.0f);
	TexCoordsToFragment = (TexTransform*vec3(TexCoords, 1.0f)).xy;
	ColorToFragment = Color;
}


#endif
#ifdef FRAGMENT_SHADER

layout(location = 0) out vec4 FragColor;

in vec2 TexCoordsToFragment;
in vec4 ColorToFragment;

uniform sampler2D Texture;

void main()
{
	FragColor = ColorToFragment;
	FragColor.a *= texture(Texture, TexCoordsToFragment).a;
}

#endif
