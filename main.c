#include <candle.h>
#include <stdio.h>
#include <components/name.h>
#include <components/decal.h>
#include <components/sprite.h>
#include <components/light.h>
#include <components/model.h>
#include <systems/editmode.h>

int main(int argc, char **argv)
{
	entity_add_component(SYS, (c_t*)c_editmode_new());
	c_editmode_activate(c_editmode(&SYS));

	c_spatial_t *cam = c_spatial(&c_editmode(&SYS)->camera);
	c_spatial_set_pos(cam, vec3(0.0f, 3.0f, 6.0f));
	c_spatial_set_rot(cam, 0.0f, 1.0f, 0.0f, M_PI / 2.0f);

	c_sauces_index_dir(c_sauces(&SYS), "resauces");

	mesh_t *cube = sauces("floor");
	mesh_t *cloth = sauces("cloth");

	entity_new(c_model_new(cube, sauces("tile"), 1, 1));

	entity_t c0 = entity_new(c_model_new(cloth, sauces("velvet"), 1, 1));
	entity_t c1 = entity_new(c_model_new(cloth, sauces("velvet"), 1, 1));
	entity_t c2 = entity_new(c_model_new(cloth, sauces("velvet"), 1, 1));
	entity_t c3 = entity_new(c_model_new(cloth, sauces("velvet"), 1, 1));
	c_model_smooth(c_model(&c0), 1);
	c_model_smooth(c_model(&c1), 1);
	c_model_smooth(c_model(&c2), 1);
	c_model_smooth(c_model(&c3), 1);

	mat1f(sauces("tile"), ref("normal.blend"), 0.3f);

	c_spatial_rotate_Y(c_spatial(&c1), M_PI / 2);
	c_spatial_rotate_Y(c_spatial(&c2), M_PI);
	c_spatial_rotate_Y(c_spatial(&c3), 3 * M_PI / 2);

	mesh_scale_uv(cube, 10);

	entity_t light = entity_new( c_name_new("light"),
			c_light_new(30.0f, vec4(3.5, 3.5, 3.3, 1.0), 512)
	);
	c_spatial_set_pos(c_spatial(&light), vec3(2.0, 8.0, 2.0));

	entity_t ambient = entity_new( c_name_new("light"),
			c_light_new(-1.0f, vec4(0.2, 0.2, 0.3, 1.0), 512)
	);
	c_spatial_set_pos(c_spatial(&ambient), vec3(2.0, 10.0, 2.0));

	mat_t *stone = mat_new("stone", "default");
	mat4f(stone, ref("albedo.color"), vec4(0.9, 0.9, 0.9, 1.0));
	mat1f(stone, ref("roughness.value"), 0.65f);
	mat1t(stone, ref("roughness.texture"), sauces("rough.png"));
	mat1f(stone, ref("roughness.blend"), 0.9f);
	mat1f(stone, ref("metalness.value"), 0.95f);
	/* /1* bok->albedo.texture = sauces("earth_16k.jpg"); *1/ */
	/* bok->albedo.blend = 1; */
	entity_t venus = entity_new(
			c_name_new("venus"),
			c_model_new(sauces("venus.obj"), stone, true, true)
	);
	c_spatial_scale(c_spatial(&venus), vec3(1.3f, 1.3f, 1.3));

	candle_wait();

    return 0;
}
//*/
