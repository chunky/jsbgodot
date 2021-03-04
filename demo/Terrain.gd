extends Spatial


func _ready():
	for lat in range(25.0, 45.0, 1.0):
		for lon in range(-130.0, -90.0, 1.0):
			print(str(lat) + "," + str(lon));
			var meshinstance = createOneTile(lat, lon, 0.5, 50);
			add_child(meshinstance);
	print("done");

func createOneTile(centerLat, centerLon, dTheta, meshdivisions):
	var mi = preload("res://TerrainTile.tscn").instance();
	
	mi.mesh.subdivide_width = meshdivisions;
	mi.mesh.subdivide_depth = meshdivisions;
	
	var img = $GDALImager.create_image(centerLat+dTheta, centerLon-dTheta,
		  centerLat-dTheta, centerLon+dTheta,
		  meshdivisions+1, meshdivisions+1);
		
	var texture = ImageTexture.new();
	texture.create_from_image(img);
	var mat = mi.mesh.surface_get_material(0);
	
	mat.set_shader_param("tex", texture);
	# print(mat.get_shader_param("tex"));
	mi.translate(Vector3(centerLon, 0, centerLat));
	
	mi.add_to_group("terrain_tiles");
	return mi;
