vec3 mx_normal_from_sample(sampler2D map, vec2 coord, float strength)
{	
    vec3 nColor = texture2D(map, coord).rgb;	
	vec3 nBase = vec3(0.5, 0.5, 1.0);
    nColor = mix(nBase, nColor, strength);	
	//nColor.g = 1.0 - nColor.g;
	return nColor * 2.0 - 1.0;
}

float mx_lambert(vec3 normal, vec3 nlightDir)
{
	return max(dot(normal, nlightDir), 0.0);	
}


float mx_specular(vec3 normal, vec3 nlightDir, vec3 viewDir, float gloss, float specularPower)
{
	vec3 halfVector = normalize(nlightDir + viewDir);
	float n_dot_h = dot(normal, halfVector);

	return min(pow(clamp(n_dot_h, 0.0f,1.0f), specularPower), gloss);	
}


vec4 testSpecular(vec3 normal, vec3 nlightDir, float att)
{
	return vec4(0.0);
	vec3 camera_position;
	camera_position.xy = view_size/2 - sceneTranslation; //sceneTranslation is negative
	camera_position.z = 100;
	vec3 viewDir = camera_position - vec3(pixel_position.x, pixel_position.y, 0);

	float specular = mx_specular(normal, nlightDir, viewDir, 0.1, 48.0);
	return vec4(1.0) * specular * att;
}

