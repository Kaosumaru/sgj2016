void mx_glposition_instanced_vertex()
{
	vec2 pos;
	if (instance_angle == 0)
	{
		pos = xywh.xy + squareVertices * xywh.zw - instance_center.xy;
		gl_Position = MVP * vec4(pos.x,pos.y,0.0,1.0);
	}
	else
	{
		vec2 up = vec2(sin(instance_angle), -cos(instance_angle));
		vec2 left = vec2(up.y, -up.x);

		//for left side x = instance_center.x, for right x = instance_center - xywh.z
		//same for top/bottom
		vec2 distance_from_center = instance_center - xywh.zw * squareVertices;

		up *= distance_from_center.y;
		left *= distance_from_center.x;

		pos = xywh.xy + up + left;

		gl_Position = MVP * vec4(pos.x,pos.y,0.0,1.0);
	}

	vec2 uv_index = squareVertices * 2.0;
	UV.x = instance_uv[int(uv_index.x)];
	UV.y = instance_uv[int(uv_index.y + 1)];

	/*
	//that fixes crash on intel 530
	vec4 copied_uv;
	copied_uv.xyzw = instance_uv.yxwz;
	vec2 uv_index = squareVertices * 2.0;
	UV.x = copied_uv[int(uv_index.x) + 1];
	UV.y = copied_uv[int(uv_index.y)];
	*/
}
