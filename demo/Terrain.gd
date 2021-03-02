extends Spatial


func _ready():
	prepOneImage(23.58, 121.0, 0.2);

func prepOneImage(centerLat, centerLon, dTheta):
	
	var img = $GDALImager.create_image(centerLat+dTheta, centerLon-dTheta, centerLat-dTheta, centerLon+dTheta);
	var texture = ImageTexture.new();
	texture.create_from_image(img);
	var mat = $MeshInstance.mesh.surface_get_material(0);
	mat.set_shader_param("tex", texture);
	print(mat.get_shader_param("tex"));
