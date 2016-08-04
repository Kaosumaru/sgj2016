#include <functions/hue_saturation.glsl>

vec4 mx_make_enemy_color(vec4 rgba, vec4 hsva_adj)
{
	rgba.rgb = mx_hue_saturation(rgba.rgb, hsva_adj.r, hsva_adj.g) * hsva_adj.b;
	rgba.a *= hsva_adj.a;
	return rgba;
}
