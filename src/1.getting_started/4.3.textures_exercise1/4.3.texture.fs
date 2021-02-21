#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
        
  // linearly interpolate between both textures (80% container, 20% awesomeface)
  //vec2 mycoord = TexCoord;
  vec2 flipped = vec2(1.0 - TexCoord.x, TexCoord.y);
  FragColor = mix(texture(texture1, TexCoord), texture(texture2, flipped), 0.2);
}