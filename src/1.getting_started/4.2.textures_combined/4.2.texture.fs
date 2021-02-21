#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D boxTexture;
uniform sampler2D smileyTexture;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(boxTexture, TexCoord), texture(smileyTexture, TexCoord), 0.2);
}